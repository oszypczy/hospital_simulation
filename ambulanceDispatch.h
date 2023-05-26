#pragma once
#include "ambulance.h"
#include "patient.h"
#include "paramedic.h"
#include "place.h"
#include <vector>

class AmbulanceDispatch : public Place {
    private:
        std::vector<std::unique_ptr<Ambulance>> ambulances;
        std::vector<std::unique_ptr<Paramedic>> paramedics;
        std::unique_ptr<Patient> patient;
    public:
        AmbulanceDispatch(std::string id);
        void addAmbulance(std::unique_ptr<Ambulance> ambulance);
        void addParamedic(std::unique_ptr<Paramedic> paramedic);
        std::vector<std::unique_ptr<Ambulance>>::iterator getAmbulance(std::string registrationNumber);
        std::vector<std::unique_ptr<Ambulance>>::iterator getFreeAmbulance();
        std::vector<std::unique_ptr<Paramedic>>::iterator getFreeParamedic();
        std::string sendAmbulance(std::unique_ptr<Patient> patient);
        void constinueAllInterventions();
        std::string checkIfFinishedIntervention();
        std::unique_ptr<Patient> getPatient();
};

