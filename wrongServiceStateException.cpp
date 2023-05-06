#include "wrongServiceStateException.h"

WrongServiceStateException::WrongServiceStateException(std::string invalidState, std::string expectedState)
: std::logic_error("Invalid service state: " + invalidState + ". Expected state: " + expectedState) {}
