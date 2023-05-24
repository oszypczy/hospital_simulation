#include "invalidHumanPointer.h"

InvalidHumanPointer::InvalidHumanPointer(std::string message): std::invalid_argument(message + " not found!"){}
