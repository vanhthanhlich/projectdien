#ifndef PSO_SWARM_H
#define PSO_SWARM_H

#include "particle.h"
#include <vector>
#include <string>

class PSOSwarm {
private:
    std::vector<Particle> particles;
    Eigen::VectorXd gBestPosition;  
    double gBestFitness;            
    int numParticles;
    int numElements;

public:
    PSOSwarm(int num_particles, int num_elements, std::mt19937& gen);

    void optimize(int maxIter, double w_min, double w_max, double c1, double c2, 
                  const Eigen::MatrixXcd& Phi, const Eigen::VectorXcd& hd, 
                  const IRSParameters& params, std::mt19937& gen, 
                  const std::string& csv_filename);
                  
    Eigen::VectorXd getBestPhaseShift() const;
};

#endif // PSO_SWARM_H
