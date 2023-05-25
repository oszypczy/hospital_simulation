#include "catch_amalgamated.hpp"
#include "../ambulanceDispatch.h"

TEST_CASE("ambulanceDispatch simple tests", "[ambulanceDispatch]")
{
    AmbulanceDispatch dispatch("9878890543");
    CHECK(dispatch.getID() == "9878890543");

    std::unique_ptr<Ambulance> ambulance = std::make_unique<Ambulance>("CB062MH");
    dispatch.addAmbulance(std::move(ambulance));
    CHECK_FALSE((*dispatch.getAmbulance("CB062MH"))->checkOccupancy());
    CHECK_FALSE((*dispatch.getAmbulance("CB062MH"))->checkPersonel());
    CHECK((*dispatch.getAmbulance("CB062MH"))->getTotalTime() == 30);
    CHECK((*dispatch.getAmbulance("CB062MH"))->getRegistrationNumber() == "CB062MH");
    CHECK((*dispatch.getAmbulance("CB062MH"))->getState() == AmbulanceState::IN_GARAGE);

    CHECK_FALSE((*dispatch.getFreeAmbulance())->checkOccupancy());
    CHECK_FALSE((*dispatch.getFreeAmbulance())->checkPersonel());
    CHECK((*dispatch.getFreeAmbulance())->getTotalTime() == 30);
    CHECK((*dispatch.getFreeAmbulance())->getRegistrationNumber() == "CB062MH");
    CHECK((*dispatch.getFreeAmbulance())->getState() == AmbulanceState::IN_GARAGE);

    std::unique_ptr<Paramedic> Lidka = std::make_unique<Paramedic>("03232407362", "Lidia", "Strzelecka", Sex::female, 20);
    std::unique_ptr<Paramedic> Jan = std::make_unique<Paramedic>("09876754333", "Jan", "Kowalski", Sex::male, 23);
    dispatch.addParamedic(std::move(Lidka));
    CHECK((*dispatch.getFreeParamedic())->getName() == "Lidia");
    CHECK((*dispatch.getFreeParamedic())->getSurname() == "Strzelecka");
    CHECK((*dispatch.getFreeParamedic())->getPESEL() == "03232407362");
    dispatch.addParamedic(std::move(Jan));

    SECTION("testing sending free ambulance on intervention")
    {
        std::unique_ptr<Patient> Oliwier = std::make_unique<Patient>("03270607850", "Oliwier", "Szypczyn", Sex::male, 20);
        std::stringstream ss;
        ss << dispatch.sendAmbulance(std::move(Oliwier));
        CHECK((*dispatch.getAmbulance("CB062MH"))->getState() == AmbulanceState::ON_ROAD);
        CHECK((*dispatch.getAmbulance("CB062MH"))->checkOccupancy());
        CHECK((*dispatch.getAmbulance("CB062MH"))->checkPersonel());
        CHECK((*dispatch.getAmbulance("CB062MH"))->getProgressTime() == 15);
        CHECK((*dispatch.getAmbulance("CB062MH"))->getPatient().getName() == "Oliwier");
        CHECK((*dispatch.getAmbulance("CB062MH"))->getPatient().getSurname() == "Szypczyn");
        CHECK((*dispatch.getAmbulance("CB062MH"))->getPatient().getState() == PatientState::IN_AMBULANCE);
        CHECK((*dispatch.getAmbulance("CB062MH"))->getPatient().getHealthCard().getPESEL() == "03270607850");
        CHECK((*dispatch.getAmbulance("CB062MH"))->getParamedic("03232407362").getName() == "Lidia");
        CHECK((*dispatch.getAmbulance("CB062MH"))->getParamedic("03232407362").getSurname() == "Strzelecka");
        CHECK((*dispatch.getAmbulance("CB062MH"))->getParamedic("03232407362").getActivity() == ParamedicActivity::IN_AMBULANCE);
        CHECK((*dispatch.getAmbulance("CB062MH"))->getParamedic("09876754333").getName() == "Jan");
        CHECK((*dispatch.getAmbulance("CB062MH"))->getParamedic("09876754333").getSurname() == "Kowalski");
        CHECK((*dispatch.getAmbulance("CB062MH"))->getParamedic("09876754333").getActivity()== ParamedicActivity::IN_AMBULANCE);
        CHECK(ss.str() == "Ambulance CB062MH is on road\nProgress time: 15\nEstimated total time: 30\nSTAFF: \n\tParamedic: Lidia Strzelecka (03232407362).\n\tParamedic: Jan Kowalski (09876754333).\nPATIENT: \n\tPatient: Oliwier Szypczyn (03270607850), 20 years old.\n");

        std::unique_ptr<Patient> Robert = std::make_unique<Patient>("03270607859", "Robert", "Nowak", Sex::male, 35);
        CHECK_THROWS_MATCHES(dispatch.sendAmbulance(std::move(Robert)), std::logic_error, Catch::Matchers::Message("Free ambulance not found!"));

        std::unique_ptr<Ambulance> ambulance = std::make_unique<Ambulance>("CT0707L");
        dispatch.addAmbulance(std::move(ambulance));

        Robert = std::make_unique<Patient>("03270607859", "Robert", "Nowak", Sex::male, 35);
        CHECK_THROWS_MATCHES(dispatch.sendAmbulance(std::move(Robert)), std::logic_error, Catch::Matchers::Message("Free paramedic not found!"));

        std::unique_ptr<Paramedic> Hubert = std::make_unique<Paramedic>("09876754222", "Hubert", "Skun", Sex::male, 28);
        dispatch.addParamedic(std::move(Hubert));

        Robert = std::make_unique<Patient>("03270607859", "Robert", "Nowak", Sex::male, 35);
        CHECK_THROWS_MATCHES(dispatch.sendAmbulance(std::move(Robert)), std::logic_error, Catch::Matchers::Message("Free paramedic not found!"));
    }

    SECTION("testing finishing intervention and starting a new one")
    {
        std::unique_ptr<Patient> Oliwier = std::make_unique<Patient>("03270607850", "Oliwier", "Szypczyn", Sex::male, 20);
        dispatch.sendAmbulance(std::move(Oliwier));
        dispatch.constinueAllInterventions();
        CHECK((*dispatch.getAmbulance("CB062MH"))->getState() == AmbulanceState::RETURNED);
        std::stringstream ss;
        ss << dispatch.checkIfFinishedIntervention();
        CHECK((*dispatch.getAmbulance("CB062MH"))->getState() == AmbulanceState::IN_GARAGE);
        CHECK_FALSE((*dispatch.getAmbulance("CB062MH"))->checkOccupancy());
        CHECK_FALSE((*dispatch.getAmbulance("CB062MH"))->checkPersonel());
        CHECK((*dispatch.getFreeAmbulance())->getRegistrationNumber() == "CB062MH");
        CHECK(ss.str() == "Ambulance CB062MH is in garage and returned from intervention.\n");
        std::unique_ptr<Patient> Robert = std::make_unique<Patient>("03270607859", "Robert", "Nowak", Sex::male, 35);
        dispatch.sendAmbulance(std::move(Robert));
    }
}