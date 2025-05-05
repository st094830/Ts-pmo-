#pragma once
#ifndef _PLUH_
#define _PLUH_
#include <vector>
#include <cmath>
#include <stdexcept>
#include <iostream>

class LeastSquaresSolver {
public:
    void linearFit(const std::vector<double>& x, const std::vector<double>& y) {
        if (x.size() != y.size() || x.size() < 2) {
            throw std::invalid_argument("Invalid input data");
        }

        double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
        const size_t n = x.size();

        for (size_t i = 0; i < n; ++i) {
            sum_x += x[i];
            sum_y += y[i];
            sum_xy += x[i] * y[i];
            sum_x2 += x[i] * x[i];
        }

        double denominator = n * sum_x2 - sum_x * sum_x;
        if (fabs(denominator) < 1e-10) {
            throw std::runtime_error("System is singular");
        }

        k = (n * sum_xy - sum_x * sum_y) / denominator;
        b = (sum_y - k * sum_x) / n;
    }


    void quadraticFit(const std::vector<double>& x, const std::vector<double>& y) {
        if (x.size() != y.size() || x.size() < 3) {
            throw std::invalid_argument("Invalid input data");
        }

        double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;
        double sum_x3 = 0, sum_x4 = 0, sum_x2y = 0;
        const size_t n = x.size();

        for (size_t i = 0; i < n; ++i) {
            const double xi = x[i];
            const double xi2 = xi * xi;
            const double xi3 = xi2 * xi;
            const double xi4 = xi3 * xi;

            sum_x += xi;
            sum_y += y[i];
            sum_xy += xi * y[i];
            sum_x2 += xi2;
            sum_x3 += xi3;
            sum_x4 += xi4;
            sum_x2y += xi2 * y[i];
        }

        double mat[3][3] = {
            {sum_x4, sum_x3, sum_x2},
            {sum_x3, sum_x2, sum_x},
            {sum_x2, sum_x, static_cast<double>(n)}
        };

        double rhs[3] = { -sum_x2y, -sum_xy, -sum_y };

        if (!solve3x3(mat, rhs)) {
            throw std::runtime_error("Failed to solve quadratic system");
        }

        A_coef = rhs[0];
        B_coef = rhs[1];
        C_coef = rhs[2];
    }

    double getSlope() const { return k; }
    double getIntercept() const { return b; }
    double getA() const { return A_coef; }
    double getB() const { return B_coef; }
    double getC() const { return C_coef; }

    double linearPredict(double x) const {
        return k * x + b;
    }

    double quadraticPredict(double x) const {
        return A_coef * x * x + B_coef * x + C_coef;
    }

private:
    double k = 0;
    double b = 0;

    double A_coef = 0;
    double B_coef = 0;
    double C_coef = 0;

    bool solve3x3(double mat[3][3], double rhs[3]) {
        const double eps = 1e-12;

        for (int col = 0; col < 3; ++col) {
            int max_row = col;
            for (int row = col + 1; row < 3; ++row) {
                if (fabs(mat[row][col]) > fabs(mat[max_row][col])) {
                    max_row = row;
                }
            }

            if (max_row != col) {
                for (int i = 0; i < 3; ++i) {
                    std::swap(mat[col][i], mat[max_row][i]);
                }
                std::swap(rhs[col], rhs[max_row]);
            }

            if (fabs(mat[col][col]) < eps) {
                return false;
            }

            for (int row = col + 1; row < 3; ++row) {
                double factor = mat[row][col] / mat[col][col];
                for (int i = col; i < 3; ++i) {
                    mat[row][i] -= factor * mat[col][i];
                }
                rhs[row] -= factor * rhs[col];
            }
        }

        for (int col = 2; col >= 0; --col) {
            rhs[col] /= mat[col][col];
            for (int row = 0; row < col; ++row) {
                rhs[row] -= mat[row][col] * rhs[col];
            }
        }

        return true;
    }
};

int main() {
    std::vector<double> x = { 1, 2, 3, 4, 5 };
    std::vector<double> y = { 2, 4, 5, 4, 5 };

    LeastSquaresSolver solver;
    solver.linearFit(x, y);

    std::cout << "Linear model: y = " << solver.getSlope() << "x + "
        << solver.getIntercept() << std::endl;
    std::cout << "Prediction at x=6: " << solver.linearPredict(6) << std::endl;


    std::vector<double> x = { 1, 2, 3, 4, 5 };
    std::vector<double> y = { 2.1, 3.9, 6.1, 8.9, 12.1 };

    LeastSquaresSolver solver;
    solver.quadraticFit(x, y);

    std::cout << "Quadratic model: y = " << solver.getA() << "x² + "
        << solver.getB() << "x + " << solver.getC() << std::endl;
    std::cout << "Prediction at x=6: " << solver.quadraticPredict(6) << std::endl;
    return 0;
}
#endif 