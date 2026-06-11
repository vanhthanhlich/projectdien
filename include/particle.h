#ifndef PARTICLE_H
#define PARTICLE_H

#include "irs_types.h"
#include <random>

class Particle {
public:
    Eigen::VectorXd position;      // Vị trí (các góc pha theta)
    Eigen::VectorXd velocity;      // Vận tốc thay đổi pha
    Eigen::VectorXd pBestPosition; // Vị trí tốt nhất cá nhân
    
    double currentFitness;
    double pBestFitness;

    Particle(int numElements, std::mt19937& gen);

    void reinitialize(std::mt19937& gen);
    void directedMutate(const Eigen::MatrixXcd& Psi, const Eigen::VectorXcd& hat_hd, const IRSParameters& params, std::mt19937& gen);
    void updateVelocity(const Eigen::VectorXd& gBestPosition, double w, double c1, double c2, std::mt19937& gen);
    void updatePosition();
    void evaluateFitness(const Eigen::MatrixXcd& Phi, const Eigen::VectorXcd& hd, const IRSParameters& params);
};

#endif // PARTICLE_H
