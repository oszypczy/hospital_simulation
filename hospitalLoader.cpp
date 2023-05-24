#include "hospitalLoader.h"
#include "doctor.h"
#include "nurse.h"
#include "paramedic.h"
#include "reception.h"
#include "ambulanceDispatch.h"


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


    std::list<std::unique_ptr<Human>> humansList;

    for(const auto& doctor : humans["doctors"]){
        std::string PESEL = doctor["PESEL"].asString();
        std::string name = doctor["name"].asString();
        std::string surname = doctor["surname"].asString();
        Sex sex = static_cast<Sex>(doctor["sex"].asUInt());
        ushort age = doctor["age"].asUInt();
        DoctorSpecialty specialty = static_cast<DoctorSpecialty>(doctor["specialty"].asUInt());

        humansList.push_back(std::make_unique<Doctor>(PESEL, name, surname, sex, age, specialty));
    }

    for(const auto& nurse : humans["nurses"]){
        std::string PESEL = nurse["PESEL"].asString();
        std::string name = nurse["name"].asString();
        std::string surname = nurse["surname"].asString();
        Sex sex = static_cast<Sex>(nurse["sex"].asUInt());
        ushort age = nurse["age"].asUInt();

        humansList.push_back(std::make_unique<Nurse>(PESEL, name, surname, sex, age));
    }

    for(const auto& paramedic : humans["paramedics"]){
        std::string PESEL = paramedic["PESEL"].asString();
        std::string name = paramedic["name"].asString();
        std::string surname = paramedic["surname"].asString();
        Sex sex = static_cast<Sex>(paramedic["sex"].asUInt());
        ushort age = paramedic["age"].asUInt();

        humansList.push_back(std::make_unique<Paramedic>(PESEL, name, surname, sex, age));
    }


    std::list<std::unique_ptr<Place>> placesList;

    for (const auto& ward : places["wards"]) {
        std::string wardId = ward["id"].asString();
        std::string wardName = ward["name"].asString();

        std::list<std::unique_ptr<Room>> roomsList;

        for (const auto& generalRoom : ward["generalrooms"]) {
            std::string generalRoomId = generalRoom["id"].asString();
            ushort maxBeds = generalRoom["maxBeds"].asUInt();

            roomsList.push_back(std::make_unique<GeneralRoom>(generalRoomId, maxBeds));
        }

        for (const auto& treatmentRoom : ward["treatmentrooms"]) {
            std::string treatmentRoomId = treatmentRoom["id"].asString();

            roomsList.push_back(std::make_unique<TreatmentRoom>(treatmentRoomId));
        }

        placesList.push_back(std::make_unique<Ward>(wardId, wardName, std::move(roomsList)));
    }

    std::string receptionId = places["reception"]["id"].asString();

    std::string ambulanceDispatchId = places["ambulancedispatch"]["id"].asString();


    Hospital hospital(hospitalName, std::move(placesList), std::move(humansList),
                      std::make_unique<Reception>(receptionId),
                      std::make_unique<AmbulanceDispatch>(ambulanceDispatchId));

    return hospital;
}
