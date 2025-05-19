#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

vector<string> permutations; // Вектор для хранения всех перестановок

// Проверка наличия строки в векторе
bool isAlreadyAdded(const string& str) {
    return find(permutations.begin(), permutations.end(), str) != permutations.end();
}

// Рекурсия без перестановок
void generatePermutations(string str, int start) {
    if (start == str.size()) {
        if (!isAlreadyAdded(str)) {
            permutations.push_back(str);
        }
        return;
    }

    for (int i = start; i < str.size(); ++i) {
        // Скип повторов
        if (i != start && str[i] == str[start]) continue;
        swap(str[start], str[i]);
        generatePermutations(str, start + 1);
        swap(str[start], str[i]);
    }
}

// Функция вывода 
void printUniquePermutations(const string& word) {
    permutations.clear();
    string sortedWord = word;
    sort(sortedWord.begin(), sortedWord.end()); // Сортировка 
    generatePermutations(sortedWord, 0);

    cout << "Unique combinations for \"" << word << "\" (" << permutations.size() << "):\n";
    for (const auto& p : permutations) {
        cout << p << endl;
    }
    cout << endl;
}

int main() {
    printUniquePermutations("dvornik");
    printUniquePermutations("kosmos");
    return 0;
}