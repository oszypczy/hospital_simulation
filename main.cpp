#include <iostream>
#include <fstream>
#include <string>
#include </home/andrzej/proi/projects/hospital/proi_23l_201_szpital/vcpkg/installed/x64-linux/include/json/json.h>
// #include </vcpkg/installed/x64-linux/include/json/json.h>

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
