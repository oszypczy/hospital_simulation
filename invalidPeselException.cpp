#include "invalidPeselException.h"

InvalidPeselException::InvalidPeselException(std::string invalidPESEL):
std::invalid_argument("Invalid PESEL number: " + invalidPESEL){}