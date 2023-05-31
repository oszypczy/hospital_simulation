#include "catch_amalgamated.hpp"
#include "../hospital.h"

TEST_CASE("hospital simple tests", "[hospital]")
{
    std::list<std::unique_ptr<Ward>> wardList;
    std::list<std::unique_ptr<Doctor>> doctorsList;
    std::list<std::unique_ptr<Nurse>> nursesList;
    std::unique_ptr<Reception> reception = std::make_unique<Reception>("reception");
    std::unique_ptr<AmbulanceDispatch> ambulanceDispatch = std::make_unique<AmbulanceDispatch>("ambulanceDispatch");

    Hospital hospital("Szpital", std::move(wardList), std::move(doctorsList), std::move(nursesList), std::move(reception), std::move(ambulanceDispatch));

    SECTION("constructor exceptions"){
        CHECK_THROWS(Hospital("", std::move(wardList), std::move(doctorsList), std::move(nursesList), std::move(reception), std::move(ambulanceDispatch)));
        wardList.push_back(std::make_unique<Ward>("123", "ward1", std::make_unique<TreatmentRoom>("123"), std::make_unique<ConsultationRoom>("124"), std::list<std::unique_ptr<GeneralRoom>>()));
        wardList.push_back(std::make_unique<Ward>("123", "ward2", std::make_unique<TreatmentRoom>("234"), std::make_unique<ConsultationRoom>("235"), std::list<std::unique_ptr<GeneralRoom>>()));
        CHECK_THROWS(Hospital("Szpital", std::move(wardList), std::move(doctorsList), std::move(nursesList), std::move(reception), std::move(ambulanceDispatch)));
    }
    SECTION("constructor getters"){
        CHECK(hospital.getName() == "Szpital");
        CHECK(hospital.getWardsList().size() == 0);
        CHECK(hospital.getDoctorsList().size() == 0);
        CHECK(hospital.getNursesList().size() == 0);
        CHECK(hospital.getReception()->getID() == "reception");
        CHECK(hospital.getAmbulanceDispatch()->getID() == "ambulanceDispatch");
    }
    SECTION("add doctors"){
        std::unique_ptr<Doctor> doctor1 = std::make_unique<Doctor>("11223344556", "Doktor", "Doktorowski", Sex::male, 55, DoctorSpecialty::GENERAL);
        hospital.addDoctor(std::move(doctor1));
        CHECK(hospital.getDoctorsList().size() == 1);
        CHECK(hospital.getDoctorsList().front()->getPESEL() == "11223344556");
        CHECK(hospital.getDoctorsList().front()->getName() == "Doktor");
        CHECK(hospital.getDoctorsList().front()->getSurname() == "Doktorowski");
        CHECK(hospital.getDoctorsList().front()->getAge() == 55);
        CHECK(hospital.getDoctorsList().front()->getSex() == Sex::male);
        CHECK(hospital.checkIfFreeDoctorExists());
    }
    SECTION("add nurse"){
        std::unique_ptr<Nurse> Lidka = std::make_unique<Nurse>("03232407362", "Lidia", "Strzelecka", Sex::female, 20);
        hospital.addNurse(std::move(Lidka));
        CHECK(hospital.getNursesList().size() == 1);
        CHECK(hospital.getNursesList().front()->getPESEL() == "03232407362");
        CHECK(hospital.getNursesList().front()->getName() == "Lidia");
        CHECK(hospital.getNursesList().front()->getSurname() == "Strzelecka");
        CHECK(hospital.getNursesList().front()->getAge() == 20);
        CHECK(hospital.getNursesList().front()->getSex() == Sex::female);
    }
    SECTION("ambulance dispatch"){
        std::unique_ptr<Ambulance> ambulance = std::make_unique<Ambulance>("WA00001");
        hospital.getAmbulanceDispatch()->addAmbulance(std::move(ambulance));
        auto& dispatch = *hospital.getAmbulanceDispatch();

        CHECK_FALSE((*dispatch.getAmbulance("WA00001"))->checkOccupancy());
        CHECK_FALSE((*dispatch.getAmbulance("WA00001"))->checkPersonel());
        CHECK((*dispatch.getAmbulance("WA00001"))->getTotalTime() == 30);
        CHECK((*dispatch.getAmbulance("WA00001"))->getRegistrationNumber() == "WA00001");
        CHECK((*dispatch.getAmbulance("WA00001"))->getState() == AmbulanceState::IN_GARAGE);

        CHECK_FALSE((*dispatch.getFreeAmbulance())->checkOccupancy());
        CHECK_FALSE((*dispatch.getFreeAmbulance())->checkPersonel());
        CHECK((*dispatch.getFreeAmbulance())->getTotalTime() == 30);
        CHECK((*dispatch.getFreeAmbulance())->getRegistrationNumber() == "WA00001");
        CHECK((*dispatch.getFreeAmbulance())->getState() == AmbulanceState::IN_GARAGE);
    }
    SECTION("reception"){
        auto& reception = *hospital.getReception();
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("12345678901", "Jan", "Kowalski", Sex::male, 19);
        patient->getHealthCard().addDisease(Diseases::FLU);
        reception.addPatientToQueueFirst(std::move(patient));
        CHECK(reception.getPatientsQueue().size() == 1);
        CHECK(reception.getPatientsQueue().front()->getPESEL() == "12345678901");
        CHECK(reception.getPatientsQueue().front()->getName() == "Jan");
        CHECK(reception.getPatientsQueue().front()->getSurname() == "Kowalski");
        CHECK(reception.getPatientsQueue().front()->getAge() == 19);
        CHECK(reception.getPatientsQueue().front()->getSex() == Sex::male);
        CHECK(reception.getPatientsQueue().front()->getHealthCard().getDiseases().size() == 1);
        CHECK(reception.getPatientsQueue().front()->getHealthCard().getDiseases() == std::vector<Diseases>{Diseases::FLU});

        reception.getPatientsQueue().clear();
        std::unique_ptr<Patient> patient1 = std::make_unique<Patient>("11111111111", "John", "Doe", Sex::male, 1);
        std::unique_ptr<Patient> patient2 = std::make_unique<Patient>("22222222222", "Jane", "Smith", Sex::male, 2);
        std::unique_ptr<Patient> patient3 = std::make_unique<Patient>("33333333333", "Alice", "Johnson", Sex::male, 3);

        patient1->getHealthCard().addDisease(Diseases::FLU);
        patient2->getHealthCard().addDisease(Diseases::BRAIN_TUMOR);
        patient3->getHealthCard().addDisease(Diseases::HEART_ATTACK);

        reception.addPatientToQueueFirst(std::move(patient1));
        reception.addPatientToQueueFirst(std::move(patient2));
        reception.addPatientToQueueFirst(std::move(patient3));

        CHECK(reception.getPatientsQueue().size() == 3);
        CHECK(reception.getPatientsQueue().front()->getPESEL() == "33333333333");
        CHECK(reception.getPatientsQueue()[1]->getPESEL() == "22222222222");
        CHECK(reception.getPatientsQueue().back()->getPESEL() == "11111111111");
        CHECK(reception.getPatientsQueue().front()->getHealthCard().getDiseases().size() == 1);
        CHECK(reception.getPatientsQueue().front()->getHealthCard().getDiseases() == std::vector<Diseases>{Diseases::HEART_ATTACK});
        CHECK(reception.getPatientsQueue().back()->getHealthCard().getDiseases().size() == 1);
        CHECK(reception.getPatientsQueue().back()->getHealthCard().getDiseases() == std::vector<Diseases>{Diseases::FLU});
        CHECK(reception.getPatientsQueue()[1]->getHealthCard().getDiseases().size() == 1);
        CHECK(reception.getPatientsQueue()[1]->getHealthCard().getDiseases() == std::vector<Diseases>{Diseases::BRAIN_TUMOR});
    }
}