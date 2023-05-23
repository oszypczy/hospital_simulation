#include "catch_amalgamated.hpp"
#include "../nurse.h"

TEST_CASE("nurse simple tests", "[nurse]")
{
    Nurse Lidka("03232407362", "Lidia", "Strzelecka", Sex::female, 20);
    CHECK(Lidka.getPESEL() == "03232407362");
    CHECK(Lidka.getName() == "Lidia");
    CHECK(Lidka.getSurname() == "Strzelecka");
    CHECK(Lidka.getAge() == 20);
    CHECK(Lidka.getSex() == Sex::female);
    CHECK_FALSE(Lidka.didStateChaneged());
    CHECK(Lidka.getActivity() == NurseActivity::RESTING);

    SECTION("testing valid setters")
    {
        Lidka.setPESEL("12345678901");
        Lidka.setName("Lida");
        Lidka.setSurname("Kowalska");
        Lidka.setAge(21);
        Lidka.setSex(Sex::male);
        Lidka.setActivity(NurseActivity::RECEPTION_SERVICE);
        CHECK(Lidka.getPESEL() == "12345678901");
        CHECK(Lidka.getName() == "Lida");
        CHECK(Lidka.getSurname() == "Kowalska");
        CHECK(Lidka.getAge() == 21);
        CHECK(Lidka.getSex() == Sex::male);
        CHECK(Lidka.getActivity() == NurseActivity::RECEPTION_SERVICE);
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
        CHECK(ss.str() == "Nurse: Lidia Strzelecka (03232407362).");
    }
}
