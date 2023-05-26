#include "hospitalLoader.h"
#include "doctor.h"
#include "nurse.h"
#include "paramedic.h"
#include "reception.h"
#include "ambulanceDispatch.h"
#include "ward.h"
#include "treatmentRoom.h"
#include "consultationRoom.h"


HospitalLoader::HospitalLoader(const std::string& jsonFile) : jsonFile(jsonFile) {}


void HospitalLoader::loadJsonFile(Json::Value& val){
    std::ifstream file(jsonFile);
    if (!file.is_open()) {
        throw std::runtime_error("File not found");
    }

    Json::Reader reader;
    if (!reader.parse(file, val)) {
        throw std::runtime_error("Failed to parse JSON file");
    }
}


Hospital HospitalLoader::loadHospital(){
    Json::Value val;
    Json::Reader reader;

    loadJsonFile(val);

    std::string hospitalName = val["hospital"]["name"].asString();

    const Json::Value& humans = val["hospital"]["humans"];
    const Json::Value& places = val["hospital"]["places"];


    std::list<std::unique_ptr<Doctor>> doctorsList;

    for(const auto& doctor : humans["doctors"]){
        std::string PESEL = doctor["PESEL"].asString();
        std::string name = doctor["name"].asString();
        std::string surname = doctor["surname"].asString();
        Sex sex = static_cast<Sex>(doctor["sex"].asUInt());
        ushort age = doctor["age"].asUInt();
        DoctorSpecialty specialty = static_cast<DoctorSpecialty>(doctor["specialty"].asUInt());

        doctorsList.push_back(std::make_unique<Doctor>(PESEL, name, surname, sex, age, specialty));
    }

    std::list<std::unique_ptr<Nurse>> nursesList;

    for(const auto& nurse : humans["nurses"]){
        std::string PESEL = nurse["PESEL"].asString();
        std::string name = nurse["name"].asString();
        std::string surname = nurse["surname"].asString();
        Sex sex = static_cast<Sex>(nurse["sex"].asUInt());
        ushort age = nurse["age"].asUInt();

        nursesList.push_back(std::make_unique<Nurse>(PESEL, name, surname, sex, age));
    }




    std::list<std::unique_ptr<Ward>> wardsList;

    for (const auto& ward : places["wards"]) {
        std::string wardId = ward["id"].asString();
        std::string wardName = ward["name"].asString();

        auto treatmentRoom = std::make_unique<TreatmentRoom>(ward["treatmentroom"]["id"].asString());
        auto consultationRoom = std::make_unique<ConsultationRoom>(ward["consultationroom"]["id"].asString());


        std::list<std::unique_ptr<Room>> generalRoomList;

        for (const auto& generalRoom : ward["generalrooms"]) {
            std::string generalRoomId = generalRoom["id"].asString();
            ushort maxBeds = generalRoom["maxBeds"].asUInt();

            generalRoomList.push_back(std::make_unique<GeneralRoom>(generalRoomId, maxBeds));
        }


        wardsList.push_back(std::make_unique<Ward>(wardId, wardName, std::move(treatmentRoom),
                            std::move(consultationRoom), std::move(generalRoomList)));
    }

    std::string receptionID = places["reception"]["id"].asString();
    auto reception = std::make_unique<Reception>(receptionID);

    // tu jakos trzeba dodac pierwsza pielegniarke do recepcji

    std::string ambulanceDispatchId = places["ambulancedispatch"]["id"].asString();
    auto ambulanceDispatch = std::make_unique<AmbulanceDispatch>(ambulanceDispatchId);


    for(const auto& ambulance : places["ambulancedispatch"]["ambulances"]){
        std::string ambulanceID = ambulance["registrationNumber"].asString();

        ambulanceDispatch->addAmbulance(std::make_unique<Ambulance>(ambulanceID));
    }

    for(const auto& paramedic : places["ambulancedispatch"]["paramedics"]){
        std::string PESEL = paramedic["PESEL"].asString();
        std::string name = paramedic["name"].asString();
        std::string surname = paramedic["surname"].asString();
        Sex sex = static_cast<Sex>(paramedic["sex"].asUInt());
        ushort age = paramedic["age"].asUInt();

        ambulanceDispatch->addParamedic(std::make_unique<Paramedic>(PESEL, name, surname, sex, age));
    }



    Hospital hospital(hospitalName, std::move(wardsList), std::move(doctorsList), std::move(nursesList),
                      std::move(reception), std::move(ambulanceDispatch));


    return hospital;
}
