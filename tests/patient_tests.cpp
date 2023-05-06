#include "catch_amalgamated.hpp"
#include "../patient.h"

TEST_CASE("patient simple tests", "[patient]")
{
    Patient Lidka("03232407362", "Lidia", "Strzelecka", Sex::female, 20);
    CHECK(Lidka.getPESEL() == "03232407362");
    CHECK(Lidka.getName() == "Lidia");
    CHECK(Lidka.getSurname() == "Strzelecka");
    CHECK(Lidka.getAge() == 20);
    CHECK(Lidka.getSex() == Sex::female);
    CHECK_FALSE(Lidka.didStateChaneged());
    CHECK(Lidka.getState() == PatientState::RESTING);
    CHECK(Lidka.getHealthCard().getPESEL() == "03232407362");

    SECTION("testing valid setters")
    {
        Lidka.setPESEL("12345678901");
        Lidka.setName("Lida");
        Lidka.setSurname("Kowalska");
        Lidka.setAge(21);
        Lidka.setSex(Sex::male);
        Lidka.setState(PatientState::IN_AMBULANCE);
        CHECK(Lidka.getPESEL() == "12345678901");
        CHECK(Lidka.getName() == "Lida");
        CHECK(Lidka.getSurname() == "Kowalska");
        CHECK(Lidka.getAge() == 21);
        CHECK(Lidka.getSex() == Sex::male);
        CHECK(Lidka.getState() == PatientState::IN_AMBULANCE);
    }

    SECTION("testing invalid setters")
    {
        CHECK_THROWS_MATCHES(Lidka.setPESEL("123456789012123"), std::invalid_argument, Catch::Matchers::Message("Invalid PESEL number: 123456789012123"));
        CHECK(Lidka.getPESEL() == "03232407362");
        CHECK_THROWS_MATCHES(Lidka.setPESEL("123"), std::invalid_argument, Catch::Matchers::Message("Invalid PESEL number: 123"));
        CHECK(Lidka.getPESEL() == "03232407362");
        CHECK_THROWS_MATCHES(Lidka.setPESEL("123abc"), std::invalid_argument, Catch::Matchers::Message("Invalid PESEL number: 123abc"));
        CHECK(Lidka.getPESEL() == "03232407362");
        CHECK_THROWS_MATCHES(Lidka.setName(""), std::invalid_argument, Catch::Matchers::Message("Name cannot be empty!"));
        CHECK(Lidka.getName() == "Lidia");
        CHECK_THROWS_MATCHES(Lidka.setSurname(""), std::invalid_argument, Catch::Matchers::Message("Surname cannot be empty!"));
        CHECK(Lidka.getSurname() == "Strzelecka");
        CHECK_THROWS_MATCHES(Lidka.setAge(0), std::invalid_argument, Catch::Matchers::Message("Age cannot be equal zero!"));
        CHECK(Lidka.getAge() == 20);
    }

    SECTION("testing state change")
    {
        CHECK_FALSE(Lidka.didStateChaneged());
        Lidka.changeState();
        CHECK(Lidka.didStateChaneged());
    }

    SECTION("testing == and != operators")
    {
        CHECK(Lidka == Lidka);
        CHECK_FALSE(Lidka != Lidka);
    }

    SECTION("testing << operator")
    {
        std::stringstream ss;
        ss << Lidka;
        CHECK(ss.str() == "Patient: Lidia Strzelecka (03232407362), 20 years old.");
    }

    SECTION("testing healthcard")
    {
        Lidka.getHealthCard().addDisease(Diseases::FLU);
        Lidka.getHealthCard().planService(20);
        CHECK(Lidka.getHealthCard().checkDisease(Diseases::FLU));
        CHECK(Lidka.getHealthCard().checkService(20));
        Lidka.getHealthCard().finishService(20);
        CHECK_FALSE(Lidka.getHealthCard().checkService(20));
        Lidka.getHealthCard().cureDisease(Diseases::FLU);
        CHECK_FALSE(Lidka.getHealthCard().checkDisease(Diseases::FLU));
    }

    SECTION("testing healthcard invalid operations")
    {
       Lidka.getHealthCard().addDisease(Diseases::FLU);
       Lidka.getHealthCard().planService(20);
       CHECK_THROWS_MATCHES(Lidka.getHealthCard().addDisease(Diseases::FLU), std::logic_error, Catch::Matchers::Message("Disease already exists!"));
       CHECK_THROWS_MATCHES(Lidka.getHealthCard().planService(20), std::logic_error, Catch::Matchers::Message("Service with given ID already exists!"));
       CHECK_THROWS_MATCHES(Lidka.getHealthCard().cureDisease(Diseases::HEART_ATTACK), std::logic_error, Catch::Matchers::Message("Disease not found!"));
       CHECK_THROWS_MATCHES(Lidka.getHealthCard().finishService(21), std::logic_error, Catch::Matchers::Message("Service with given ID not found!"));
    }
}
