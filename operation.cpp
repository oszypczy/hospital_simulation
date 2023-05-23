#include "operation.h"
#include "wrongPersonelException.h"
#include "wrongServiceStateException.h"
#include "wrongPatientException.h"
#include <iostream>
#include <iomanip>

Operation::Operation(ushort ID, unsigned short totalTime, bool NFZ, OperationType type, Diseases disease)
: MedicalService(ID, totalTime, NFZ), type(type), disease(disease), requiredSecialty({{OperationType::HEART_TRANSPLANT, DoctorSpecialty::CARDIOLOGIST}, {OperationType::BRAIN_TUMOR_REMOVAL, DoctorSpecialty::NEUROLOGIST}}) {
}

Operation::~Operation() {}

bool Operation::checkPersonel() const {
    /*
    * Checks if the necessary personnel were added to operation
    * (there must be at least one nurse and two doctors, one of which must be an anesthesiologist)
    * @return True if the personnel are available, false otherwise.
    */
    if (nurses.size() < 1) {
        return false;
    }
    try {
        this->getDoctor(DoctorSpecialty::ANESTESIOLOGYST);
        this->getDoctor(requiredSecialty.at(type));
    } catch (std::invalid_argument& e) {
        return false;
    }
    return true;
}

void Operation::startService(std::unique_ptr<Patient> newPatient) {
    /*
    * Starts the operation service with the provided patient.
    * Changes the state of patient, nurses and doctors
    * calls operator++() to start the operation
    * @param newPatient The patient to start the operation service with.
    * @throws WrongPersonelException if the necessary personnel are not available for the operation.
    * @throws WrongPatientException if the patient's health card does not have it planned or the disease is not on the health card.
    */
    if (!checkPersonel()){
        this->patient = std::move(newPatient);
        throw WrongPersonelException();
    }
    if (!(newPatient->getHealthCard().checkService(ID))){
        this->patient = std::move(newPatient);
        throw WrongPatientException();
    }
    if (!(newPatient->getHealthCard().checkDisease(disease))){
        this->patient = std::move(newPatient);
        throw WrongPatientException();
    }
    this->state = ServiceState::IN_PROGRESS;
    this->patient = std::move(newPatient);
    (*patient).setState(PatientState::IN_OR);
    for (auto& doctor : doctors) {
        (*doctor).setActivity(DoctorActivity::OPERATING);
    }
    for (auto& nurse : nurses) {
        (*nurse).setActivity(NurseActivity::OPERATION_ASSISTANCE);
    }
    this->operator++();
}

void Operation::continueService() {
    /*
    * Continues the operation service.
    * Progresses the operation if it is in progress or finishes it if it is already finished.
    * @throws WrongServiceStateException if the operation is not in progress or not ready to be progressed.
    */
    if (state == ServiceState::IN_PROGRESS) {
        this->operator++();
    }
    if (state == ServiceState::FINISHED) {
        finishService();
    }
    if (state == ServiceState::NOT_READY){
        throw WrongServiceStateException("NOT_READY", "IN_PROGRESS");
    }
}

void Operation::finishService() {
    /*
    * Finishes the operation service.
    * Updates the state of the patient and the activity of the medical personnel.
    * Cures the disease on the patient's health card.
    * @throws WrongServiceStateException if the operation is already finished or not ready to be finished.
    */
    if (state == ServiceState::FINISHED) {
        (*patient).setState(PatientState::RESTING);
        (*patient).getHealthCard().cureDisease(disease);
        for (auto& doctor : doctors) {
            (*doctor).setActivity(DoctorActivity::RESTING);
        }
        for (auto& nurse : nurses) {
            (*nurse).setActivity(NurseActivity::RESTING);
        }
    } else if (state == ServiceState::IN_PROGRESS) {
        throw WrongServiceStateException("IN_PROGRESS", "FINISHED");
    } else {
        throw WrongServiceStateException("NOT_READY", "FINISHED");
    }
}

uint Operation::calculateCost() const {
    uint cost = 0;
    if (NFZ) {
        return cost;
    }
    for (const auto& doctor : doctors) {
        cost += doctor->getHourlyWage();
    }
    for (const auto& nurse : nurses) {
        cost += nurse->getHourlyWage();
    }
    cost = ((100 * cost) / 60) * totalTime;
    cost += 90000;
    cost = 1.23 * cost;
    return cost;
}

std::string Operation::print() const {
    /*
    * Generates a string representation of the operation service.
    * Neccesary for virtualization of the print() method from MedicalService.
    * @return The string representation of the operation service.
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
    if (nurses.size() != 0){
        for (auto& nurse : nurses) {
            os << (*nurse) << std::endl;
        }
    }
    ushort zl = calculateCost() / 100;
    ushort gr = calculateCost() % 100;
    os << "Operation ID: " << ID << std::endl;
    os << "Cost of Operation: " << zl << "." << std::setw(2) << std::setfill('0') << std::to_string(gr) << " zł" << std::endl;
    os << "Time of the Operation left: " << totalTime - progressTime << "min" << std::endl;
    os << "NFZ: " << NFZ;
    return os.str();
}

std::ostream& operator<<(std::ostream& os, const Operation& operation) {
    os << operation.print();
    return os;
}