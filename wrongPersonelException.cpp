#include "wrongPersonelException.h"

WrongPersonelException::WrongPersonelException():
std::logic_error("Wrong personel for this service!"){}