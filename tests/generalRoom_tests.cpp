#include "catch_amalgamated.hpp"
#include "../generalRoom.h"

TEST_CASE("general room simple tests", "[generalRoom]")
{
    GeneralRoom generalRoom("Sala ogolna", 10);

    CHECK(generalRoom.getID() == "Sala ogolna");
    CHECK(generalRoom.checkOccupancy() == 0);
    CHECK(generalRoom.isFull() == false);

    SECTION("add patient"){
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("12345678901", "Jan", "Kowalski", Sex::male, 1);
        generalRoom.addPatient(std::move(patient));
        CHECK(generalRoom.checkOccupancy() == 1);
        CHECK(generalRoom.isFull() == false);
        CHECK(generalRoom.checkIfPatientAssigned("12345678901") == true);
        CHECK(generalRoom.checkIfPatientAssigned("11223344556") == false);
        CHECK(generalRoom.getPatientsList().size() == 1);
        CHECK(generalRoom.getPatientsPESELList().size() == 1);
    }
    SECTION("remove patient")
    {
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("12345678901", "Jan", "Kowalski", Sex::male, 1);
        generalRoom.addPatient(std::move(patient));
        CHECK(generalRoom.checkOccupancy() == 1);
        CHECK(generalRoom.isFull() == false);
        CHECK(generalRoom.checkIfPatientAssigned("12345678901") == true);
    }
    SECTION("full room")
    {
        for (int i = 0; i < 10; i++)
        {
            std::unique_ptr<Patient> patient = std::make_unique<Patient>("12345678901", "Jan", "Kowalski", Sex::male, 1);
            generalRoom.addPatient(std::move(patient));
        }
        CHECK(generalRoom.checkOccupancy() == 10);
        CHECK(generalRoom.isFull() == true);
        CHECK(generalRoom.checkIfPatientAssigned("12345678901") == true);
        CHECK(generalRoom.checkIfPatientAssigned("11223344556") == false);
        CHECK(generalRoom.getPatientsList().size() == 10);
        CHECK(generalRoom.getPatientsPESELList().size() == 10);
    }
    SECTION("get lists")
    {
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("12345678901", "Jan", "Kowalski", Sex::male, 1);
        generalRoom.addPatient(std::move(patient));
        CHECK(generalRoom.getPatientsList().size() == 1);
        CHECK(generalRoom.getPatientsPESELList().size() == 1);
        CHECK(generalRoom.getPatientsList().front()->getPESEL() == "12345678901");
        CHECK(generalRoom.getPatientsPESELList().front() == "12345678901");

        std::unique_ptr<Patient> patient2 = std::make_unique<Patient>("12345678902", "Jan", "Kowalski", Sex::male, 1);
        generalRoom.addPatient(std::move(patient2));
        CHECK(generalRoom.getPatientsPESELList().size() == 2);
        CHECK(generalRoom.getPatientsPESELList() == std::list<std::string>{"12345678901", "12345678902"});

        std::unique_ptr<Patient> patient3 = std::make_unique<Patient>("12345678903", "Jan", "Kowalski", Sex::male, 1);
        generalRoom.addPatient(std::move(patient3));
        CHECK(generalRoom.getPatientsPESELList().size() == 3);
        CHECK(generalRoom.getPatientsPESELList() == std::list<std::string>{"12345678901", "12345678902", "12345678903"});
    }
}
