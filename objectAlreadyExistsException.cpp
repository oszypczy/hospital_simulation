#include "objectAlreadyExistsException.h"

ObjectAlreadyExistsException::ObjectAlreadyExistsException(std::string message):
std::logic_error(message + " already exists!"){}