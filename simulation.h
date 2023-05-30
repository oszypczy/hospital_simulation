#pragma once
#include "hospital.h"
#include "randomNumberGenerator.h"
#include "randomPatientGenerator.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

class Simulation {
    private:
        std::unique_ptr<Hospital> hospital;
        unsigned int iterations;
        std::vector<std::string> messages;
        ushort waitTime;
        std::string outputFileName;
        std::chrono::system_clock::time_point dateTime;
        RandomNumberGenerator randomNumberGenerator;
        RandomPatientGenerator randomPatientGenerator;
    public:
        Simulation(std::unique_ptr<Hospital> hospital, unsigned int iterations, ushort waitTime, std::string outputFileName);
        void run();
        void incrementDateTime();
        std::string getDateTime();
        void printMessages();
        void writeMessagesToFile();
        void newPatientInReception();
        void patientCalled911();
        void checkForReturnedAmbulances();
        void goThroughGeneralRooms();
        void conductConsultations();
        void moveToGeneralRoom();
        void conductTreatments();
};