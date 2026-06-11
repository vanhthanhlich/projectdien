# IRS Optimization Project

This project implements and compares two optimization algorithms, **Particle Swarm Optimization (PSO)** and **Aquila Optimizer (AO)** (referred to as Alternating Optimization in some parts of the code), for Intelligent Reflecting Surface (IRS) assisted communication systems. The goal is to maximize the achievable rate by optimizing the phase shifts of the IRS elements.

## Features

- **PSO Implementation:** A robust Particle Swarm Optimization algorithm tailored for IRS phase shift optimization.
- **AO Implementation:** An implementation of the Aquila Optimizer for comparison.
- **Practical Phase Shift Model:** Incorporates a realistic hardware model for IRS elements using `IRSParameters`.
- **Convergence Tracking:** Both algorithms export convergence data to CSV files for performance analysis.
- **Visualization:** A Python script is provided to generate convergence plots comparing PSO and AO.
- **Eigen Integration:** Utilizes the Eigen C++ library for efficient matrix and vector operations.

## Project Structure

- `src/`: C++ source files (`main.cpp`, `particle.cpp`, `pso_swarm.cpp`, `ao_optimizer.cpp`).
- `include/`: Header files defining the algorithms and data structures.
- `python/`: Python scripts for data visualization.
- `python/results/`: Directory where simulation results and plots are saved.

## Prerequisites

### C++ Requirements
- CMake (>= 3.10)
- C++17 compliant compiler
- [Eigen3](https://eigen.tuxfamily.org/) library

### Python Requirements
- Python 3.x
- pandas
- matplotlib

## Building and Running

### 1. Build the C++ Project
```bash
mkdir build
cd build
cmake ..
make
```

### 2. Run the Optimization
```bash
./irs_pso
```
This will run both PSO and AO optimizations and save the results to `python/results/pso_convergence.csv` and `python/results/ao_convergence.csv`.

### 3. Generate Convergence Plots
```bash
python3 python/plot.py
```
The comparison plot `pso_vs_ao_comparison.png` will be generated in the `python/results/` directory.

## Implementation Details

The project simulates an IRS-assisted system with `numElements` (default: 40). It uses a practical phase shift model defined by:
- `beta_min`: Minimum amplitude coefficient.
- `k`: Phase shift parameter.
- `phi_param`: Phase shift offset.

The achievable rate is calculated based on simulated channel matrices (`Phi`) and direct link vectors (`hd`).
