cmake_minimum_required(VERSION 3.10)
project(EVTOL_Simulation VERSION 1.0)
set(CMAKE_CXX_STANDARD 14)  # Changed from 11 to 14
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_executable(evtol_sim src/evtol_sim.cpp)
target_compile_options(evtol_sim PRIVATE -Wall -Wextra)