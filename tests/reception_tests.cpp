#include "catch_amalgamated.hpp"
#include "../reception.h"

TEST_CASE("reception tests", "[reception]")
{
    Reception reception("reception");
    reception.getID() == "reception";
    CHECK(reception.getPatientsQueue().empty());
    CHECK(reception.getServiceDataBase().getMedicalServices().empty());

    SECTION("add patient to queue first")
    {
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
    }
    SECTION("add multiple patients to the front of the queue") {
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
        CHECK(reception.getPatientsQueue().back()->getPESEL() == "11111111111");
        CHECK(reception.getPatientsQueue().front()->getHealthCard().getDiseases().size() == 1);
        CHECK(reception.getPatientsQueue().front()->getHealthCard().getDiseases() == std::vector<Diseases>{Diseases::HEART_ATTACK});
        CHECK(reception.getPatientsQueue().back()->getHealthCard().getDiseases().size() == 1);
        CHECK(reception.getPatientsQueue().back()->getHealthCard().getDiseases() == std::vector<Diseases>{Diseases::FLU});
        CHECK(reception.getPatientsQueue()[1]->getHealthCard().getDiseases().size() == 1);
        CHECK(reception.getPatientsQueue()[1]->getHealthCard().getDiseases() == std::vector<Diseases>{Diseases::BRAIN_TUMOR});
    }
    SECTION("add multiple patients to the back of the queue"){
        std::unique_ptr<Patient> patient1 = std::make_unique<Patient>("11111111111", "John", "Doe", Sex::male, 1);
        std::unique_ptr<Patient> patient2 = std::make_unique<Patient>("22222222222", "Jane", "Smith", Sex::male, 2);
        std::unique_ptr<Patient> patient3 = std::make_unique<Patient>("33333333333", "Alice", "Johnson", Sex::male, 3);

        patient1->getHealthCard().addDisease(Diseases::FLU);
        patient2->getHealthCard().addDisease(Diseases::BRAIN_TUMOR);
        patient3->getHealthCard().addDisease(Diseases::HEART_ATTACK);

        reception.addPatientToQueueLast(std::move(patient1));
        reception.addPatientToQueueLast(std::move(patient2));
        reception.addPatientToQueueLast(std::move(patient3));

        CHECK(reception.getPatientsQueue().size() == 3);
        CHECK(reception.getPatientsQueue().back()->getPESEL() == "33333333333");
        CHECK(reception.getPatientsQueue().front()->getPESEL() == "11111111111");
        CHECK(reception.getPatientsQueue().front()->getHealthCard().getDiseases().size() == 1);
        CHECK(reception.getPatientsQueue().front()->getHealthCard().getDiseases() == std::vector<Diseases>{Diseases::FLU});
        CHECK(reception.getPatientsQueue().back()->getHealthCard().getDiseases().size() == 1);
        CHECK(reception.getPatientsQueue().back()->getHealthCard().getDiseases() == std::vector<Diseases>{Diseases::HEART_ATTACK});
        CHECK(reception.getPatientsQueue()[1]->getHealthCard().getDiseases().size() == 1);
        CHECK(reception.getPatientsQueue()[1]->getHealthCard().getDiseases() == std::vector<Diseases>{Diseases::BRAIN_TUMOR});
    }
}
