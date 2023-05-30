#include "medicalService.h"
#include "wrongServiceStateException.h"
#include "emptyAttributeException.h"
#include "objectNotFoundException.h"
#include "wrongPersonStateException.h"
#include <iostream>
#include <algorithm>

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

std::unique_ptr<Doctor> MedicalService::addDoctor(std::unique_ptr<Doctor> doctor) {
    if (doctor->getActivity() == DoctorActivity::RESTING) {
        doctors.push_back(std::move(doctor));
        return nullptr;
    }
    return std::move(doctor);
}

std::unique_ptr<Nurse> MedicalService::addNurse(std::unique_ptr<Nurse> nurse) {
    if (nurse->getActivity() == NurseActivity::RESTING) {
        nurses.push_back(std::move(nurse));
        return nullptr;
    }
    return std::move(nurse);
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

std::list<std::unique_ptr<Doctor>>::iterator MedicalService::getDoctor(std::string PESEL) {
    auto it = std::find_if(doctors.begin(), doctors.end(), [PESEL](const std::unique_ptr<Doctor>& doctor) {
        return doctor->getPESEL() == PESEL;
    });
    if (it == doctors.end()){
        throw ObjectNotFoundException("Doctor with PESEL: " + PESEL);
    }
    return it;
}

Doctor& MedicalService::getDoctor(DoctorSpecialty specialty) const {
    for (const auto& doctor : doctors) {
        if (doctor->getSpecialty() == specialty) {
            return *doctor;
        }
    }
    throw ObjectNotFoundException("Doctor with the given specialty");
}

Doctor& MedicalService::getFirstDoctor() const {
    if (doctors.empty()) {
        throw ObjectNotFoundException("Doctor");
    }
    return *doctors.front();
}

Nurse& MedicalService::getFirstNurse() const {
    if (nurses.empty()) {
        throw ObjectNotFoundException("Nurse");
    }
    return *nurses.front();
}

std::list<std::unique_ptr<Nurse>>::iterator MedicalService::getNurse(std::string PESEL) {
    auto it = std::find_if(nurses.begin(), nurses.end(), [PESEL](const std::unique_ptr<Nurse>& nurse) {
        return nurse->getPESEL() == PESEL;
    });
    if (it == nurses.end()){
        throw ObjectNotFoundException("Nurse with PESEL: " + PESEL);
    }
    return it;
}

std::unique_ptr<Patient> MedicalService::returnPatient() {
    if (patient == nullptr) {
        throw ObjectNotFoundException("Patient in service");
    }
    if (patient->getState() == PatientState::ON_APPOITMENT) {
        throw WrongPersonStateException("to be on consultation", "rest");
    }
    if (patient->getState() == PatientState::IN_OR) {
        throw WrongPersonStateException("to be on operation", "rest");
    }
    return std::move(patient);
}

std::unique_ptr<Doctor> MedicalService::returnDoctor(std::string PESEL) {
    auto it = getDoctor(PESEL);
    if ((*it)->getActivity() == DoctorActivity::OPERATING) {
        throw WrongPersonStateException("to be on operation", "rest");
    }
    if ((*it)->getActivity() == DoctorActivity::CONSULTING) {
        throw WrongPersonStateException("to be on consultation", "rest");
    }
    auto tempDoctor = std::move(*it);
    doctors.erase(it);
    return tempDoctor;
}

std::unique_ptr<Nurse> MedicalService::returnNurse(std::string PESEL) {
    auto it = getNurse(PESEL);
    if ((*it)->getActivity() == NurseActivity::OPERATION_ASSISTANCE) {
        throw WrongPersonStateException("to be on operation", "rest");
    }
    if ((*it)->getActivity() == NurseActivity::RECEPTION_SERVICE) {
        throw WrongPersonStateException("work in reception", "rest");
    }
    auto tempNurse = std::move(*it);
    nurses.erase(it);
    return tempNurse;
}

std::ostream& operator<<(std::ostream& os, const MedicalService& service) {
    os << service.print();
    return os;
}