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
    /*
    * Checks if the ambulance is occupied.
    * @return True if the ambulance is occupied, false otherwise.
    */
    return isOccupied;
}

AmbulanceState Ambulance::getState() const{
    return state;
}

Patient& Ambulance::getPatient() const{
    return *patient;
}

Paramedic& Ambulance::getParamedic(std::string PESEL) const{
    /*
    * Gets the paramedic with the specified PESEL.
    * @param PESEL The PESEL of the paramedic to retrieve.
    * @return A reference to the paramedic object.
    * @throws ObjectNotFoundException If no paramedic with the given PESEL is found.
    */
    for (auto& paramedic : paramedics) {
        if ((*paramedic).getPESEL() == PESEL) {
            return *paramedic;
        }
    }
    throw ObjectNotFoundException("No paramedic with given PESEL");
}

void Ambulance::operator++() {
    /*
    * Overloads the prefix increment operator to advance the progress time of the ambulance.
    * Changes the state of the ambulance to RETURNED if the progress time reaches the total time.
    * @throws WrongServiceStateException If the ambulance is not in the correct state for incrementing.
    */
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
    /*
    * Adds a paramedic to the ambulance (only when the paramedic is resting).
    * @param paramedic A unique pointer to the Paramedic object to be added.
    * @return A unique pointer to the added paramedic if the paramedic's activity is not IN_AMBULANCE, nullptr otherwise.
    */
    if (paramedic->getActivity() == ParamedicActivity::RESTING){
        paramedics.push_back(std::move(paramedic));
        return nullptr;
    }
    return std::move(paramedic);
}

bool Ambulance::checkPersonel() const {
    /*
    * Checks if the ambulance has the required personnel.
    * @return True if the ambulance has at least two paramedics, false otherwise.
    */
    if (paramedics.size() < 2) {
        return false;
    }
    return true;
}

void Ambulance::setState(AmbulanceState newState) {
    /*
    * Sets the state of the ambulance.
    * @param newState The new state of the ambulance.
    */
    state = newState;
}

void Ambulance::startIntervention(std::unique_ptr<Patient> newPatient) {
    /*
    * Starts an intervention by assigning a new patient to the ambulance.
    * Changes the state of patient and paramedics.
    * Calls operator++() to start the intervention.
    * @param newPatient A unique pointer to the Patient object to be assigned.
    * @throws WrongPersonelException If the ambulance does not have the required personnel.
    */
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
    /*
    * Continues the intervention by advancing the progress time of the ambulance.
    * Throws an exception if the ambulance state is not valid for continuing the intervention.
    * If the progress time reaches or exceeds the total time, the intervention is finished.
    * @throws WrongServiceStateException If the ambulance is not in the correct state for continuing the intervention.
    */
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
    /*
    * Finishes the intervention by updating the state of the ambulance and the associated personnel and patient.
    * Throws an exception if the ambulance state is not valid for finishing the intervention.
    * @throws WrongServiceStateException If the ambulance is not in the correct state for finishing the intervention.
    */
    if (state == AmbulanceState::RETURNED) {
        (*patient).setState(PatientState::RESTING);
        for (auto& paramedic : paramedics) {
            (*paramedic).setActivity(ParamedicActivity::RESTING);
        }
        isOccupied = false;
    } else if (state == AmbulanceState::ON_ROAD) {
        throw WrongServiceStateException("ON_ROAD", "RETURNED");
    } else {
        throw WrongServiceStateException("IN_GARAGE", "RETURNED");
    }
}

std::unique_ptr<Patient> Ambulance::returnPatient() {
    /*
    * Returns the patient from the ambulance.
    * @return A unique pointer to the Patient object in the ambulance.
    * @throws ObjectNotFoundException If no patient is found inside the ambulance.
    * @throws WrongPersonStateException If the patient is not in the expected state to be returned.
    */
    if (patient == nullptr) {
        throw ObjectNotFoundException("Patient inside ambulance");
    }
    if (patient->getState() == PatientState::IN_AMBULANCE) {
        throw WrongPersonStateException("to be in ambulance", "rest");
    }
    return std::move(patient);
}

std::unique_ptr<Paramedic> Ambulance::returnParamedic() {
    /*
    * Returns the paramedic who finished the intervention.
    * @return A unique pointer to the Paramedic object with the specified PESEL.
    * @throws ObjectNotFoundException If no paramedic with the given PESEL is found.
    * @throws WrongPersonStateException If the paramedic is not in the expected state to be returned.
    */
    auto it = std::find_if(paramedics.begin(), paramedics.end(), [](const std::unique_ptr<Paramedic>& paramedic) {
        return paramedic->getActivity() == ParamedicActivity::RESTING;
    });
    if (it == paramedics.end()) {
        throw ObjectNotFoundException("Paramedic who rests in ambulance");
    }
    auto tempParamedic = std::move(*it);
    paramedics.erase(it);
    return tempParamedic;
}

std::ostream& operator<<(std::ostream& os, const Ambulance& ambulance) {
    os << "Ambulance " << ambulance.registrationNumber << " is ";
    if (ambulance.state == AmbulanceState::IN_GARAGE) {
        os << "in garage and returned from intervention." << std::endl;
        return os;
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
