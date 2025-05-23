#ifndef DATAPROC_H
#define DATAPROC_H

#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>

class DataProcessor {
public:
    explicit DataProcessor(const std::string& inputFileName = "");
    void readData();
    void setData(const std::vector<double>& newData);
    void calculateMean();
    void calculateDeviations();
    void calculateStandardDeviation();
    void calculateMeanError();
    void processPairPoints(const std::vector<double>& x, const std::vector<double>& y);

    double getMean() const;
    double getStandardDeviation() const;
    double getMeanError() const;
    const std::vector<double>& getData() const;
    const std::vector<double>& getDeviations() const;
    const std::vector<double>& getSquaredDeviations() const;
    double getSlope() const;
    double getIntercept() const;
    double getSlopeStdDev() const;
    double getInterceptStdDev() const;
    double getSlopeStdErr() const;
    double getInterceptStdErr() const;
    double getSlopeConfidence() const;
    double getInterceptConfidence() const;

private:
    std::string inputFileName;
    std::vector<double> data;

    double mean = 0.0;
    std::vector<double> deviations;
    std::vector<double> squaredDeviations;
    double standardDeviation = 0.0;
    double meanError = 0.0;

    double a_mean = 0.0;
    double b_mean = 0.0;
    double a_stddev = 0.0;
    double b_stddev = 0.0;
    double a_stderr = 0.0;
    double b_stderr = 0.0;
    double a_confidence = 0.0;
    double b_confidence = 0.0;
};



int main() {
    try {
        std::cout.precision(6);
        std::cout << std::fixed;

       


        DataProcessor ebVoltageData("TwoDigEB.txt");
        DataProcessor cbVoltageData("TwoDigCB.txt");

        ebVoltageData.readData();
        cbVoltageData.readData();






        std::vector<double> currents;
        const double R3 = 12.0;
        for (const auto& u_kb : cbVoltageData.getData()) {
            if (R3 < 1e-10) {  
                throw std::runtime_error("R3 resistance is too small");
            }
            currents.push_back(u_kb / R3);
        }

      



        std::vector<double> logCurrents;
        for (const auto& I_k : currents) {
            if (I_k <= 0) {  
                throw std::runtime_error("Current value must be positive for logarithm");
            }
            logCurrents.push_back(log(I_k));
        }




        DataProcessor processor;
        processor.processPairPoints(ebVoltageData.getData(), logCurrents);




        double temperature = 295.15;
        double e_over_k = temperature * processor.getSlope();
        double e_over_k_error = temperature * processor.getSlopeConfidence();



        double I0 = exp(processor.getIntercept());
        double I0_error = I0 * processor.getInterceptConfidence();



        std::ofstream Autput("tgs.txt");
        Autput << "Linear regression parameters ln(I_k) = a·U_eb + b:\n";
        Autput << "  Slope (a): " << processor.getSlope()
            << " ± " << processor.getSlopeConfidence() << " 1/V\n";
        Autput << "  Intercept (b): " << processor.getIntercept()
            << " ± " << processor.getInterceptConfidence() << "\n";
        Autput << "  Temperature (T): " << temperature << " K\n";
        Autput << "\ne/k RATIO:\n";
        Autput << "  e/k = T·a = " << e_over_k
            << " ± " << e_over_k_error << " K/V\n";
        Autput << "\nSATURATION CURRENT:\n";
        Autput << "  I0 = exp(b) = " << I0
            << " ± " << I0_error << " A\n";
        Autput << "------------------------------------------------\n";
        Autput.close();



        std::cout << "Saving graph data to graph_data.txt...\n";
        std::ofstream graphData("graph_data.txt");
        if (!graphData) {
            throw std::runtime_error("Cannot open graph_data.txt for writing");
        }
        graphData << "# U_eb[V]\tln(I_k)\tI_k[A]\n";
        for (size_t i = 0; i < ebVoltageData.getData().size(); ++i) {
            graphData << ebVoltageData.getData()[i] << "\t"
                << logCurrents[i] << "\t"
                << currents[i] << "\n";
        }
        graphData.close();



        std::ofstream params("fit_params.txt");
        if (!params) {
            throw std::runtime_error("Cannot open fit_params.txt for writing");
        }
        params << "a " << processor.getSlope() << " " << processor.getSlopeConfidence() << "\n";
        params << "b " << processor.getIntercept() << " " << processor.getInterceptConfidence() << "\n";
        params << "I0 " << I0 << " " << I0_error << "\n";
        params << "e_over_k " << e_over_k << " " << e_over_k_error << "\n";
        params.close();

        std::cout << "Data processing completed successfully!\n";
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
#endif