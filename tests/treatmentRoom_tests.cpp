#include "catch_amalgamated.hpp"
#include "../treatmentRoom.h"

TEST_CASE("treatment room simple tests", "[treatmentRoom]")
{
    TreatmentRoom treatmentRoom("TreatmentRoom 1");
    CHECK(treatmentRoom.getID() == "TreatmentRoom 1");

    SECTION("add patient"){
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("12345678901", "Jan", "Kowalski", Sex::male, 1);
        treatmentRoom.addPatientToQueue(std::move(patient));
        CHECK(treatmentRoom.checkIfAnyoneInQueue());
        CHECK(treatmentRoom.getFirstPatientInQueue()->getPESEL() == "12345678901");
    }
    SECTION("Add multiple patients to the queue") {
        std::unique_ptr<Patient> patient1 = std::make_unique<Patient>("11111111111", "Jacek", "Placek", Sex::male, 1);
        std::unique_ptr<Patient> patient2 = std::make_unique<Patient>("22222222222", "Janina", "Tak", Sex::male, 2);
        std::unique_ptr<Patient> patient3 = std::make_unique<Patient>("33333333333", "Alicja", "Nie", Sex::male, 3);

        treatmentRoom.addPatientToQueue(std::move(patient1));
        treatmentRoom.addPatientToQueue(std::move(patient2));
        treatmentRoom.addPatientToQueue(std::move(patient3));

        CHECK(treatmentRoom.checkIfAnyoneInQueue());
        auto patient = treatmentRoom.getFirstPatientInQueue();
        CHECK(patient->getPESEL() == "11111111111");
        CHECK(patient->getName() == "Jacek");
        CHECK(patient->getSurname() == "Placek");
        CHECK(patient->getAge() == 1);
        CHECK(patient->getSex() == Sex::male);

        patient = treatmentRoom.getFirstPatientInQueue();
        CHECK(patient->getPESEL() == "22222222222");
        CHECK(patient->getName() == "Janina");
        CHECK(patient->getSurname() == "Tak");
        CHECK(patient->getAge() == 2);

        patient = treatmentRoom.getFirstPatientInQueue();
        CHECK(patient->getPESEL() == "33333333333");
        CHECK(patient->getName() == "Alicja");
        CHECK(patient->getSurname() == "Nie");
        CHECK(patient->getAge() == 3);
    }
    SECTION("id managing"){
        CHECK(treatmentRoom.getCurrentServiceID() == -1);
        treatmentRoom.setCurrentServiceID(10);
        CHECK(treatmentRoom.getCurrentServiceID() == 10);
    }
}
