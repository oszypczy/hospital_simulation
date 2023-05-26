#include <iostream>
#include <fstream>
#include <string>
#include "simulation.h"
#include "hospitalLoader.h"

// arguments = json_file, output_file, iterations, simulation_wait_time (s)
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Invalid number of arguments" << std::endl;
        return 1;
    }
    std::string jsonFile = argv[1];
    HospitalLoader hospitalLoader(jsonFile);
    std::unique_ptr<Hospital> hospital = std::make_unique<Hospital>(hospitalLoader.loadHospital());
    Simulation simulation(std::move(hospital), std::stoi(argv[3]), std::stoi(argv[4]), argv[2]);
    simulation.run();
    return 0;
}
