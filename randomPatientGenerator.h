#pragma once
#include <string>
#include <vector>
#include <memory>
#include "patient.h"
#include "randomNumberGenerator.h"

class RandomPatientGenerator{
    private:
        RandomNumberGenerator generator;
        std::vector<std::string> alreadyUsedPESELS;
        std::vector<std::string> maleNames;
        std::vector<std::string> femaleNames;
        std::vector<std::string> surnames;
    public:
        RandomPatientGenerator();
        std::unique_ptr<Patient> generatePatient();
        bool isPESELUsed(std::string PESEL);
        std::string generatePESEL();
        std::string generateName(Sex sex);
        std::string generateSurname(Sex sex);
        Sex generateSex();
        ushort generateAge();
};