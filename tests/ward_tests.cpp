#include "catch_amalgamated.hpp"
#include "../ward.h"

TEST_CASE("ward tests", "[ward]")
{
    std::unique_ptr<TreatmentRoom> treatmentRoom = std::make_unique<TreatmentRoom>("TreatmentRoom 1");
    std::unique_ptr<ConsultationRoom> consultationRoom = std::make_unique<ConsultationRoom>("ConsultationRoom 1");
    std::list<std::unique_ptr<GeneralRoom>> generalRoomList;
    generalRoomList.push_back(std::make_unique<GeneralRoom>("GeneralRoom 1", 5));
    generalRoomList.push_back(std::make_unique<GeneralRoom>("GeneralRoom 2", 7));
    generalRoomList.push_back(std::make_unique<GeneralRoom>("GeneralRoom 3", 3));

    Ward ward("Ward 1", "Ward 1", std::move(treatmentRoom), std::move(consultationRoom), std::move(generalRoomList));
    CHECK(ward.getID() == "Ward 1");
    CHECK(ward.getName() == "Ward 1");

    SECTION("get treatment room"){
        CHECK(ward.getTreatmentRoom()->getID() == "TreatmentRoom 1");
        auto patient = std::make_unique<Patient>("12345679802", "Jacek", "Wacek", Sex::male, 55);
        ward.getTreatmentRoom()->addPatientToQueue(std::move(patient));
        CHECK(ward.getTreatmentRoom()->checkIfAnyoneInQueue());
        auto patient2 = ward.getTreatmentRoom()->getFirstPatientInQueue();
        CHECK_FALSE(ward.getTreatmentRoom()->checkIfAnyoneInQueue());
        CHECK(patient2->getPESEL() == "12345679802");
        CHECK(patient2->getName() == "Jacek");
        CHECK(patient2->getSurname() == "Wacek");
        CHECK(patient2->getAge() == 55);
        CHECK(patient2->getSex() == Sex::male);
    }
    SECTION("get consultation room"){
        CHECK(ward.getConsultationRoom()->getID() == "ConsultationRoom 1");
        auto patient = std::make_unique<Patient>("12345679801", "Jacek", "Placek", Sex::male, 55);
        ward.getConsultationRoom()->addPatientToQueue(std::move(patient));
        CHECK(ward.getConsultationRoom()->checkIfAnyoneInQueue());
        auto patient2 = ward.getConsultationRoom()->getFirstPatientInQueue();
        CHECK_FALSE(ward.getConsultationRoom()->checkIfAnyoneInQueue());
        CHECK(patient2->getPESEL() == "12345679801");
        CHECK(patient2->getName() == "Jacek");
        CHECK(patient2->getSurname() == "Placek");
        CHECK(patient2->getAge() == 55);
        CHECK(patient2->getSex() == Sex::male);
    }
    SECTION("get general room list"){
        CHECK(ward.getGeneralRoomList().size() == 3);
        CHECK(ward.getGeneralRoomList().front()->getID() == "GeneralRoom 1");
        CHECK(ward.getGeneralRoomList().back()->getID() == "GeneralRoom 3");
    }
}
