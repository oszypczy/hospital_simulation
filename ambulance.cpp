#include "ambulance.h"
#include "wrongPersonelException.h"
#include "wrongServiceStateException.h"
#include "objectNotFoundException.h"
#include "invalidRegistrationNumberException.h"
#include "wrongPersonStateException.h"
#include <algorithm>
#include <iostream>

Ambulance::Ambulance(std::string registrationNumber, unsigned short totalTime) {
    checkRegistrationNumber(registrationNumber);
    this->totalTime = totalTime;
    this->registrationNumber = registrationNumber;
}

void Ambulance::checkRegistrationNumber(std::string registrationNumber) const{
    if (registrationNumber.length() != 7) {
        throw InvalidRegistrationNumberException(registrationNumber);
    }
    for (char c : registrationNumber) {
        if (!std::isalnum(c) || std::islower(c)) {
            throw InvalidRegistrationNumberException(registrationNumber);
        }
    }
}

void Ambulance::setRegistrationNumber(std::string newRegistrationNumber) {
    checkRegistrationNumber(newRegistrationNumber);
    registrationNumber = newRegistrationNumber;
}

ushort Ambulance::getProgressTime() const{
    return progressTime;
}

ushort Ambulance::getTotalTime() const{
    return totalTime;
}

std::string Ambulance::getRegistrationNumber() const{
    return registrationNumber;
}

bool Ambulance::checkOccupancy() const{
    return isOccupied;
}

AmbulanceState Ambulance::getState() const{
    return state;
}

Patient& Ambulance::getPatient() const{
    return *patient;
}

Paramedic& Ambulance::getParamedic(std::string PESEL) const{
    for (auto& paramedic : paramedics) {
        if ((*paramedic).getPESEL() == PESEL) {
            return *paramedic;
        }
    }
    throw ObjectNotFoundException("No paramedic with given PESEL");
}

void Ambulance::operator++() {
    if (state == AmbulanceState::IN_GARAGE) {
        throw WrongServiceStateException("IN_GARAGE", "ON_ROAD");
    }
    progressTime += 15;
    if (progressTime >= totalTime) {
        progressTime = totalTime;
        state = AmbulanceState::RETURNED;
    }
}

std::unique_ptr<Paramedic> Ambulance::addParamedic(std::unique_ptr<Paramedic> paramedic) {
    if (paramedic->getActivity() == ParamedicActivity::RESTING){
        paramedics.push_back(std::move(paramedic));
        return nullptr;
    }
    return std::move(paramedic);
}

bool Ambulance::checkPersonel() const {
    if (paramedics.size() < 2) {
        return false;
    }
    return true;
}

void Ambulance::startIntervention(std::unique_ptr<Patient> newPatient) {
    if (!checkPersonel()){
        this->patient = std::move(newPatient);
        throw WrongPersonelException();
    }
    isOccupied = true;
    this->state = AmbulanceState::ON_ROAD;
    this->patient = std::move(newPatient);
    (*patient).setState(PatientState::IN_AMBULANCE);
    for (auto& paramedic : paramedics) {
        (*paramedic).setActivity(ParamedicActivity::IN_AMBULANCE);
    }
    this->operator++();
}

void Ambulance::continueIntervention() {
    if (state == AmbulanceState::ON_ROAD) {
        this->operator++();
    }
    if (state == AmbulanceState::RETURNED) {
        finishIntervention();
    } else if (state == AmbulanceState::IN_GARAGE){
        throw WrongServiceStateException("IN_GARAGE", "ON_ROAD");
    }
}

void Ambulance::finishIntervention() {
    if (state == AmbulanceState::RETURNED) {
        (*patient).setState(PatientState::RESTING);
        for (auto& paramedic : paramedics) {
            (*paramedic).setActivity(ParamedicActivity::RESTING);
        }
        isOccupied = false;
        state = AmbulanceState::IN_GARAGE;
    } else if (state == AmbulanceState::ON_ROAD) {
        throw WrongServiceStateException("ON_ROAD", "RETURNED");
    } else {
        throw WrongServiceStateException("IN_GARAGE", "RETURNED");
    }
}

std::unique_ptr<Patient> Ambulance::returnPatient() {
    if (patient == nullptr) {
        throw ObjectNotFoundException("Patient inside ambulance");
    }
    if (patient->getState() == PatientState::IN_AMBULANCE) {
        throw WrongPersonStateException("to be in ambulance", "rest");
    }
    return std::move(patient);
}

std::unique_ptr<Paramedic> Ambulance::returnParamedic(std::string PESEL) {
    auto it = std::find_if(paramedics.begin(), paramedics.end(), [PESEL](const std::unique_ptr<Paramedic>& paramedic) {
        return paramedic->getPESEL() == PESEL;
    });
    if (it == paramedics.end()) {
        throw ObjectNotFoundException("Paramedic with given PESEL");
    }
    if ((*it)->getActivity() == ParamedicActivity::IN_AMBULANCE) {
        throw WrongPersonStateException("to be in ambulance", "rest");
    }
    auto tempParamedic = std::move(*it);
    paramedics.erase(it);
    return tempParamedic;
}

std::ostream& operator<<(std::ostream& os, const Ambulance& ambulance) {
    os << "Ambulance " << ambulance.registrationNumber << " is ";
    if (ambulance.state == AmbulanceState::IN_GARAGE) {
        os << "in garage" << std::endl;
    } else if (ambulance.state == AmbulanceState::ON_ROAD) {
        os << "on road" << std::endl;
    }
    os << "Progress time: " << ambulance.progressTime << std::endl;
    os << "Estimated total time: " << ambulance.totalTime << std::endl;
    os << "STAFF: " << std::endl;
    for (const auto& paramedic : ambulance.paramedics) {
        os << "\t" << (*paramedic) << std::endl;
    }
    if (ambulance.isOccupied) {
        os << "PATIENT: " << std::endl;
        os << "\t" << (*ambulance.patient) << std::endl;
    }
    return os;
}
