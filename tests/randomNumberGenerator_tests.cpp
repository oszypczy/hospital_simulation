#include "catch_amalgamated.hpp"
#include "../randomNumberGenerator.h"

TEST_CASE("random number generator simple tests", "[randomNumberGenerator]")
{
    SECTION("Test percentage method") {
        RandomNumberGenerator rng;
        int count = 100000;
        int targetPercentage = 50;
        int countWithinRange = 0;

        for (int i = 0; i < count; ++i) {
            if (rng.percentage(targetPercentage)) {
                countWithinRange++;
            }
        }

        int calculatedPercentage = (countWithinRange * 100) / count;

        REQUIRE(calculatedPercentage <= targetPercentage);
    }

    SECTION("Test chooseNumber method") {
        RandomNumberGenerator rng;
        int min = 1;
        int max = 100;

        int number = rng.chooseNumber(min, max);

        REQUIRE(number >= min);
        REQUIRE(number <= max);
    }
}
