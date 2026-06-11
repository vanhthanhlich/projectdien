#include "ao_optimizer.h"
#include <iostream>
#include <fstream>

AO_Optimizer::AO_Optimizer(int num_elements, std::mt19937& gen) : numElements(num_elements) {
    position.resize(numElements);
    
    std::uniform_int_distribution<> dist(0, 1);
    for (int i = 0; i < numElements; ++i) {
        position(i) = (dist(gen) == 0) ? PI : -PI;
    }
}

void AO_Optimizer::optimize(int maxIter, const Eigen::MatrixXcd& Phi, const Eigen::VectorXcd& hd, 
                            const IRSParameters& params, const std::string& csv_filename) {
    
    std::ofstream csv_file(csv_filename);
    if (!csv_file.is_open()) return;
    csv_file << "Iteration,BestFitness\n";

    Eigen::MatrixXcd Psi = Phi * Phi.adjoint();
    Eigen::VectorXcd hat_hd = Phi * hd;
    
    Eigen::VectorXcd v(numElements);
    for (int n = 0; n < numElements; ++n) {
        double theta = position(n);
        double beta = (1.0 - params.beta_min) * std::pow((std::sin(theta - params.phi_param) + 1.0) / 2.0, params.k) + params.beta_min;
        v(n) = std::polar(beta, theta);
    }

    currentFitness = (v.adjoint() * Phi + hd.adjoint()).squaredNorm();
    csv_file << 0 << "," << currentFitness << "\n";

    for (int t = 1; t <= maxIter; ++t) {
        for (int n = 0; n < numElements; ++n) {
            std::complex<double> phi_n = hat_hd(n);
            for (int m = 0; m < numElements; ++m) {
                if (m != n) phi_n += Psi(n, m) * v(m);
            }

            double arg_phi = std::arg(phi_n);
            double abs_phi = std::abs(phi_n);
            double lambda = (arg_phi >= 0.0) ? 0.0 : 1.0;

            double theta_A = arg_phi;
            double theta_B = arg_phi + std::pow(-1.0, lambda) * PI / 2.0;
            double theta_C = std::pow(-1.0, lambda) * PI;

            auto eval_f = [&](double theta) {
                double beta = (1.0 - params.beta_min) * std::pow((std::sin(theta - params.phi_param) + 1.0) / 2.0, params.k) + params.beta_min;
                return beta * beta * Psi(n, n).real() + 2.0 * beta * abs_phi * std::cos(arg_phi - theta);
            };

            double f1 = eval_f(theta_A);
            double f2 = eval_f(theta_B);
            double f3 = eval_f(theta_C);

            double denom = 4.0 * (f1 - 2.0 * f2 + f3);
            double theta_star;
            
            if (std::abs(denom) < 1e-9) { 
                theta_star = theta_A;
                if (f2 > f1 && f2 > f3) theta_star = theta_B;
                if (f3 > f1 && f3 > f2) theta_star = theta_C;
            } else {
                theta_star = (theta_C * (3.0 * f1 - 4.0 * f2 + f3) + theta_A * (f1 - 4.0 * f2 + 3.0 * f3)) / denom;
            }

            while (theta_star > PI) theta_star -= 2.0 * PI;
            while (theta_star < -PI) theta_star += 2.0 * PI;

            position(n) = theta_star;
            double beta_star = (1.0 - params.beta_min) * std::pow((std::sin(theta_star - params.phi_param) + 1.0) / 2.0, params.k) + params.beta_min;
            v(n) = std::polar(beta_star, theta_star);
        }

        currentFitness = (v.adjoint() * Phi + hd.adjoint()).squaredNorm();
        csv_file << t << "," << currentFitness << "\n";
    }
    
    csv_file.close();
    std::cout << "Hoàn tất thuật toán AO. Dữ liệu: " << csv_filename << "\n";
}
