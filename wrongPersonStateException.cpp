#include "wrongPersonStateException.h"

WrongPersonStateException::WrongPersonStateException(std::string invalidState, std::string expectedState):
std::logic_error("Person must " + expectedState + ", not " + invalidState){}