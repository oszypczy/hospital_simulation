#include "catch_amalgamated.hpp"
#include "../ambulance.h"

TEST_CASE("ambulance simple tests", "[ambulance]")
{
    Ambulance ambulance("CB062MH", 60);
    CHECK(ambulance.getProgressTime() == 0);
    CHECK_FALSE(ambulance.checkPersonel());
    CHECK_FALSE(ambulance.checkOccupancy());
    CHECK(ambulance.getTotalTime() == 60);
    CHECK(ambulance.getRegistrationNumber() == "CB062MH");
    CHECK(ambulance.getState() == AmbulanceState::IN_GARAGE);

    SECTION("testing setters")
    {
        ambulance.setRegistrationNumber("CB12345");
        CHECK(ambulance.getRegistrationNumber() == "CB12345");
        CHECK_THROWS_MATCHES(ambulance.setRegistrationNumber(""), std::invalid_argument, Catch::Matchers::Message("Registration number:  is invalid!"));
        CHECK(ambulance.getRegistrationNumber() == "CB12345");
        CHECK_THROWS_MATCHES(ambulance.setRegistrationNumber("CB092##"), std::invalid_argument, Catch::Matchers::Message("Registration number: CB092## is invalid!"));
        CHECK(ambulance.getRegistrationNumber() == "CB12345");
        CHECK_THROWS_MATCHES(ambulance.setRegistrationNumber("cb092mh"), std::invalid_argument, Catch::Matchers::Message("Registration number: cb092mh is invalid!"));
        CHECK(ambulance.getRegistrationNumber() == "CB12345");
        CHECK_THROWS_MATCHES(ambulance.setRegistrationNumber("CB0000KKK"), std::invalid_argument, Catch::Matchers::Message("Registration number: CB0000KKK is invalid!"));
        CHECK(ambulance.getRegistrationNumber() == "CB12345");
    }

    SECTION("testing some wrong scenarios")
    {
        CHECK(ambulance.getProgressTime() == 0);
        CHECK_THROWS_MATCHES(++ambulance, std::logic_error, Catch::Matchers::Message("Invalid service state: IN_GARAGE. Expected state: ON_ROAD"));
        CHECK(ambulance.getProgressTime() == 0);
        CHECK_THROWS_MATCHES(ambulance.finishIntervention(), std::logic_error, Catch::Matchers::Message("Invalid service state: IN_GARAGE. Expected state: RETURNED"));
        CHECK(ambulance.getProgressTime() == 0);
        CHECK_THROWS_MATCHES(ambulance.continueIntervention(), std::logic_error, Catch::Matchers::Message("Invalid service state: IN_GARAGE. Expected state: ON_ROAD"));
        CHECK(ambulance.getProgressTime() == 0);
        CHECK_THROWS_MATCHES(ambulance.returnPatient(), std::logic_error, Catch::Matchers::Message("Patient inside ambulance not found!"));
        CHECK_THROWS_MATCHES(ambulance.returnParamedic("12345678901"), std::logic_error, Catch::Matchers::Message("Paramedic with given PESEL not found!"));
        std::unique_ptr<Paramedic> paramedic1 = std::make_unique<Paramedic>("03270607850", "Jan", "Kowalski", Sex::male, 29, ParamedicActivity::IN_AMBULANCE);
        paramedic1 = ambulance.addParamedic(std::move(paramedic1));
        CHECK_THROWS_MATCHES(ambulance.returnParamedic("03270607850"), std::logic_error, Catch::Matchers::Message("Paramedic with given PESEL not found!"));
        CHECK(paramedic1->getName() == "Jan");
    }

    SECTION("start and continue of intervention")
    {
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("03232407362", "Lidia", "Strzelecka", Sex::female, 20);
        CHECK(patient->getState() == PatientState::RESTING);
        std::unique_ptr<Paramedic> paramedic1 = std::make_unique<Paramedic>("03270607850", "Jan", "Kowalski", Sex::male, 29);
        std::unique_ptr<Paramedic> paramedic2 = std::make_unique<Paramedic>("09876534879", "Konrad", "Nowak", Sex::other, 56);
        CHECK(paramedic1->getActivity() == ParamedicActivity::RESTING);
        CHECK(paramedic2->getActivity() == ParamedicActivity::RESTING);
        CHECK_FALSE(ambulance.checkPersonel());
        ambulance.addParamedic(std::move(paramedic1));
        ambulance.addParamedic(std::move(paramedic2));
        CHECK(ambulance.checkPersonel());
        CHECK(ambulance.getState() == AmbulanceState::IN_GARAGE);
        CHECK_FALSE(ambulance.checkOccupancy());
        ambulance.startIntervention(std::move(patient));
        CHECK(ambulance.checkOccupancy());
        CHECK(ambulance.getState() == AmbulanceState::ON_ROAD);
        CHECK(ambulance.getPatient().getState() == PatientState::IN_AMBULANCE);
        CHECK(ambulance.getParamedic("03270607850").getActivity() == ParamedicActivity::IN_AMBULANCE);
        CHECK(ambulance.getParamedic("09876534879").getActivity() == ParamedicActivity::IN_AMBULANCE);
        CHECK(ambulance.getProgressTime() == 15);
        CHECK_THROWS_MATCHES(ambulance.returnPatient(), std::logic_error, Catch::Matchers::Message("Person must rest, not to be in ambulance"));
        CHECK_THROWS_MATCHES(ambulance.returnParamedic("09876534879"), std::logic_error, Catch::Matchers::Message("Person must rest, not to be in ambulance"));
    }

    SECTION("finish intervention")
    {
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("03232407362", "Lidia", "Strzelecka", Sex::female, 20);
        std::unique_ptr<Paramedic> paramedic1 = std::make_unique<Paramedic>("03270607850", "Jan", "Kowalski", Sex::male, 29);
        std::unique_ptr<Paramedic> paramedic2 = std::make_unique<Paramedic>("09876534879", "Konrad", "Nowak", Sex::other, 56);
        ambulance.addParamedic(std::move(paramedic1));
        ambulance.addParamedic(std::move(paramedic2));
        CHECK(ambulance.getProgressTime() == 0);
        ambulance.startIntervention(std::move(patient));
        CHECK(ambulance.getProgressTime() == 15);
        CHECK_THROWS_MATCHES(ambulance.finishIntervention(), std::logic_error, Catch::Matchers::Message("Invalid service state: ON_ROAD. Expected state: RETURNED"));
        ambulance.continueIntervention();
        CHECK(ambulance.getProgressTime() == 30);
        ambulance.continueIntervention();
        CHECK(ambulance.getProgressTime() == 45);
        ambulance.continueIntervention();
        CHECK(ambulance.getProgressTime() == 60);
        CHECK_FALSE(ambulance.checkOccupancy());
        CHECK(ambulance.getState() == AmbulanceState::IN_GARAGE);
        CHECK(ambulance.getPatient().getState() == PatientState::RESTING);
        CHECK(ambulance.getParamedic("03270607850").getActivity() == ParamedicActivity::RESTING);
        CHECK(ambulance.getParamedic("09876534879").getActivity() == ParamedicActivity::RESTING);
        patient = ambulance.returnPatient();
        CHECK(patient->getState() == PatientState::RESTING);
        paramedic1 = ambulance.returnParamedic("03270607850");
        CHECK(paramedic1->getName() == "Jan");
    }

    SECTION("testing operator <<")
    {
        std::unique_ptr<Patient> patient = std::make_unique<Patient>("03232407362", "Lidia", "Strzelecka", Sex::female, 20);
        std::unique_ptr<Paramedic> paramedic1 = std::make_unique<Paramedic>("03270607850", "Jan", "Kowalski", Sex::male, 29);
        std::unique_ptr<Paramedic> paramedic2 = std::make_unique<Paramedic>("09876534879", "Konrad", "Nowak", Sex::other, 56);
        ambulance.addParamedic(std::move(paramedic1));
        ambulance.addParamedic(std::move(paramedic2));
        CHECK(ambulance.getProgressTime() == 0);
        ambulance.startIntervention(std::move(patient));
        std::stringstream ss;
        ss << ambulance;
        CHECK(ss.str() == "Ambulance CB062MH is on road\nProgress time: 15\nEstimated total time: 60\nSTAFF: \n\tParamedic: Jan Kowalski (03270607850).\n\tParamedic: Konrad Nowak (09876534879).\nPATIENT: \n\tPatient: Lidia Strzelecka (03232407362), 20 years old.\n");
    }
}