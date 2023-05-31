#include "catch_amalgamated.hpp"
#include "../consultationRoom.h"

TEST_CASE("consultation room simple tests", "[consultationRoom]")
{
    ConsultationRoom consultationRoom("ConsultationRoom 1");
    CHECK(consultationRoom.getID() == "ConsultationRoom 1");

    SECTION("add patient"){
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("12345678901", "Jan", "Kowalski", Sex::male, 1);
        consultationRoom.addPatientToQueue(std::move(patient));
        CHECK(consultationRoom.checkIfAnyoneInQueue());
        CHECK(consultationRoom.getFirstPatientInQueue()->getPESEL() == "12345678901");
    }
    SECTION("Add multiple patients to the queue") {
        std::unique_ptr<Patient> patient1 = std::make_unique<Patient>("11111111111", "Jacek", "Placek", Sex::male, 1);
        std::unique_ptr<Patient> patient2 = std::make_unique<Patient>("22222222222", "Janina", "Tak", Sex::male, 2);
        std::unique_ptr<Patient> patient3 = std::make_unique<Patient>("33333333333", "Alicja", "Nie", Sex::male, 3);

        consultationRoom.addPatientToQueue(std::move(patient1));
        consultationRoom.addPatientToQueue(std::move(patient2));
        consultationRoom.addPatientToQueue(std::move(patient3));

        CHECK(consultationRoom.checkIfAnyoneInQueue());
        auto patient = consultationRoom.getFirstPatientInQueue();
        CHECK(patient->getPESEL() == "11111111111");
        CHECK(patient->getName() == "Jacek");
        CHECK(patient->getSurname() == "Placek");
        CHECK(patient->getAge() == 1);
        CHECK(patient->getSex() == Sex::male);

        patient = consultationRoom.getFirstPatientInQueue();
        CHECK(patient->getPESEL() == "22222222222");
        CHECK(patient->getName() == "Janina");
        CHECK(patient->getSurname() == "Tak");
        CHECK(patient->getAge() == 2);

        patient = consultationRoom.getFirstPatientInQueue();
        CHECK(patient->getPESEL() == "33333333333");
        CHECK(patient->getName() == "Alicja");
        CHECK(patient->getSurname() == "Nie");
        CHECK(patient->getAge() == 3);
    }
    SECTION("id managing"){
        CHECK(consultationRoom.getCurrentServiceID() == -1);
        consultationRoom.setCurrentServiceID(10);
        CHECK(consultationRoom.getCurrentServiceID() == 10);
    }
}