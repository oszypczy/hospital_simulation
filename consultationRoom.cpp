#include "consultationRoom.h"
#include "invalidHumanPointer.h"

#include <algorithm>

ConsultationRoom::ConsultationRoom(std::string id): Room(id){}


void ConsultationRoom::addPatientToQueue(std::unique_ptr<Patient> patient){
    patientsQueue.push_back(std::move(patient));
}


void ConsultationRoom::removePatient(std::unique_ptr<Patient> patient){
    auto it = std::find_if(patientsQueue.begin(), patientsQueue.end(), [&](const std::unique_ptr<Patient>& p) {
        return *p == *patient;
    });

    if (it != patientsQueue.end()) {
        patientsQueue.erase(it);
    }
    else throw InvalidHumanPointer("Patient");
}

std::unique_ptr<Patient> ConsultationRoom::getFirstPatientInQueue(){
    auto it = std::move(patientsQueue.front());
    patientsQueue.pop_front();
    return it;
}

int ConsultationRoom::getCurrentServiceID() const{
    return currentServiceID;
}

void ConsultationRoom::setCurrentServiceID(int id){
    currentServiceID = id;
}

bool ConsultationRoom::checkIfAnyoneInQueue(){
    return !patientsQueue.empty();
}


// void ConsultationRoom::addDoctor(std::unique_ptr<Doctor> doctor){
//     doctors.push_back(std::move(doctor));
// }


// void ConsultationRoom::removeDoctor(std::unique_ptr<Doctor> doctor){
//     auto it = std::find_if(doctors.begin(), doctors.end(), [&](const std::unique_ptr<Doctor>& d) {
//         return *d == *doctor;
//     });

//     if (it != doctors.end()) {
//         doctors.erase(it);
//     }
//     else throw InvalidHumanPointer("Doctor");
// }


// void ConsultationRoom::addNurse(std::unique_ptr<Nurse> nurse){
//     nurses.push_back(std::move(nurse));
// }


// void ConsultationRoom::removeNurse(std::unique_ptr<Nurse> nurse){
//     auto it = std::find_if(nurses.begin(), nurses.end(), [&](const std::unique_ptr<Nurse>& n) {
//         return *n == *nurse;
//     });

//     if (it != nurses.end()) {
//         nurses.erase(it);
//     }
//     else throw InvalidHumanPointer("Nurse");
// }


// std::unique_ptr<Patient> ConsultationRoom::movePatient(std::unique_ptr<Patient> patient){
//     auto it = std::find_if(patientsQueue.begin(), patientsQueue.end(), [&](const std::unique_ptr<Patient>& p) {
//         return *p == *patient;
//     });

//     if (it != patientsQueue.end()) {
//         patientsQueue.erase(it);
//     }
//     else throw InvalidHumanPointer("Patient");

//     return std::move(*it);
// }

// std::unique_ptr<Doctor> ConsultationRoom::moveDoctor(std::unique_ptr<Doctor> doctor){
//     auto it = std::find_if(doctors.begin(), doctors.end(), [&](const std::unique_ptr<Doctor>& d) {
//         return *d == *doctor;
//     });

//     if (it != doctors.end()) {
//         doctors.erase(it);
//     }
//     else throw InvalidHumanPointer("Doctor");

//     return std::move(*it);
// }

// std::unique_ptr<Nurse> ConsultationRoom::moveNurse(std::unique_ptr<Nurse> nurse){
//     auto it = std::find_if(nurses.begin(), nurses.end(), [&](const std::unique_ptr<Nurse>& n) {
//         return *n == *nurse;
//     });

//     if (it != nurses.end()) {
//         nurses.erase(it);
//     }
//     else throw InvalidHumanPointer("Nurse");

//     return std::move(*it);
// }

// void ConsultationRoom::setConsultedPatient(std::unique_ptr<Patient> patient){
//     this->consultedPatient = std::move(patient);
// }

// std::unique_ptr<Patient> ConsultationRoom::moveConsultedPatient(){
//     return std::move(consultedPatient);
// }

// void ConsultationRoom::setConsultation(std::unique_ptr<Consultation> consultation){
//     this->consultation = std::move(consultation);
// }

// std::list<std::unique_ptr<Doctor>>& ConsultationRoom::getDoctors(){
//     return doctors;
// }

// std::list<std::unique_ptr<Nurse>>& ConsultationRoom::getNurses(){
//     return nurses;
// }

// std::unique_ptr<Patient>& ConsultationRoom::getConsultedPatient(){
//     return consultedPatient;
// }

// std::unique_ptr<Consultation>& ConsultationRoom::getConsultation(){
//     return consultation;
// }
