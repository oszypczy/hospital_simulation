#include "catch_amalgamated.hpp"
#include "../consultation.h"

TEST_CASE("consultation simple tests", "[consultation]")
{
    Consultation consultation(2, 30, false);
    CHECK(consultation.getID() == 2);
    CHECK_FALSE(consultation.checkPersonel());
    CHECK(consultation.getServiceState() == ServiceState::NOT_READY);
    CHECK(consultation.getProgressTime() == 0);
    CHECK_THROWS_MATCHES(consultation.getPatient(), std::logic_error, Catch::Matchers::Message("Patient attribute cannot be empty!"));
    CHECK_THROWS_MATCHES(consultation.getDoctor("12345678901"), std::logic_error, Catch::Matchers::Message("Doctor with PESEL: 12345678901 not found!"));
    CHECK_THROWS_MATCHES(consultation.getNurse("12345678901"), std::logic_error, Catch::Matchers::Message("Nurse with PESEL: 12345678901 not found!"));

    SECTION("prepare and start consultation")
    {
        std::unique_ptr<Doctor> doctor = std::make_unique<Doctor>("03270607850", "Jan", "Kowalski", Sex::male, 29, DoctorSpecialty::GENERAL);
        consultation.addDoctor(std::move(doctor));
        CHECK(consultation.checkPersonel());
        CHECK((*consultation.getDoctor("03270607850"))->getName() == "Jan");
        CHECK((*consultation.getDoctor("03270607850"))->getSurname() == "Kowalski");
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("03232407362", "Lidia", "Strzelecka", Sex::female, 20);
        CHECK(patient->getState() == PatientState::RESTING);
        patient->getHealthCard().planService(consultation.getID());
        consultation.startService(std::move(patient));
        CHECK(consultation.getServiceState() == ServiceState::IN_PROGRESS);
        CHECK(consultation.getProgressTime() == 15);
        CHECK(consultation.getPatient().getName() == "Lidia");
        CHECK(consultation.getPatient().getSurname() == "Strzelecka");
        CHECK(consultation.getPatient().getState() == PatientState::ON_APPOITMENT);
        CHECK((*consultation.getDoctor("03270607850"))->getActivity() == DoctorActivity::CONSULTING);
        CHECK_THROWS_MATCHES(consultation.finishService(), std::logic_error, Catch::Matchers::Message("Invalid service state: IN_PROGRESS. Expected state: FINISHED"));
    }

    SECTION("continue and finish consultation")
    {
        std::unique_ptr<Doctor> doctor = std::make_unique<Doctor>("03270607850", "Jan", "Kowalski", Sex::male, 29, DoctorSpecialty::GENERAL);
        consultation.addDoctor(std::move(doctor));
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("03232407362", "Lidia", "Strzelecka", Sex::female, 20);
        patient->getHealthCard().planService(consultation.getID());
        consultation.startService(std::move(patient));
        CHECK(consultation.getProgressTime() == 15);
        consultation.continueService();
        CHECK(consultation.getProgressTime() == 30);
        CHECK(consultation.getServiceState() == ServiceState::FINISHED);
        CHECK(consultation.getPatient().getState() == PatientState::RESTING);
        CHECK((*consultation.getDoctor("03270607850"))->getActivity() == DoctorActivity::RESTING);
    }

    SECTION("testing different invalid scenarios and returning patient")
    {
        CHECK_FALSE(consultation.checkPersonel());
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("03232407362", "Lidia", "Strzelecka", Sex::female, 20);
        CHECK_THROWS_MATCHES(consultation.startService(std::move(patient)), std::logic_error, Catch::Matchers::Message("Wrong personel for this service!"));
        CHECK_THROWS_MATCHES(consultation.continueService(), std::logic_error, Catch::Matchers::Message("Invalid service state: NOT_READY. Expected state: IN_PROGRESS"));
        patient = consultation.returnPatient();
        CHECK(patient->getName() == "Lidia");
    }

    SECTION("testing cost of consultation")
    {
        std::unique_ptr<Doctor> doctor = std::make_unique<Doctor>("03270607850", "Jan", "Kowalski", Sex::male, 29, DoctorSpecialty::GENERAL);
        consultation.addDoctor(std::move(doctor));
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("03232407362", "Lidia", "Strzelecka", Sex::female, 20);
        patient->getHealthCard().planService(consultation.getID());
        consultation.startService(std::move(patient));
        CHECK(consultation.calculateCost() == 23976);
        std::stringstream ss;
        ss << consultation;
        CHECK(ss.str() == "Patient: Lidia Strzelecka (03232407362), 20 years old.\nMedical personel: General doctor Jan Kowalski (03270607850).\nConsultation ID: 2\nCost of consultation: 239.76 zł\nTime of the consultation left: 15min\nNFZ: 0");
    }

    SECTION("testing correct personel however wrong patient")
    {
        std::unique_ptr<Doctor> doctor = std::make_unique<Doctor>("03270607850", "Jan", "Kowalski", Sex::male, 29, DoctorSpecialty::GENERAL);
        consultation.addDoctor(std::move(doctor));
        CHECK(consultation.checkPersonel());
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("03232407362", "Lidia", "Strzelecka", Sex::female, 20);
        CHECK_THROWS_MATCHES(consultation.startService(std::move(patient)), std::logic_error, Catch::Matchers::Message("Patient's health card does not match this service!"));
    }
}
