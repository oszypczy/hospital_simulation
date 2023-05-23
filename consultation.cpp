#include "consultation.h"
#include "wrongPersonelException.h"
#include "wrongServiceStateException.h"
#include "wrongPatientException.h"
#include <iostream>
#include <iomanip>

Consultation::Consultation(ushort ID, unsigned short totalTime, bool NFZ) : MedicalService(ID, totalTime, NFZ) {}

Consultation::~Consultation() {}

bool Consultation::checkPersonel() const {
    /*
    * Checks if the necessary personnel were added to consultation
    * (there must be at least one doctor)
    * @return True if the personnel are available, false otherwise.
    */
    if (doctors.size() < 1) {
        return false;
    }
    return true;
}

void Consultation::startService(std::unique_ptr<Patient> newPatient) {
    /*
    * Starts the consultation service with the provided patient.
    * Changes the state of patient and doctor
    * calls operator++() to start the operation
    * @param newPatient The patient to start the operation service with.
    * @throws WrongPersonelException if the necessary personnel are not available for the consultation.
    * @throws WrongPatientException if the patient's health card does not have it planned
    */
    if (!checkPersonel()){
        this->patient = std::move(newPatient);
        throw WrongPersonelException();
    }
    if (!(newPatient->getHealthCard().checkService(ID))){
        this->patient = std::move(newPatient);
        throw WrongPatientException();
    }
    this->state = ServiceState::IN_PROGRESS;
    this->patient = std::move(newPatient);
    (*patient).setState(PatientState::ON_APPOITMENT);
    for (auto& doctor : doctors) {
        (*doctor).setActivity(DoctorActivity::CONSULTING);
    }
    this->operator++();
}

void Consultation::continueService() {
    /*
    * Continues the consultation service.
    * Progresses the consultation if it is in progress or finishes it if it is already finished.
    * @throws WrongServiceStateException if the consultation is not in progress or not ready to be progressed.
    */
    if (state == ServiceState::IN_PROGRESS) {
        this->operator++();
    }
    if (state == ServiceState::FINISHED) {
        finishService();
    } else {
        throw WrongServiceStateException("NOT_READY", "IN_PROGRESS");
    }
}

void Consultation::finishService() {
    /*
    * Finishes the consultation service
    * Updates the state of the patient and the activity of the medical personnel
    * @throws WrongServiceStateException if the consultation is already finished or not ready to be finished.
    */
    if (state == ServiceState::FINISHED) {
        (*patient).setState(PatientState::RESTING);
        for (auto& doctor : doctors) {
            (*doctor).setActivity(DoctorActivity::RESTING);
        }
    } else if (state == ServiceState::IN_PROGRESS) {
        throw WrongServiceStateException("IN_PROGRESS", "FINISHED");
    } else {
        throw WrongServiceStateException("NOT_READY", "FINISHED");
    }
}

uint Consultation::calculateCost() const {
    uint cost = 0;
    if (NFZ) {
        return cost;
    }
    for (const auto& doctor : doctors) {
        cost += doctor->getHourlyWage();
    }
    cost = (100 * cost) / 60 * totalTime;
    cost += 15000;
    cost = 1.2 * cost;
    return cost;
}

std::string Consultation::print() const {
    /*
    * Generates a string representation of the consultation service.
    * Neccesary for virtualization of the print() method from MedicalService.
    * @return The string representation of the consltation service.
    */
    std::stringstream os;
    if (patient){
        os << *patient << std::endl;
    }
    if (doctors.size() != 0){
        os << "Medical personel: ";
        for (auto& doctor : doctors) {
            os << (*doctor) << std::endl;
    }
    }
    ushort zl = calculateCost() / 100;
    ushort gr = calculateCost() % 100;
    os << "Consultation ID: " << ID << std::endl;
    os << "Cost of consultation: " << zl << "." << std::setw(2) << std::setfill('0') << std::to_string(gr) << " zł" << std::endl;
    os << "Time of the consultation left: " << totalTime - progressTime << "min" << std::endl;
    os << "NFZ: " << NFZ;
    return os.str();
}

std::ostream& operator<<(std::ostream& os, const Consultation& consultation) {
    os << consultation.print();
    return os;
}
