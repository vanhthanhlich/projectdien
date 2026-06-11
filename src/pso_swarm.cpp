#include "pso_swarm.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>

PSOSwarm::PSOSwarm(int num_particles, int num_elements, std::mt19937& gen) 
    : numParticles(num_particles), numElements(num_elements), gBestFitness(-1.0) {
    
    gBestPosition.resize(numElements);
    for (int i = 0; i < numParticles; ++i) {
        particles.emplace_back(numElements, gen);
    }
}

void PSOSwarm::optimize(int maxIter, double w_min, double w_max, double c1, double c2, 
                        const Eigen::MatrixXcd& Phi, const Eigen::VectorXcd& hd, 
                        const IRSParameters& params, std::mt19937& gen, 
                        const std::string& csv_filename) {
    
    std::ofstream csv_file(csv_filename);
    if (!csv_file.is_open()) return;
    csv_file << "Iteration,BestFitness\n";

    Eigen::MatrixXcd Psi = Phi * Phi.adjoint();
    Eigen::VectorXcd hat_hd = Phi * hd;

    for (auto& p : particles) {
        p.evaluateFitness(Phi, hd, params);
        if (p.currentFitness > gBestFitness) {
            gBestFitness = p.currentFitness;
            gBestPosition = p.pBestPosition;
        }
    }

    int stagnationCounter = 0;
    const int STAGNATION_THRESHOLD = 30; 
    const int numReinit = static_cast<int>(numParticles * 0.3); 

    for (int t = 1; t <= maxIter; ++t) {
        double w = w_max - ((w_max - w_min) / maxIter) * t;
        bool improvedInThisIter = false; 

        for (auto& p : particles) {
            p.updateVelocity(gBestPosition, w, c1, c2, gen);
            p.updatePosition();
            p.evaluateFitness(Phi, hd, params);

            if (p.currentFitness > gBestFitness) {
                if (p.currentFitness - gBestFitness > 1e-6) {
                    improvedInThisIter = true; 
                }
                gBestFitness = p.currentFitness;
                gBestPosition = p.pBestPosition;
            }
        }
        
        if (improvedInThisIter) {
            stagnationCounter = 0; 
        } else {
            stagnationCounter++;
        }

        if (stagnationCounter >= STAGNATION_THRESHOLD) {
            std::cout << "[INFO] Iter " << t << ": Phat hien kẹt local optima. Kich hoat Trust Region Mutation!\n";
            
            std::vector<int> indices(numParticles);
            std::iota(indices.begin(), indices.end(), 0);
            std::shuffle(indices.begin(), indices.end(), gen);

            for (int i = 0; i < numReinit; ++i) {
                Particle& p_to_reset = particles[indices[i]];
                p_to_reset.directedMutate(Psi, hat_hd, params, gen);
                p_to_reset.evaluateFitness(Phi, hd, params);
                
                if (p_to_reset.currentFitness > gBestFitness) {
                    gBestFitness = p_to_reset.currentFitness;
                    gBestPosition = p_to_reset.pBestPosition;
                }
            }
            stagnationCounter = 0; 
        }
        
        csv_file << t << "," << gBestFitness << "\n";
    }
    
    csv_file.close();
    std::cout << "Hoàn tất PSO. Dữ liệu: " << csv_filename << "\n";
}

Eigen::VectorXd PSOSwarm::getBestPhaseShift() const {
    return gBestPosition;
}
