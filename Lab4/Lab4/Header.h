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
    std::vector<double> getCurrency();
    std::vector<double> getLogarithm();
    std::vector<double> getData();

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
    std::vector<double> currency;
    std::vector<double> currencyLogarithm;

};

int main(int argc, char* argv[])
{
    std::ofstream outputFile("CurrencyTwoDig.txt");    
    DataProcessor TwoDigCB("TwoDigCB.txt");
    TwoDigCB.readData();
    std::vector<double> Currencies = TwoDigCB.getCurrency();
    for (int i = 0; i < 16; i++)
    {
        outputFile << Currencies[i] << "\n";
    }
    outputFile.close();

    std::ofstream outputFile2("LogarithmTwoDig.txt");
    TwoDigCB.readData();
    TwoDigCB.calculateMean();
    double Meow = TwoDigCB.getMean();
    std::vector<double> Logarithms = TwoDigCB.getLogarithm();
    for (int i = 0; i < 16; i++)
    {
        outputFile2 << Logarithms[i] << "\n";
    }
    outputFile2 << Meow << "\n------";
    outputFile2.close();
    TwoDigCB.~TwoDigCB();

    DataProcessor TwoDigEB("TwoDigEB.txt");
    TwoDigEB.readData();
    TwoDigEB.calculateMean();
    Meow = TwoDigEB.getMean();
    std::vector<double> EBVoltage = TwoDigEB.getData();
    DataProcessor TwoDigLog("LogarithmTwoDig.txt");
    TwoDigLog.readData();
    std::vector<double> LnCurrency = TwoDigLog.getData();
    std::ofstream outputFile3("iterations.txt");
    double sum = 0;
    for (int i = 0; i < 8; i++) 
    {
        outputFile3 << (EBVoltage[i + 8] - EBVoltage[i]) / (LnCurrency[i + 8] - LnCurrency[i]) << "\n";
        sum += (EBVoltage[i + 8] - EBVoltage[i]) / (LnCurrency[i + 8] - LnCurrency[i]);
    }
    outputFile3 << sum << "\n---------\n" << Meow << "   srednee Ueb";
    outputFile3.close();
    TwoDigEB.~TwoDigEB();
    TwoDigLog.~TwoDigLog();

    std::ofstream outputFile4("iterations.txt");
    DataProcessor TgCalculation("iterations.txt");
    TgCalculation.readData();
    TgCalculation.calculateMean();
    TgCalculation.calculateDeviations();
    std::vector<double> Devs = TgCalculation.getDeviations();
    std::vector<double> SqrDevs = TgCalculation.getSquaredDeviations();
    sum = 0;
    for (int i = 0; i < 8; i++) 
    {
        outputFile4 << Devs[i] << "  " << SqrDevs[i] << "\n";
        sum += SqrDevs[i];
    }
    outputFile4 << sum;
    outputFile4.close();
    TgCalculation.~TgCalculation();
    return 0;
}
#endif