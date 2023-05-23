#include "randomNumberGenerator.h"

RandomNumberGenerator::RandomNumberGenerator() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator = std::default_random_engine(seed);
}

bool RandomNumberGenerator::percentage(int number) {
    auto distribution = std::uniform_int_distribution<int>(1, 100);
    return distribution(generator) <= number;
}

int RandomNumberGenerator::chooseNumber(int min, int max) {
    auto distribution = std::uniform_int_distribution<int>(min, max);
    return distribution(generator);
}