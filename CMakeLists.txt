cmake_minimum_required(VERSION 3.10)
project(IRS_Optimization)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Eigen3
find_package(Eigen3 REQUIRED NO_MODULE)

# Include directories
include_directories(include)
include_directories(${EIGEN3_INCLUDE_DIR})

# Source files
set(SOURCES
    src/main.cpp
    src/particle.cpp
    src/pso_swarm.cpp
    src/ao_optimizer.cpp
)

# Executable
add_executable(irs_pso ${SOURCES})

# Link libraries
target_link_libraries(irs_pso Eigen3::Eigen)
