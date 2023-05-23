#pragma once
#include <chrono>
#include <random>

class RandomNumberGenerator{
    private:
        std::default_random_engine generator;
    public:
        RandomNumberGenerator();
        bool percentage(int number);
        int chooseNumber(int min, int max);
};