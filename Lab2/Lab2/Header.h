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
    another_output << Column1.getStandardDeviation() << "\t";
    double g = 0;
    double t = 0;
    t = Column1.getMean()/1000;
    g = 2 * (0.272 - 1.050 * t) / (pow(t,2));
    double deltaG = 0;
    deltaG = sqrt(pow(2*0.001/pow(t,2),2)/9+pow((-4*0.272/pow(t, 3)+2*0.005/pow(t, 2))*Column1.getStandardDeviation()/1000, 2) + pow(-2*0.005/t, 2) / 9);
    another_output << g << " " << deltaG << "\n";


    DataProcessor Column2("Br.txt");
    Column2.calculateMean();
    Column2.calculateStandardDeviation();
    outputFile << Column2.getMean() << "\n";
    another_output << Column2.getStandardDeviation() << "\n";

    DataProcessor Column3("St.txt");
    Column3.calculateMean();
    Column3.calculateStandardDeviation();
    outputFile << Column3.getMean() << "\n";
    another_output << Column3.getStandardDeviation() << "\n";

    DataProcessor Column4("Wd.txt");
    Column4.calculateMean();
    Column4.calculateStandardDeviation();
    outputFile << Column4.getMean() << "\n";
    another_output << Column4.getStandardDeviation() << "\n";

    DataProcessor Column5("Pl.txt");
    Column5.calculateMean();
    Column5.calculateStandardDeviation();
    outputFile << Column5.getMean() << "\n";
    another_output << Column5.getStandardDeviation() << "\n";

    DataProcessor Column6("Pb.txt");
    Column6.calculateMean();
    Column6.calculateStandardDeviation();
    outputFile << Column6.getMean() << "\n";
    another_output << Column6.getStandardDeviation() << "\n";
    outputFile.close();
    another_output.close();
}
#endif