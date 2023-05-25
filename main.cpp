#include <iostream>
#include <fstream>
#include <string>

#include "hospitalLoader.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Invalid number of arguments" << std::endl;
        return 1;
    }

    std::string jsonFile = argv[1];
    HospitalLoader hospitalLoader(jsonFile);
    Hospital hospital = hospitalLoader.loadHospital();

    return 0;
}
