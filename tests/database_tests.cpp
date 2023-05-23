#include "catch_amalgamated.hpp"
#include "../serviceDataBase.h"

TEST_CASE("serviceDataBase simple tests", "[serviceDataBase]")
{
    ServiceDataBase serviceDataBase;
    serviceDataBase.addConsultation(12, 60, false);
    CHECK((*serviceDataBase.getServiceByID(12))->getServiceState() == ServiceState::NOT_READY);
    CHECK((*serviceDataBase.getServiceByID(12))->getProgressTime() == 0);
    CHECK((*serviceDataBase.getServiceByID(12))->getID() == 12);
    serviceDataBase.addOperation(25, 120, true, OperationType::HEART_TRANSPLANT);
    CHECK((*serviceDataBase.getServiceByID(25))->getServiceState() == ServiceState::NOT_READY);
    CHECK((*serviceDataBase.getServiceByID(25))->getProgressTime() == 0);
    CHECK((*serviceDataBase.getServiceByID(25))->getID() == 25);

    SECTION("testing virtual function calculating total cost and printing out")
    {
        CHECK(serviceDataBase.calculateTotalCost() == 18000);
        CHECK((*serviceDataBase.getServiceByID(25))->calculateCost() == 0);
        CHECK((*serviceDataBase.getServiceByID(12))->calculateCost() == 18000);
        std::stringstream ss;
        ss << serviceDataBase;
        CHECK(ss.str() == "--------------------------------------------\nConsultation ID: 12\nCost of consultation: 180.00 zł\nTime of the consultation left: 60min\nNFZ: 0\n--------------------------------------------\nOperation ID: 25\nCost of Operation: 0.00 zł\nTime of the Operation left: 120min\nNFZ: 1\n");
    }

    SECTION("testing removing from database")
    {
        CHECK(serviceDataBase.calculateTotalCost() == 18000);
        serviceDataBase.removeService(12);
        CHECK(serviceDataBase.calculateTotalCost() == 0);
    }

    SECTION("testing adding to database service that already exist")
    {
        CHECK_THROWS_MATCHES(serviceDataBase.addConsultation(12, 120, true), std::logic_error, Catch::Matchers::Message("Consultation with given ID already exists!"));
        CHECK_THROWS_MATCHES(serviceDataBase.addOperation(25, 120, true, OperationType::HEART_TRANSPLANT), std::logic_error, Catch::Matchers::Message("Operation with given ID already exists!"));
    }

    SECTION("removing service that does not exist")
    {
        CHECK_THROWS_MATCHES(serviceDataBase.removeService(100), std::logic_error, Catch::Matchers::Message("Service with given ID not found!"));
    }

    SECTION("testing adding personel and patient to service")
    {
        std::unique_ptr<Doctor> doctor = std::make_unique<Doctor>("03270607850", "Jan", "Kowalski", Sex::male, 29, DoctorSpecialty::GENERAL);
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("03232407362", "Lidia", "Strzelecka", Sex::female, 20);
        (*serviceDataBase.getServiceByID(12))->addDoctor(std::move(doctor));
        CHECK((*serviceDataBase.getServiceByID(12))->getDoctor("03270607850").getName() ==  "Jan");
        (*serviceDataBase.getServiceByID(12))->startService(std::move(patient));
        CHECK((*serviceDataBase.getServiceByID(12))->getPatient().getName() == "Lidia");
    }

    SECTION("testing running full operation")
    {
        std::unique_ptr<Doctor> doctor1 = std::make_unique<Doctor>("03270607850", "Jan", "Kowalski", Sex::male, 29, DoctorSpecialty::CARDIOLOGIST);
        std::unique_ptr<Doctor> doctor2 = std::make_unique<Doctor>("09876534879", "Konrad", "Nowak", Sex::other, 56, DoctorSpecialty::ANESTESIOLOGYST);
        std::unique_ptr<Nurse> nurse = std::make_unique<Nurse>("03287607850", "Ania", "Kotas", Sex::female, 19);
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("03232407362", "Lidia", "Strzelecka", Sex::female, 20);
        (*serviceDataBase.getServiceByID(25))->addDoctor(std::move(doctor1));
        (*serviceDataBase.getServiceByID(25))->addDoctor(std::move(doctor2));
        (*serviceDataBase.getServiceByID(25))->addNurse(std::move(nurse));
        (*serviceDataBase.getServiceByID(25))->startService(std::move(patient));
        (*serviceDataBase.getServiceByID(25))->continueService();
        CHECK((*serviceDataBase.getServiceByID(25))->checkPersonel());
        (*serviceDataBase.getServiceByID(25))->continueService();
        CHECK((*serviceDataBase.getServiceByID(25))->getServiceState() == ServiceState::IN_PROGRESS);
        (*serviceDataBase.getServiceByID(25))->continueService();
        CHECK((*serviceDataBase.getServiceByID(25))->getProgressTime() == 60);
        (*serviceDataBase.getServiceByID(25))->continueService();
        (*serviceDataBase.getServiceByID(25))->continueService();
        (*serviceDataBase.getServiceByID(25))->continueService();
        (*serviceDataBase.getServiceByID(25))->continueService();
        CHECK((*serviceDataBase.getServiceByID(25))->getServiceState() == ServiceState::FINISHED);
        CHECK((*serviceDataBase.getServiceByID(25))->getPatient().getState() == PatientState::RESTING);
        CHECK((*serviceDataBase.getServiceByID(25))->getDoctor("03270607850").getActivity() == DoctorActivity::RESTING);
        CHECK((*serviceDataBase.getServiceByID(25))->getDoctor("09876534879").getActivity() == DoctorActivity::RESTING);
        CHECK((*serviceDataBase.getServiceByID(25))->getNurse("03287607850").getActivity() == NurseActivity::RESTING);
    }
}
