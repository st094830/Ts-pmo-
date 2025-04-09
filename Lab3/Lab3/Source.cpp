#include "Header.h"


DataProcessor::DataProcessor(const std::string& inputFileName)
    : inputFileName(inputFileName), mean(0.0), maxDelta(0.0), standardDeviation(0.0), meanError(0.0) {
}

void DataProcessor::readData() {
    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Не удалось открыть файл " + inputFileName);
    }

    double value;
    while (inputFile >> value) {
        data.push_back(value);
    }
    inputFile.close();

    if (data.size() != 5) {
        throw std::runtime_error("Файл должен содержать ровно 5 значений");
    }
}

void DataProcessor::calculateMean() {
    double sum = 0.0;
    for (double val : data) {
        sum += val;
    }
    mean = sum / data.size();
}

void DataProcessor::calculateDeviations() {
    deviations.clear();
    squaredDeviations.clear();

    for (double val : data) {
        double deviation = val - mean;
        deviations.push_back(deviation);
        squaredDeviations.push_back(deviation * deviation);
    }
}

void DataProcessor::calculateAbsoluteErrors() {
    deltas.clear();

    for (double R : data) {
        double relativeError = 1 + R / 6.0;
        double absoluteError = relativeError / 100 * R;
        deltas.push_back(absoluteError);
    }

    maxDelta = *std::max_element(deltas.begin(), deltas.end());
}

void DataProcessor::calculateStandardDeviation() {
    double sumSquaredDeviations = 0.0;
    for (double squaredDev : squaredDeviations) {
        sumSquaredDeviations += squaredDev;
    }
    standardDeviation = std::sqrt(sumSquaredDeviations / data.size());
}


void DataProcessor::calculateMeanError() {
    meanError = standardDeviation / std::sqrt(data.size());
}


double DataProcessor::getMaxDelta() const {
    return maxDelta;
}

double DataProcessor::getStandardDeviation() const {
    return standardDeviation;
}

double DataProcessor::getMeanError() const {
    return meanError;
}

const std::vector<double>& DataProcessor::getDeviations() const {
    return deviations;
}

const std::vector<double>& DataProcessor::getSquaredDeviations() const {
    return squaredDeviations;
}

double DataProcessor::getMean() const {
    return mean;
}

std::vector<double> DataProcessor::getSensitivity() 
{
    for (int i = 1; i < 6; i++) 
    {
        sensitivity.push_back(10 * i / (2 * sqrt(2) * data[i - 1]));
    }
    return sensitivity;
}
