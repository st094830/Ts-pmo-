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
    standDev = 0;
    standErr = 0;
    deltaTg = 0;

    for (double val : data) {
        double deviation = val - mean;
        deviations.push_back(deviation);
        squaredDeviations.push_back(deviation * deviation);
        standDev += deviation * deviation;
    }
    standDev = sqrt(standDev / (data.size() - 1));
    standErr = standDev / sqrt(data.size());
    deltaTg = 2.365 * standErr;
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

std::vector<double> DataProcessor::getCurrency()
{
    currency.clear();
    for (int i = 0; i < data.size(); i++) 
    {
        currency.push_back(data[i] / 12);
    }
    return currency;
}

std::vector<double> DataProcessor::getLogarithm()
{
    currencyLogarithm.clear();
    for (int i = 0; i < data.size(); i++) 
    {
        currencyLogarithm.push_back(log(data[i]));
    }
    return currencyLogarithm;
}

std::vector<double> DataProcessor::getData()
{
    return data;
}

double DataProcessor::getSDev() const
{
    return standDev;
}

double DataProcessor::getSErr() const
{
    return standErr;
}

double DataProcessor::getDelta() const
{
    return deltaTg;
}
