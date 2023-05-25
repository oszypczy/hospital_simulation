#pragma once

#include "hospital.h"
#include "ward.h"
#include "treatmentRoom.h"
#include "generalRoom.h"

#include <iostream>
#include <fstream>
#include <json/json.h>

class HospitalLoader {
    private:
        std::string jsonFile;
        void loadJsonFile(Json::Value& val);
    public:
        HospitalLoader(const std::string& jsonFile);
        Hospital loadHospital();
};
