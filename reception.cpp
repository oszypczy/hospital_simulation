#include "reception.h"

Reception::Reception(std::string id): Place(id){}

void Reception::addPatientToQueueFirst(std::unique_ptr<Patient> patient){
    patientsQueue.push_front(std::move(patient));
}

void Reception::addPatientToQueueLast(std::unique_ptr<Patient> patient){
    patientsQueue.push_back(std::move(patient));
}

void Reception::addNurse(std::unique_ptr<Nurse> nurse){
    this->nurse = std::move(nurse);
}

std::unique_ptr<Nurse> Reception::moveNurse(){
    return std::move(nurse);
}

std::unique_ptr<Patient> Reception::movePatient(){
    auto tempPatient = std::move(patientsQueue.front());
    patientsQueue.pop_front();
    return tempPatient;
}
