#include "zeroAgeException.h"

ZeroAgeException::ZeroAgeException():
std::invalid_argument("Age cannot be equal zero!"){}