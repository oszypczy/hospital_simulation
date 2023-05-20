#include "catch_amalgamated.hpp"
#include "../randomPatientGenerator.h"

TEST_CASE("random patient generator tests", "[randomPatientGenerator]") {
    RandomPatientGenerator generator;

    SECTION("Test generatePESEL method") {
        std::string pesel = generator.generatePESEL();
        REQUIRE(pesel.size() == 11);
    }

    SECTION("Test isPESELUsed method") {
        std::string pesel = "12345678901";
        REQUIRE_FALSE(generator.isPESELUsed(pesel));
    }

    SECTION("Test generateAge method") {
        ushort age = generator.generateAge();
        REQUIRE(age >= 1);
        REQUIRE(age <= 100);
    }

    SECTION("Test generateName method") {
        Sex male = Sex::male;
        Sex female = Sex::female;

        std::string maleName = generator.generateName(male);
        REQUIRE_FALSE(maleName.empty());

        std::string femaleName = generator.generateName(female);
        REQUIRE_FALSE(femaleName.empty());

        REQUIRE(maleName != femaleName);
    }

    SECTION("Test generateSurname method") {
        Sex male = Sex::male;
        Sex female = Sex::female;

        std::string maleSurname = generator.generateSurname(male);
        REQUIRE_FALSE(maleSurname.empty());

        std::string femaleSurname = generator.generateSurname(female);
        REQUIRE_FALSE(femaleSurname.empty());

        REQUIRE(maleSurname != femaleSurname);
    }

    SECTION("Test generatePatient method") {
        std::unique_ptr<Patient> patient = generator.generatePatient();

        REQUIRE_FALSE(patient->getPESEL().empty());
        REQUIRE_FALSE(patient->getName().empty());
        REQUIRE_FALSE(patient->getSurname().empty());
        REQUIRE(patient->getAge() >= 1);
        REQUIRE(patient->getAge() <= 100);
    }
}
