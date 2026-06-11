#include "particle.h"
#include <algorithm>
#include <numeric>

Particle::Particle(int numElements, std::mt19937& gen) {
    position.resize(numElements);
    velocity.resize(numElements);
    pBestPosition.resize(numElements);
    reinitialize(gen);
}

void Particle::reinitialize(std::mt19937& gen) {
    std::uniform_int_distribution<> dist(0, 1);
    std::uniform_real_distribution<double> distVel(-0.1 * PI, 0.1 * PI);

    for (int i = 0; i < position.size(); ++i) {
        position(i) = (dist(gen) == 0) ? PI : -PI;
        velocity(i) = distVel(gen);
    }
    
    pBestPosition = position;
    currentFitness = 0.0;
    pBestFitness = -1.0; 
}

void Particle::directedMutate(const Eigen::MatrixXcd& Psi, const Eigen::VectorXcd& hat_hd, const IRSParameters& params, std::mt19937& gen) {
    int N = position.size();
    Eigen::VectorXcd v(N);
    
    for (int i = 0; i < N; ++i) {
        double theta = position(i);
        double amplitude = (1.0 - params.beta_min) * std::pow((std::sin(theta - params.phi_param) + 1.0) / 2.0, params.k) + params.beta_min;
        v(i) = std::polar(amplitude, theta);
    }

    int numMutations = N / 2; 
    std::vector<int> indices(N);
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), gen);

    for (int k = 0; k < numMutations; ++k) {
        int n = indices[k];
        
        std::complex<double> phi_n = hat_hd(n);
        for (int m = 0; m < N; ++m) {
            if (m != n) phi_n += Psi(n, m) * v(m);
        }

        double arg_phi = std::arg(phi_n);
        double lambda = (arg_phi >= 0.0) ? 0.0 : 1.0;
        double theta_C = std::pow(-1.0, lambda) * PI;

        double min_bound = std::min(arg_phi, theta_C);
        double max_bound = std::max(arg_phi, theta_C);

        std::uniform_real_distribution<double> distTR(min_bound, max_bound);
        position(n) = distTR(gen);
        
        std::uniform_real_distribution<double> distVel(-0.05 * PI, 0.05 * PI);
        velocity(n) = distVel(gen);
        
        double new_amp = (1.0 - params.beta_min) * std::pow((std::sin(position(n) - params.phi_param) + 1.0) / 2.0, params.k) + params.beta_min;
        v(n) = std::polar(new_amp, position(n));
    }
    
    pBestPosition = position;
    pBestFitness = -1.0; 
}

void Particle::updateVelocity(const Eigen::VectorXd& gBestPosition, double w, double c1, double c2, std::mt19937& gen) {
    std::uniform_real_distribution<double> randDist(0.0, 1.0);
    int N = position.size();
    
    for (int i = 0; i < N; ++i) {
        double r1 = randDist(gen);
        double r2 = randDist(gen);
        
        velocity(i) = w * velocity(i) + 
                      c1 * r1 * (pBestPosition(i) - position(i)) + 
                      c2 * r2 * (gBestPosition(i) - position(i));
    }
}

void Particle::updatePosition() {
    position += velocity;
    
    for (int i = 0; i < position.size(); ++i) {
        while (position(i) > PI) position(i) -= 2.0 * PI;
        while (position(i) < -PI) position(i) += 2.0 * PI;
    }
}

void Particle::evaluateFitness(const Eigen::MatrixXcd& Phi, const Eigen::VectorXcd& hd, const IRSParameters& params) {
    int N = position.size();
    Eigen::VectorXcd v(N);
    
    for (int i = 0; i < N; ++i) {
        double theta = position(i);
        double amplitude = (1.0 - params.beta_min) * std::pow((std::sin(theta - params.phi_param) + 1.0) / 2.0, params.k) + params.beta_min;
        v(i) = std::polar(amplitude, theta);
    }
    
    Eigen::RowVectorXcd v_Hermitian = v.adjoint();
    Eigen::RowVectorXcd hd_Hermitian = hd.adjoint();
    
    Eigen::RowVectorXcd combinedChannel = v_Hermitian * Phi + hd_Hermitian;
    currentFitness = combinedChannel.squaredNorm();
    
    if (currentFitness > pBestFitness) {
        pBestFitness = currentFitness;
        pBestPosition = position;
    }
}
