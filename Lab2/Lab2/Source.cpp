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

    if (data.size() != 30) {
        throw std::runtime_error("Файл должен содержать ровно 50 значений");
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

void DataProcessor::calculateAccelerationDeviation()
{
    double mean_ = mean / 1000;
    g = 2 * (0.272 - 1.050 * mean_) / (pow(mean_, 2));
    deltaG = sqrt(pow(2 * 0.001 / pow(mean_, 2), 2) / 9 + pow((-4 * 0.272 / pow(mean_, 3) + 2 * 0.005 / pow(mean_, 2)) * standardDeviation / 1000, 2) + pow(-2 * 0.005 / mean_, 2) / 9);
}

double DataProcessor::getAccel() const
{
    return g;
}

double DataProcessor::getAccelDeviation() const
{
    return deltaG;
}
