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
    void calculateAccelerationDeviation();
    double getAccel() const;
    double getAccelDeviation() const;

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
    double g = 0;
    double deltaG = 0;
};


int main() 
{
    std::ofstream outputFile("means.txt");
    std::ofstream another_output("deviations.txt");

    DataProcessor Column1 ("Al.txt");
    Column1.readData();
    Column1.calculateMean();
    Column1.calculateDeviations();
    Column1.calculateStandardDeviation();
    outputFile << Column1.getMean() << "\n";
    another_output << Column1.getStandardDeviation() << "\n";
    Column1.calculateAccelerationDeviation();
    another_output << Column1.getAccel() << " " << Column1.getAccelDeviation() << "\n \n";
    Column1.~DataProcessor();

    DataProcessor Column2("Br.txt");
    Column2.readData();
    Column2.calculateMean();
    Column2.calculateDeviations();
    Column2.calculateStandardDeviation();
    outputFile << Column2.getMean() << "\n";
    another_output << Column2.getStandardDeviation() << "\n";
    Column2.calculateAccelerationDeviation();
    another_output << Column2.getAccel() << " " << Column2.getAccelDeviation() << "\n \n";
    Column2.~DataProcessor();

    DataProcessor Column3("St.txt");
    Column3.readData();
    Column3.calculateMean();
    Column3.calculateDeviations();
    Column3.calculateStandardDeviation();
    outputFile << Column3.getMean() << "\n";
    another_output << Column3.getStandardDeviation() << "\n";
    Column3.calculateAccelerationDeviation();
    another_output << Column3.getAccel() << " " << Column3.getAccelDeviation() << "\n \n";
    Column3.~DataProcessor();

    DataProcessor Column4("Wd.txt");
    Column4.readData();
    Column4.calculateMean();
    Column4.calculateDeviations();
    Column4.calculateStandardDeviation();
    outputFile << Column4.getMean() << "\n";
    another_output << Column4.getStandardDeviation() << "\n";
    Column4.calculateAccelerationDeviation();
    another_output << Column4.getAccel() << " " << Column4.getAccelDeviation() << "\n \n";
    Column4.~DataProcessor();

    DataProcessor Column5("Pl.txt");
    Column5.readData();
    Column5.calculateMean();
    Column5.calculateDeviations();
    Column5.calculateStandardDeviation();
    outputFile << Column5.getMean() << "\n";
    another_output << Column5.getStandardDeviation() << "\n";
    Column5.calculateAccelerationDeviation();
    another_output << Column5.getAccel() << " " << Column5.getAccelDeviation() << "\n \n";
    Column5.~DataProcessor();

    DataProcessor Column6("Pb.txt");
    Column6.readData();
    Column6.calculateMean();
    Column6.calculateDeviations();
    Column6.calculateStandardDeviation();
    outputFile << Column6.getMean() << "\n";
    another_output << Column6.getStandardDeviation() << "\n";
    Column6.calculateAccelerationDeviation();
    another_output << Column6.getAccel() << " " << Column6.getAccelDeviation() << "\n \n";
    Column6.~DataProcessor();
    
    outputFile.close();
    another_output.close();
}
#endif