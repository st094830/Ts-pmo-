#include "Header.h"
#include <cmath>
#include <fstream>
#include <stdexcept>

DataProcessor::DataProcessor(const std::string& inputFileName) : inputFileName(inputFileName) {}

void DataProcessor::readData() {
    data.clear();
    std::ifstream file(inputFileName);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + inputFileName);
    }

    double value;
    while (file >> value) {
        data.push_back(value);
    }

    if (data.empty()) {
        throw std::runtime_error("No data read from file: " + inputFileName);
    }
}

void DataProcessor::setData(const std::vector<double>& newData) {
    if (newData.empty()) {
        throw std::runtime_error("Input data is empty");
    }
    data = newData;
}

void DataProcessor::calculateMean() {
    if (data.empty()) {
        throw std::runtime_error("No data for mean calculation");
    }

    mean = 0.0;
    for (const auto& val : data) {
        mean += val;
    }
    mean /= static_cast<double>(data.size());
}

void DataProcessor::calculateDeviations() {
    if (data.empty()) {
        throw std::runtime_error("No data for deviation calculation");
    }

    deviations.clear();
    squaredDeviations.clear();

    for (const auto& val : data) {
        double dev = val - mean;
        deviations.push_back(dev);
        squaredDeviations.push_back(dev * dev);
    }
}

void DataProcessor::calculateStandardDeviation() {
    if (deviations.empty()) {
        calculateDeviations();
    }

    if (data.size() < 2) {
        throw std::runtime_error("Not enough data for standard deviation");
    }

    double sum = 0.0;
    for (const auto& sqDev : squaredDeviations) {
        sum += sqDev;
    }
    standardDeviation = sqrt(sum / static_cast<double>(data.size() - 1));
}

void DataProcessor::calculateMeanError() {
    if (standardDeviation == 0.0) {
        calculateStandardDeviation();
    }

    meanError = standardDeviation / sqrt(static_cast<double>(data.size()));
}

void DataProcessor::processPairPoints(const std::vector<double>& x, const std::vector<double>& y) {
    if (x.size() != y.size()) {
        throw std::runtime_error("x and y must have the same size");
    }

    if (x.size() % 2 != 0) {
        throw std::runtime_error("Data size must be even for pair points method");
    }

    if (x.size() < 4) {
        throw std::runtime_error("Not enough data points for pair points method");
    }

    std::vector<double> slopes;
    std::vector<double> intercepts;

    size_t halfSize = x.size() / 2;
    for (size_t i = 0; i < halfSize; ++i) {
        size_t j = i + halfSize;
        double delta_x = x[j] - x[i];
        double delta_y = y[j] - y[i];

        if (std::abs(delta_x) < 1e-10) {
            throw std::runtime_error("Zero delta_x in pair points method");
        }

        double a = delta_y / delta_x;
        double b = y[i] - a * x[i];

        slopes.push_back(a);
        intercepts.push_back(b);
    }

    a_mean = 0.0;
    b_mean = 0.0;
    for (size_t i = 0; i < slopes.size(); ++i) {
        a_mean += slopes[i];
        b_mean += intercepts[i];
    }
    a_mean /= static_cast<double>(slopes.size());
    b_mean /= static_cast<double>(intercepts.size());

    double a_dev_sum = 0.0;
    double b_dev_sum = 0.0;
    for (size_t i = 0; i < slopes.size(); ++i) {
        a_dev_sum += (slopes[i] - a_mean) * (slopes[i] - a_mean);
        b_dev_sum += (intercepts[i] - b_mean) * (intercepts[i] - b_mean);
    }

    a_stddev = sqrt(a_dev_sum / static_cast<double>(slopes.size() - 1));
    b_stddev = sqrt(b_dev_sum / static_cast<double>(intercepts.size() - 1));

    a_stderr = a_stddev / sqrt(static_cast<double>(slopes.size()));
    b_stderr = b_stddev / sqrt(static_cast<double>(intercepts.size()));

    double student_coeff = 2.365;
    a_confidence = a_stderr * student_coeff;
    b_confidence = b_stderr * student_coeff;
}

double DataProcessor::getMean() const { return mean; }
double DataProcessor::getStandardDeviation() const { return standardDeviation; }
double DataProcessor::getMeanError() const { return meanError; }
const std::vector<double>& DataProcessor::getData() const { return data; }
const std::vector<double>& DataProcessor::getDeviations() const { return deviations; }
const std::vector<double>& DataProcessor::getSquaredDeviations() const { return squaredDeviations; }
double DataProcessor::getSlope() const { return a_mean; }
double DataProcessor::getIntercept() const { return b_mean; }
double DataProcessor::getSlopeStdDev() const { return a_stddev; }
double DataProcessor::getInterceptStdDev() const { return b_stddev; }
double DataProcessor::getSlopeStdErr() const { return a_stderr; }
double DataProcessor::getInterceptStdErr() const { return b_stderr; }
double DataProcessor::getSlopeConfidence() const { return a_confidence; }
double DataProcessor::getInterceptConfidence() const { return b_confidence; }