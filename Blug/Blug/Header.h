#pragma once
#ifndef DATAPROC_H
#define DATAPROC_H

#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <iostream>
#include <algorithm>

class DataProcessor {
public:
    DataProcessor(const std::string& inputFileName);
    ~DataProcessor() = default;

    void readData();
    void processData();
    void calculateStatistics();
    double getMean() const { return mean; }
    double getStandardDeviation() const { return standardDeviation; }
    double getMeanError() const { return meanError; }
    const std::vector<double>& getProcessedValues() const { return processedValues; }

private:
    std::string inputFileName;
    std::vector<double> data;
    std::vector<double> processedValues;
    double mean;
    double standardDeviation;
    double meanError;
};

// Реализация методов класса
DataProcessor::DataProcessor(const std::string& inputFileName)
    : inputFileName(inputFileName), mean(0.0), standardDeviation(0.0), meanError(0.0) {
}

void DataProcessor::readData() {
    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Cannot open file " + inputFileName);
    }

    data.clear();
    double value;
    while (inputFile >> value) {
        data.push_back(value);
    }
    inputFile.close();
}

void DataProcessor::processData() {
    // Убедимся, что у нас четное количество точек
    if (data.size() % 2 != 0) {
        data.pop_back(); // Или добавить обработку нечетного случая
    }

    processedValues.clear();

    // Вычисляем отношения для пар точек
    for (size_t i = 0; i < data.size() / 2; ++i) {
        double numerator = data[i + data.size() / 2] - data[i];
        double denominator = log(data[i + data.size() / 2] / 12.0) - log(data[i] / 12.0);

        if (denominator != 0) { // Избегаем деления на ноль
            processedValues.push_back(numerator / denominator);
        }
    }

    // Вычисляем статистики
    calculateStatistics();
}

void DataProcessor::calculateStatistics() {
    if (processedValues.empty()) return;

    // Среднее значение
    mean = 0.0;
    for (double val : processedValues) {
        mean += val;
    }
    mean /= processedValues.size();

    // Стандартное отклонение
    standardDeviation = 0.0;
    for (double val : processedValues) {
        standardDeviation += (val - mean) * (val - mean);
    }
    standardDeviation = sqrt(standardDeviation / (processedValues.size() - 1));

    // Ошибка среднего
    meanError = standardDeviation / sqrt(processedValues.size());
}


int main() {
    try {
        const double temperature = 295.0; // Температура в Кельвинах

        DataProcessor voltageData("TwoDigEB.txt");
        voltageData.readData();
        voltageData.processData();

        double meanSlope = voltageData.getMean();
        double error = voltageData.getMeanError();

        double e_kB_ratio = meanSlope * temperature;
        double e_kB_error = error * temperature;

        std::cout << "Calculated e/kB ratio: " << e_kB_ratio << " ± " << e_kB_error << " K/V\n";
        std::cout << "Theoretical value: 11604.5 K/V\n";
        std::cout << "Relative error: " << abs(e_kB_ratio - 11604.5) / 11604.5 * 100 << "%\n";

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

#endif
