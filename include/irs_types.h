#ifndef IRS_TYPES_H
#define IRS_TYPES_H

#include <cmath>
#include <eigen3/Eigen/Dense>

const double PI = std::acos(-1.0);

// Cấu trúc mô phỏng tham số phần cứng IRS (Practical Phase Shift Model)
struct IRSParameters {
    double beta_min;
    double k;
    double phi_param;
};

#endif // IRS_TYPES_H
