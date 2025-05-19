#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <limits>
#include <algorithm>
#include <numeric>

using namespace std;

class Ant {
public:
    vector<int> path;
    vector<bool> visited;
    double path_length;

    Ant(int start_city, int num_cities) {
        path.push_back(start_city);
        visited.resize(num_cities, false);
        visited[start_city] = true;
        path_length = 0.0;
    }
};

class ACO_TSP {
private:
    int num_cities;
    vector<vector<double>> distance_matrix;
    vector<vector<double>> pheromone;
    int num_ants;
    double alpha, beta, evaporation_rate, Q;
    int max_iterations;

    void initialize_pheromone() {
        double initial_pheromone = 1.0;
        pheromone.resize(num_cities, vector<double>(num_cities, initial_pheromone));
    }

public:
    ACO_TSP(vector<vector<double>> matrix, int num_ants, double alpha, double beta,
        double evaporation_rate, double Q, int max_iterations)
        : distance_matrix(matrix), num_cities(matrix.size()), num_ants(num_ants),
        alpha(alpha), beta(beta), evaporation_rate(evaporation_rate),
        Q(Q), max_iterations(max_iterations) {
        initialize_pheromone();
    }

    double calculate_path_length(const vector<int>& path) {
        double length = 0.0;
        for (size_t i = 0; i < path.size() - 1; ++i) {
            length += distance_matrix[path[i]][path[i + 1]];
        }
        length += distance_matrix[path.back()][path[0]];
        return length;
    }

    pair<vector<int>, double> solve() {
        vector<int> best_path(num_cities);
        iota(best_path.begin(), best_path.end(), 0);
        double best_path_length = calculate_path_length(best_path);

        random_device rd;
        mt19937 gen(rd());

        for (int iter = 0; iter < max_iterations; ++iter) {
            vector<vector<double>> delta_pheromone(num_cities, vector<double>(num_cities, 0.0));
            vector<Ant> ants;

            uniform_int_distribution<int> start_city(0, num_cities - 1);
            for (int i = 0; i < num_ants; ++i) {
                ants.emplace_back(start_city(gen), num_cities);
            }

            for (Ant& ant : ants) {
                while (ant.path.size() < num_cities) {
                    int current_city = ant.path.back();
                    vector<double> probabilities;
                    vector<int> available_cities;

                    double sum = 0.0;
                    for (int j = 0; j < num_cities; ++j) {
                        if (!ant.visited[j]) {
                            double ph = pow(pheromone[current_city][j], alpha);
                            double vis = pow(1.0 / distance_matrix[current_city][j], beta);
                            sum += ph * vis;
                            probabilities.push_back(ph * vis);
                            available_cities.push_back(j);
                        }
                    }

                    if (available_cities.empty()) {
                        ant.path_length = numeric_limits<double>::max();
                        break;
                    }

                    uniform_real_distribution<double> dist(0.0, 1.0);
                    double rand_val = dist(gen);
                    double cumulative = 0.0;
                    int next_city = available_cities.back();

                    if (sum > 0) {
                        for (size_t i = 0; i < probabilities.size(); ++i) {
                            probabilities[i] /= sum;
                            cumulative += probabilities[i];
                            if (rand_val <= cumulative) {
                                next_city = available_cities[i];
                                break;
                            }
                        }
                    }

                    ant.path.push_back(next_city);
                    ant.visited[next_city] = true;
                    ant.path_length += distance_matrix[current_city][next_city];
                }

                if (ant.path.size() == num_cities) {
                    ant.path_length += distance_matrix[ant.path.back()][ant.path[0]];
                    ant.path.push_back(ant.path[0]);

                    if (ant.path_length < best_path_length) {
                        best_path_length = ant.path_length;
                        best_path = ant.path;
                    }

                    double contribution = Q / ant.path_length;
                    for (size_t i = 0; i < ant.path.size() - 1; ++i) {
                        int from = ant.path[i];
                        int to = ant.path[i + 1];
                        delta_pheromone[from][to] += contribution;
                        delta_pheromone[to][from] += contribution;
                    }
                }
            }

            for (int i = 0; i < num_cities; ++i) {
                for (int j = 0; j < num_cities; ++j) {
                    pheromone[i][j] = pheromone[i][j] * evaporation_rate + delta_pheromone[i][j];
                }
            }
        }

        return { best_path, best_path_length };
    }
};

int main() {
    vector<vector<double>> distance_matrix = {
        {0,   230, 321, 432, 442, 533, 320},
        {334, 0,   219, 673, 651, 344, 762},
        {456, 776, 0,   443, 622, 632, 511},
        {877, 446, 840, 0,   332, 542, 631},
        {432, 112, 532, 762, 0,   544, 226},
        {633, 422, 773, 533, 721, 0,   652},
        {238, 232, 645, 761, 354, 631, 0}
    };

    int num_ants = distance_matrix.size();
    double alpha = 1.0;
    double beta = 2.0;
    double evaporation_rate = 0.5;
    double Q = 100.0;
    int max_iterations = 100;

    ACO_TSP solver(distance_matrix, num_ants, alpha, beta, evaporation_rate, Q, max_iterations);
    auto result = solver.solve();

    cout << result.second << endl;
    for (int city : result.first) {
        cout << city << " ";
    }
    cout << endl;

    return 0;
}