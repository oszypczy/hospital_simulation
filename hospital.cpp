#include "hospital.h"
#include "ward.h"
#include "invalidHumanPointer.h"
#include "invalidHospitalName.h"
#include "invalidPlaceIDs.h"

#include <algorithm>

Hospital::Hospital(std::string name, std::list<std::unique_ptr<Ward>> wardsList,
                    std::list<std::unique_ptr<Doctor>> doctorsList, std::list<std::unique_ptr<Nurse>> nursesList,
                    std::unique_ptr<Reception> reception,
                    std::unique_ptr<AmbulanceDispatch> ambulanceDispatch):
                    name(name), wardsList(std::move(wardsList)), doctorsList(std::move(doctorsList)),
                    nursesList(std::move(nursesList)), reception(std::move(reception)),
                    ambulanceDispatch(std::move(ambulanceDispatch))
                    {
                        if(name.empty()){
                            throw InvalidHospitalName();
                        }
                        if (!checkIfPlacesIDUnique()){
                            throw InvalidPlaceIDs();
                        }
                    }

bool Hospital::checkIfPlacesIDUnique()const{
    std::list<std::string> duplicatePlacesList;

    for(const auto& ward : wardsList){
        duplicatePlacesList.push_back(ward->getID());

        for(const auto& room : ward->getGeneralRoomList()){
            duplicatePlacesList.push_back(room->getID());
            }
        duplicatePlacesList.push_back(ward->getTreatmentRoom()->getID());
        duplicatePlacesList.push_back(ward->getConsultationRoom()->getID());
        }

    std::list<std::string> placesList = duplicatePlacesList;
    duplicatePlacesList.unique();
    return (duplicatePlacesList.size() == placesList.size());
}


void Hospital::addDoctor(std::unique_ptr<Doctor> doctor){
    doctorsList.push_back(std::move(doctor));
}

void Hospital::addNurse(std::unique_ptr<Nurse> nurse){
    nursesList.push_back(std::move(nurse));
}

void Hospital::removeDoctor(std::unique_ptr<Doctor> doctor){
    auto it = std::find_if(doctorsList.begin(), doctorsList.end(), [&](const std::unique_ptr<Doctor>& d) {
        return *d == *doctor;
    });

    if (it != doctorsList.end()) {
        doctorsList.erase(it);
    }
    else throw InvalidHumanPointer("Doctor");
}

void Hospital::removeNurse(std::unique_ptr<Nurse> nurse){
    auto it = std::find_if(nursesList.begin(), nursesList.end(), [&](const std::unique_ptr<Nurse>& n) {
        return *n == *nurse;
    });

    if (it != nursesList.end()) {
        nursesList.erase(it);
    }
    else throw InvalidHumanPointer("Nurse");
}

std::unique_ptr<Doctor> Hospital::moveDoctor(std::unique_ptr<Doctor> doctor){
    auto it = std::find_if(doctorsList.begin(), doctorsList.end(), [&](const std::unique_ptr<Doctor>& d) {
        return *d == *doctor;
    });

    if (it != doctorsList.end()) {
        std::unique_ptr<Doctor> temp = std::move(*it);
        doctorsList.erase(it);
        return temp;
    }
    else throw InvalidHumanPointer("Doctor");
}

std::unique_ptr<Nurse> Hospital::moveNurse(std::unique_ptr<Nurse> nurse){
    auto it = std::find_if(nursesList.begin(), nursesList.end(), [&](const std::unique_ptr<Nurse>& n) {
        return *n == *nurse;
    });

    if (it != nursesList.end()) {
        std::unique_ptr<Nurse> temp = std::move(*it);
        nursesList.erase(it);
        return temp;
    }
    else throw InvalidHumanPointer("Nurse");
}

std::list<std::unique_ptr<Ward>>& Hospital::getWardsList(){
    return wardsList;
}

std::list<std::unique_ptr<Doctor>>& Hospital::getDoctorsList(){
    return doctorsList;
}

std::list<std::unique_ptr<Nurse>>& Hospital::getNursesList(){
    return nursesList;
}

std::unique_ptr<Reception>& Hospital::getReception(){
    return reception;
}

std::unique_ptr<AmbulanceDispatch>& Hospital::getAmbulanceDispatch(){
    return ambulanceDispatch;
}

std::string Hospital::getName()const{
    return name;
}
