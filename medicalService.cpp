#include "medicalService.h"
#include "wrongServiceStateException.h"
#include "emptyAttributeException.h"
#include "objectNotFoundException.h"
#include <iostream>

MedicalService::MedicalService(ushort ID, unsigned short totalTime, bool NFZ) : totalTime(totalTime), NFZ(NFZ), ID(ID) {}

MedicalService::~MedicalService() {}

void MedicalService::operator++() {
    if (state == ServiceState::NOT_READY) {
        throw WrongServiceStateException("Not ready", "In progress");
    }
    progressTime += 15;
    if (progressTime >= totalTime) {
        progressTime = totalTime;
        state = ServiceState::FINISHED;
    }
}

void MedicalService::addDoctor(std::unique_ptr<Doctor> doctor) {
    doctors.push_back(std::move(doctor));
}

void MedicalService::addNurse(std::unique_ptr<Nurse> nurse) {
    nurses.push_back(std::move(nurse));
}

ServiceState MedicalService::getServiceState() const {
    return state;
}

ushort MedicalService::getID() const {
    return ID;
}

void MedicalService::changeState(ServiceState state) {
    this->state = state;
}

unsigned short MedicalService::getProgressTime() const {
    return progressTime;
}

Patient& MedicalService::getPatient() const {
    if (!patient) {
        throw EmptyAttributeException("Patient attribute");
    }
    return *patient;
}

Doctor& MedicalService::getDoctor(std::string PESEL) const {
    for (const auto& doctor : doctors) {
        if (doctor->getPESEL() == PESEL) {
            return *doctor;
        }
    }
    throw ObjectNotFoundException("Doctor with PESEL: " + PESEL);
}

Doctor& MedicalService::getDoctor(DoctorSpecialty specialty) const {
    for (const auto& doctor : doctors) {
        if (doctor->getSpecialty() == specialty) {
            return *doctor;
        }
    }
    throw ObjectNotFoundException("Doctor with the given specialty");
}

Nurse& MedicalService::getNurse(std::string PESEL) const {
    for (const auto& nurse : nurses) {
        if (nurse->getPESEL() == PESEL) {
            return *nurse;
        }
    }
    throw ObjectNotFoundException("Nurse with PESEL: " + PESEL);
}

std::unique_ptr<Patient> MedicalService::returnPatient() {
    return std::move(patient);
}

std::ostream& operator<<(std::ostream& os, const MedicalService& service) {
    os << service.print();
    return os;
}