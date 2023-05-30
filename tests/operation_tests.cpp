#include "catch_amalgamated.hpp"
#include "../operation.h"

TEST_CASE("operation simple tests", "[operation]")
{
    Operation operation(101, 140, false, OperationType::BRAIN_TUMOR_REMOVAL, Diseases::BRAIN_TUMOR);
    CHECK(operation.getID() == 101);
    CHECK_FALSE(operation.checkPersonel());
    CHECK(operation.getServiceState() == ServiceState::NOT_READY);
    CHECK(operation.getProgressTime() == 0);
    CHECK_THROWS_MATCHES(operation.getPatient(), std::logic_error, Catch::Matchers::Message("Patient attribute cannot be empty!"));
    CHECK_THROWS_MATCHES(operation.getDoctor("12345678901"), std::logic_error, Catch::Matchers::Message("Doctor with PESEL: 12345678901 not found!"));
    CHECK_THROWS_MATCHES(operation.getNurse("12345678901"), std::logic_error, Catch::Matchers::Message("Nurse with PESEL: 12345678901 not found!"));

    SECTION("prepare and start operation")
    {
        std::unique_ptr<Doctor> doctor1 = std::make_unique<Doctor>("03270607850", "Jan", "Kowalski", Sex::male, 29, DoctorSpecialty::NEUROLOGIST);
        operation.addDoctor(std::move(doctor1));
        std::unique_ptr<Doctor> doctor2 = std::make_unique<Doctor>("09876534879", "Konrad", "Nowak", Sex::other, 56, DoctorSpecialty::ANESTESIOLOGYST);
        operation.addDoctor(std::move(doctor2));
        std::unique_ptr<Nurse> nurse = std::make_unique<Nurse>("03287607850", "Ania", "Kotas", Sex::female, 19);
        operation.addNurse(std::move(nurse));
        CHECK(operation.checkPersonel());
        CHECK(operation.getDoctor(DoctorSpecialty::ANESTESIOLOGYST).getName() == "Konrad");
        CHECK(operation.getDoctor(DoctorSpecialty::NEUROLOGIST).getName() == "Jan");
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("03232407362", "Lidia", "Strzelecka", Sex::female, 20);
        patient->getHealthCard().addDisease(Diseases::BRAIN_TUMOR);
        patient->getHealthCard().planService(101);
        CHECK(patient->getState() == PatientState::RESTING);
        operation.startService(std::move(patient));
        CHECK(operation.getServiceState() == ServiceState::IN_PROGRESS);
        CHECK(operation.getProgressTime() == 15);
        CHECK(operation.getPatient().getName() == "Lidia");
        CHECK(operation.getPatient().getSurname() == "Strzelecka");
        CHECK(operation.getPatient().getState() == PatientState::IN_OR);
        CHECK((*operation.getDoctor("03270607850"))->getActivity() == DoctorActivity::OPERATING);
        CHECK((*operation.getDoctor("09876534879"))->getActivity() == DoctorActivity::OPERATING);
        CHECK((*operation.getNurse("03287607850"))->getActivity() == NurseActivity::OPERATION_ASSISTANCE);
        CHECK_THROWS_MATCHES(operation.finishService(), std::logic_error, Catch::Matchers::Message("Invalid service state: IN_PROGRESS. Expected state: FINISHED"));
    }

    SECTION("continue and finish operation")
    {
        std::unique_ptr<Doctor> doctor1 = std::make_unique<Doctor>("03270607850", "Jan", "Kowalski", Sex::male, 29, DoctorSpecialty::NEUROLOGIST);
        operation.addDoctor(std::move(doctor1));
        std::unique_ptr<Doctor> doctor2 = std::make_unique<Doctor>("09876534879", "Konrad", "Nowak", Sex::other, 56, DoctorSpecialty::ANESTESIOLOGYST);
        operation.addDoctor(std::move(doctor2));
        std::unique_ptr<Nurse> nurse = std::make_unique<Nurse>("03287607850", "Ania", "Kotas", Sex::female, 19);
        operation.addNurse(std::move(nurse));
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("03232407362", "Lidia", "Strzelecka", Sex::female, 20);
        patient->getHealthCard().addDisease(Diseases::BRAIN_TUMOR);
        patient->getHealthCard().planService(101);
        operation.startService(std::move(patient));
        CHECK(operation.getProgressTime() == 15);
        operation.continueService();
        CHECK(operation.getProgressTime() == 30);
        operation.continueService();
        CHECK(operation.getProgressTime() == 45);
        operation.continueService();
        CHECK(operation.getProgressTime() == 60);
        operation.continueService();
        CHECK(operation.getProgressTime() == 75);
        operation.continueService();
        CHECK(operation.getProgressTime() == 90);
        operation.continueService();
        CHECK(operation.getProgressTime() == 105);
        operation.continueService();
        CHECK(operation.getProgressTime() == 120);
        operation.continueService();
        CHECK(operation.getProgressTime() == 135);
        operation.continueService();
        CHECK(operation.getProgressTime() == 140);
        CHECK(operation.getServiceState() == ServiceState::FINISHED);
        CHECK(operation.getPatient().getState() == PatientState::RESTING);
        CHECK((*operation.getDoctor("03270607850"))->getActivity() == DoctorActivity::RESTING);
        CHECK((*operation.getDoctor("09876534879"))->getActivity() == DoctorActivity::RESTING);
        CHECK((*operation.getNurse("03287607850"))->getActivity() == NurseActivity::RESTING);
        CHECK_FALSE(operation.getPatient().getHealthCard().checkDisease(Diseases::BRAIN_TUMOR));
    }

    SECTION("testing different invalid scenarios")
    {
        CHECK_FALSE(operation.checkPersonel());
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("03232407362", "Lidia", "Strzelecka", Sex::female, 20);
        CHECK_THROWS_MATCHES(operation.startService(std::move(patient)), std::logic_error, Catch::Matchers::Message("Wrong personel for this service!"));
        CHECK_THROWS_MATCHES(operation.continueService(), std::logic_error, Catch::Matchers::Message("Invalid service state: NOT_READY. Expected state: IN_PROGRESS"));
        std::unique_ptr<Doctor> doctor1 = std::make_unique<Doctor>("03270607850", "Jan", "Kowalski", Sex::male, 29, DoctorSpecialty::NEUROLOGIST);
        operation.addDoctor(std::move(doctor1));
        CHECK_THROWS_MATCHES(operation.startService(std::move(patient)), std::logic_error, Catch::Matchers::Message("Wrong personel for this service!"));
        std::unique_ptr<Doctor> doctor2 = std::make_unique<Doctor>("09876534879", "Konrad", "Nowak", Sex::other, 56, DoctorSpecialty::GYNECOLOGYST);
        operation.addDoctor(std::move(doctor2));
        CHECK_THROWS_MATCHES(operation.startService(std::move(patient)), std::logic_error, Catch::Matchers::Message("Wrong personel for this service!"));
    }

    SECTION("testing cost of operation")
    {
        std::unique_ptr<Doctor> doctor1 = std::make_unique<Doctor>("03270607850", "Jan", "Kowalski", Sex::male, 29, DoctorSpecialty::NEUROLOGIST);
        operation.addDoctor(std::move(doctor1));
        std::unique_ptr<Doctor> doctor2 = std::make_unique<Doctor>("09876534879", "Konrad", "Nowak", Sex::other, 56, DoctorSpecialty::ANESTESIOLOGYST);
        operation.addDoctor(std::move(doctor2));
        std::unique_ptr<Nurse> nurse = std::make_unique<Nurse>("03287607850", "Ania", "Kotas", Sex::female, 19);
        operation.addNurse(std::move(nurse));
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("03232407362", "Lidia", "Strzelecka", Sex::female, 20);
        patient->getHealthCard().addDisease(Diseases::BRAIN_TUMOR);
        patient->getHealthCard().planService(101);
        operation.startService(std::move(patient));
        CHECK(operation.calculateCost() == 257070);
        std::stringstream ss;
        ss << operation;
        CHECK(ss.str() == "Patient: Lidia Strzelecka (03232407362), 20 years old\nMedical personel: Neurology doctor Jan Kowalski (03270607850)\nAnesthesiology doctor Konrad Nowak (09876534879)\nNurse: Ania Kotas (03287607850)\nOperation ID: 101\nCost of Operation: 2570.70 zł\nTime of the Operation left: 125min\nNFZ: 0");
    }

    SECTION("testing adding and returning staff/pateint from service")
    {
        std::unique_ptr<Doctor> doctor1 = std::make_unique<Doctor>("03270607850", "Jan", "Kowalski", Sex::male, 29, DoctorSpecialty::NEUROLOGIST);
        std::unique_ptr<Doctor> doctor2 = std::make_unique<Doctor>("09876534879", "Konrad", "Nowak", Sex::other, 56, DoctorSpecialty::ANESTESIOLOGYST);
        std::unique_ptr<Doctor> doctor3 = std::make_unique<Doctor>("09876534999", "Jakub", "Szuminski", Sex::other, 48, DoctorSpecialty::ANESTESIOLOGYST, DoctorActivity::OPERATING);
        std::unique_ptr<Nurse> nurse1 = std::make_unique<Nurse>("03287607850", "Ania", "Kotas", Sex::female, 19);
        std::unique_ptr<Nurse> nurse2 = std::make_unique<Nurse>("03287607854", "Ola", "Kapot", Sex::female, 20, NurseActivity::RECEPTION_SERVICE);
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("03232407362", "Lidia", "Strzelecka", Sex::female, 20);
        operation.addDoctor(std::move(doctor1));
        CHECK((*operation.getDoctor("03270607850"))->getName() == "Jan");
        operation.addDoctor(std::move(doctor2));
        CHECK((*operation.getDoctor("09876534879"))->getName() == "Konrad");
        operation.addDoctor(std::move(doctor3));
        CHECK_THROWS_MATCHES(operation.getDoctor("09876534999"), std::logic_error, Catch::Matchers::Message("Doctor with PESEL: 09876534999 not found!"));
        operation.addNurse(std::move(nurse1));
        CHECK((*operation.getNurse("03287607850"))->getName() == "Ania");
        operation.addNurse(std::move(nurse2));
        CHECK_THROWS_MATCHES(operation.getNurse("03287607854"), std::logic_error, Catch::Matchers::Message("Nurse with PESEL: 03287607854 not found!"));
        CHECK_THROWS_MATCHES(operation.returnPatient(), std::logic_error, Catch::Matchers::Message("Patient in service not found!"));
        patient->getHealthCard().addDisease(Diseases::BRAIN_TUMOR);
        patient->getHealthCard().planService(101);
        operation.startService(std::move(patient));
        CHECK_THROWS_MATCHES(operation.returnPatient(), std::logic_error, Catch::Matchers::Message("Person must rest, not to be on operation"));
        CHECK_THROWS_MATCHES(operation.returnDoctor("03270607850"), std::logic_error, Catch::Matchers::Message("Person must rest, not to be on operation"));
        CHECK_THROWS_MATCHES(operation.returnNurse("03287607850"), std::logic_error, Catch::Matchers::Message("Person must rest, not to be on operation"));
        for (int i = 0; i < 9; i++){
            operation.continueService();
        }
        patient = operation.returnPatient();
        doctor1 = operation.returnDoctor("03270607850");
        nurse1 = operation.returnNurse("03287607850");
        CHECK(patient->getName() == "Lidia");
        CHECK(doctor1->getName() == "Jan");
        CHECK(nurse1->getName() == "Ania");
    }
}
