#include "objectNotFoundException.h"

ObjectNotFoundException::ObjectNotFoundException(std::string message):
std::logic_error(message + " not found!"){}