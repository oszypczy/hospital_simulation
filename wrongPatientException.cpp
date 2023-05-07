#include "wrongPatientException.h"

WrongPatientException::WrongPatientException():
std::logic_error("Patient's health card does not match this service!"){}