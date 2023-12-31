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

std::unique_ptr<Doctor> Hospital::getFreeDoctor(){
    for(auto& doctor : doctorsList){
        if(doctor->getActivity() == DoctorActivity::RESTING){
            auto temp = std::move(doctor);
            doctorsList.remove(doctor);
            return temp;
        }
    }
    return nullptr;
}

std::unique_ptr<ServiceDataBase>& Hospital::getServiceDataBase(){
    return serviceDataBase;
}

std::unique_ptr<Nurse> Hospital::getFreeNurse(){
    for(auto& nurse : nursesList){
        if(nurse->getActivity() == NurseActivity::RESTING){
            auto temp = std::move(nurse);
            nursesList.remove(nurse);
            return temp;
        }
    }
    return nullptr;
}

std::unique_ptr<Doctor> Hospital::getDoctorBySpeciality(DoctorSpecialty specialty){
    for(auto& doctor : doctorsList){
        if(doctor->getSpecialty() == specialty){
            auto temp = std::move(doctor);
            doctorsList.remove(doctor);
            return temp;
        }
    }
    return nullptr;
}

bool Hospital::checkIfFreeDoctorExists(){
    for(auto& doctor : doctorsList){
        if(doctor->getActivity() == DoctorActivity::RESTING){
            return true;
        }
    }
    return false;
}

//it has to check if there is at least one doctor with given specialty, anasthesiologist and one nurse
bool Hospital::checkIfStaffReadyForOperation(DoctorSpecialty specialty){
    bool doctorExists = false;
    bool anasthesiologistExists = false;
    bool nurseExists = false;
    for(auto& doctor : doctorsList){
        if(doctor->getSpecialty() == specialty){
            doctorExists = true;
        }
        if(doctor->getSpecialty() == DoctorSpecialty::ANESTESIOLOGYST){
            anasthesiologistExists = true;
        }
    }
    for(auto& nurse : nursesList){
        if(nurse->getActivity() == NurseActivity::RESTING){
            nurseExists = true;
        }
    }
    return (doctorExists && anasthesiologistExists && nurseExists);
}
