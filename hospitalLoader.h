#pragma once

#include "hospital.h"
#include "ward.h"
#include "treatmentRoom.h"
#include "generalRoom.h"

#include <iostream>
#include <fstream>
#include </home/andrzej/proi/projects/hospital/proi_23l_201_szpital/vcpkg/installed/x64-linux/include/json/json.h>

class HospitalLoader {
    private:
        std::string jsonFile;
        void loadJsonFile(Json::Value& val);
    public:
        HospitalLoader(const std::string& jsonFile);
        Hospital loadHospital();
};
