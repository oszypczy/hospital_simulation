#include "ambulanceDispatch.h"
#include "objectAlreadyExistsException.h"
#include <algorithm>
#include "objectNotFoundException.h"
#include <sstream>
#include <iostream>

AmbulanceDispatch::AmbulanceDispatch(std::string id) : Place(id){};

std::vector<std::unique_ptr<Ambulance>>::iterator AmbulanceDispatch::getAmbulance(std::string registrationNumber){
    auto it = std::find_if(ambulances.begin(), ambulances.end(), [&registrationNumber](const std::unique_ptr<Ambulance>& ambulance){
        return ambulance->getRegistrationNumber() == registrationNumber;
    });
    return it;
}

void AmbulanceDispatch::addAmbulance(std::unique_ptr<Ambulance> ambulance){
    auto it = getAmbulance(ambulance->getRegistrationNumber());
    if (it != ambulances.end()){
        throw ObjectAlreadyExistsException("Ambulance");
    }
    ambulances.push_back(std::move(ambulance));
}

void AmbulanceDispatch::addParamedic(std::unique_ptr<Paramedic> paramedic){
    paramedics.push_back(std::move(paramedic));
}

std::vector<std::unique_ptr<Ambulance>>::iterator AmbulanceDispatch::getFreeAmbulance(){
    auto it = std::find_if(ambulances.begin(), ambulances.end(), [](const std::unique_ptr<Ambulance>& ambulance){
        return ambulance->getState() == AmbulanceState::IN_GARAGE;
    });
    return it;
}

std::vector<std::unique_ptr<Paramedic>>::iterator AmbulanceDispatch::getFreeParamedic(){
    auto it = std::find_if(paramedics.begin(), paramedics.end(), [](const std::unique_ptr<Paramedic>& paramedic){
        return paramedic->getActivity() == ParamedicActivity::RESTING;
    });
    return it;
}

std::string AmbulanceDispatch::sendAmbulance(std::unique_ptr<Patient> patient){
    auto it = getFreeAmbulance();
    if (it == ambulances.end()){
        throw ObjectNotFoundException("Free ambulance");
    }
    auto it2 = getFreeParamedic();
    if (it2 == paramedics.end()){
        throw ObjectNotFoundException("Free paramedic");
    }
    (*it)->addParamedic(std::move(*it2));
    paramedics.erase(it2);
    auto it3 = getFreeParamedic();
    if (it3 == paramedics.end()){
        throw ObjectNotFoundException("Free paramedic");
    }
    (*it)->addParamedic(std::move(*it3));
    paramedics.erase(it3);
    (*it)->startIntervention(std::move(patient));
    std::stringstream ss;
    ss << *(*it);
    return ss.str();
}

void AmbulanceDispatch::constinueAllInterventions(){
    for (auto& ambulance : ambulances){
        if (ambulance->getState() == AmbulanceState::ON_ROAD){
            ambulance->continueIntervention();
        }
    }
}

std::unique_ptr<Patient> AmbulanceDispatch::getPatient(){
    return std::move(patient);
}

std::string AmbulanceDispatch::checkIfFinishedIntervention(){
    auto it = std::find_if(ambulances.begin(), ambulances.end(), [](const std::unique_ptr<Ambulance>& ambulance){
        return ambulance->getState() == AmbulanceState::RETURNED;
    });
    if (it != ambulances.end()){
        (*it)->setState(AmbulanceState::IN_GARAGE);
        (*it)->resetProgressTime();
        paramedics.push_back((*it)->returnParamedic());
        paramedics.push_back((*it)->returnParamedic());
        patient = (*it)->returnPatient();
        std::stringstream ss;
        ss << *(*it);
        return ss.str();
    }
    return "";
}