#pragma once
#pragma once
#ifndef DATAPROC_H
#define DATAPROC_H

#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <iostream>

class DataProcessor {
public:
    DataProcessor(const std::string& inputFileName);
    ~DataProcessor() {};
    void readData();
    void calculateMean();
    void calculateDeviations();
    void calculateAbsoluteErrors();
    void calculateStandardDeviation();
    void calculateMeanError();
    double getMaxDelta() const;
    double getStandardDeviation() const;
    double getMeanError() const;
    const std::vector<double>& getDeviations() const;
    const std::vector<double>& getSquaredDeviations() const;
    double getMean() const;
    std::vector<double> getSensitivity();

private:
    std::string inputFileName;
    std::vector<double> data;
    double mean;
    std::vector<double> deviations;
    std::vector<double> squaredDeviations;
    std::vector<double> deltas;
    double maxDelta;
    double standardDeviation;
    double meanError;
    std::vector<double> sensitivity;
};

int main(int argc, char* argv[]) 
{
    std::ofstream outputFile("Sensitivity.txt");

    DataProcessor Vert ("Vert.txt");
    DataProcessor Horus ("Horus.txt");
    DataProcessor Maxx ("Maxx.txt");
    Vert.readData();
    Horus.readData();
    Maxx.readData();
    std::vector<double> SenssV = Vert.getSensitivity();
    for (int i = 0; i < 5; i++) {
        outputFile << SenssV[i] << "\n";
    }
    outputFile << "\n ______ \n";
    std::vector<double> SenssH = Horus.getSensitivity();
    for (int i = 0; i < 5; i++) {
        outputFile << SenssH[i] << "\n";
    }
    outputFile << "\n ______ \n";
    std::vector<double> Senss = Maxx.getSensitivity();
    for (int i = 0; i < 5; i++) {
        outputFile << Senss[i] << "\n";
    }
    outputFile << "\n \n Max koefficient: \n \n";
    for (int i = 0; i < 5; i++) {
        outputFile << Senss[i]/SenssV[i] << "\n";
    }
}
#endif