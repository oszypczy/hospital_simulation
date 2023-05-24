#include "treatmentRoom.h"
#include "invalidHumanPointer.h"

#include <algorithm>

TreatmentRoom::TreatmentRoom(std::string id): Room(id){}


void TreatmentRoom::addPatientToQueue(std::unique_ptr<Patient> patient){
    patientsQueue.push_back(std::move(patient));
}


void TreatmentRoom::removePatient(std::unique_ptr<Patient> patient){
    auto it = std::find_if(patientsQueue.begin(), patientsQueue.end(), [&](const std::unique_ptr<Patient>& p) {
        return *p == *patient;
    });

    if (it != patientsQueue.end()) {
        patientsQueue.erase(it);
    }
    else throw InvalidHumanPointer("Patient");
}


void TreatmentRoom::addDoctor(std::unique_ptr<Doctor> doctor){
    doctors.push_back(std::move(doctor));
}


void TreatmentRoom::removeDoctor(std::unique_ptr<Doctor> doctor){
    auto it = std::find_if(doctors.begin(), doctors.end(), [&](const std::unique_ptr<Doctor>& d) {
        return *d == *doctor;
    });

    if (it != doctors.end()) {
        doctors.erase(it);
    }
    else throw InvalidHumanPointer("Doctor");
}


void TreatmentRoom::addNurse(std::unique_ptr<Nurse> nurse){
    nurses.push_back(std::move(nurse));
}


void TreatmentRoom::removeNurse(std::unique_ptr<Nurse> nurse){
    auto it = std::find_if(nurses.begin(), nurses.end(), [&](const std::unique_ptr<Nurse>& n) {
        return *n == *nurse;
    });

    if (it != nurses.end()) {
        nurses.erase(it);
    }
    else throw InvalidHumanPointer("Nurse");
}


std::unique_ptr<Patient> TreatmentRoom::movePatient(std::unique_ptr<Patient> patient){
    auto it = std::find_if(patientsQueue.begin(), patientsQueue.end(), [&](const std::unique_ptr<Patient>& p) {
        return *p == *patient;
    });

    if (it != patientsQueue.end()) {
        patientsQueue.erase(it);
    }
    else throw InvalidHumanPointer("Patient");

    return std::move(*it);
}

std::unique_ptr<Doctor> TreatmentRoom::moveDoctor(std::unique_ptr<Doctor> doctor){
    auto it = std::find_if(doctors.begin(), doctors.end(), [&](const std::unique_ptr<Doctor>& d) {
        return *d == *doctor;
    });

    if (it != doctors.end()) {
        doctors.erase(it);
    }
    else throw InvalidHumanPointer("Doctor");

    return std::move(*it);
}

std::unique_ptr<Nurse> TreatmentRoom::moveNurse(std::unique_ptr<Nurse> nurse){
    auto it = std::find_if(nurses.begin(), nurses.end(), [&](const std::unique_ptr<Nurse>& n) {
        return *n == *nurse;
    });

    if (it != nurses.end()) {
        nurses.erase(it);
    }
    else throw InvalidHumanPointer("Nurse");

    return std::move(*it);
}
