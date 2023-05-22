#include "treatmentRoom.h"
#include "invalidHumanPointer.h"

#include <algorithm>

TreatmentRoom::TreatmentRoom(std::string id): Room(id){}


void TreatmentRoom::proceedTreatment(){

}


void TreatmentRoom::addPatientToQueue(std::unique_ptr<Patient> patient){
    patientsQueue.push_back(std::move(patient));
}


void TreatmentRoom::removePatient(std::unique_ptr<Patient> patient){
//     auto it = std::find_if(patients.begin(), patients.end(), [&](const std::unique_ptr<Human>& h) {
//         return *p == *patient;
//     });

//     if (it != humansList.end()) {
//         humansList.erase(it);
//     }
//     else throw InvalidHumanPointer();
}


void TreatmentRoom::addDoctor(){

}


void TreatmentRoom::removeDoctor(){

}


void TreatmentRoom::addNurse(){

}


void TreatmentRoom::removeNurse(){

}

