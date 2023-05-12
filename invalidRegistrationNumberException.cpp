#include "invalidRegistrationNumberException.h"

InvalidRegistrationNumberException::InvalidRegistrationNumberException(std::string invalidNumber):
std::invalid_argument("Registration number: " + invalidNumber + " is invalid!"){}