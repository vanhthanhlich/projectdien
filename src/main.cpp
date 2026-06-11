#include <iostream>
#include <random>
#include "pso_swarm.h"
#include "ao_optimizer.h"

int main() {
    // 1. Khởi tạo bộ sinh số ngẫu nhiên
    std::random_device rd;
    std::mt19937 gen(rd());

    // 2. Thiết lập cấu hình hệ thống
    int numElements = 40;     
    int maxIter = 500;        
    
    // Tham số PSO
    double w_max = 0.9, w_min = 0.4;           
    double c1 = 1.5;          
    double c2 = 1.5;          

    IRSParameters params = {0.2, 1.6, 0.43 * PI}; 

    // 3. Giả lập kênh truyền 
    Eigen::MatrixXcd Phi = Eigen::MatrixXcd::Random(numElements, 2); 
    Eigen::VectorXcd hd = Eigen::VectorXcd::Random(2);               

    // 4. Khởi tạo bầy đàn và chạy tối ưu
    int numParticles = 100;
    PSOSwarm pso_swarm(numParticles, numElements, gen);
    std::cout << "[1/2] Bắt đầu chạy PSO...\n";
    pso_swarm.optimize(maxIter, w_min, w_max, c1, c2, Phi, hd, params, gen, "../python/results/pso_convergence.csv");

    // Chạy AO
    AO_Optimizer ao_optimizer(numElements, gen);
    std::cout << "[2/2] Bắt đầu chạy AO...\n";
    ao_optimizer.optimize(maxIter, Phi, hd, params, "../python/results/ao_convergence.csv");

    return 0;
}
