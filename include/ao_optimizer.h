#ifndef AO_OPTIMIZER_H
#define AO_OPTIMIZER_H

#include "irs_types.h"
#include <random>
#include <string>

class AO_Optimizer {
private:
    Eigen::VectorXd position; 
    double currentFitness;
    int numElements;

public:
    AO_Optimizer(int num_elements, std::mt19937& gen);

    void optimize(int maxIter, const Eigen::MatrixXcd& Phi, const Eigen::VectorXcd& hd, 
                  const IRSParameters& params, const std::string& csv_filename);
};

#endif // AO_OPTIMIZER_H
