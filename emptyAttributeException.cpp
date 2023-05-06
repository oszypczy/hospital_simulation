#include "emptyAttributeException.h"

EmptyAttributeException::EmptyAttributeException(std::string attributeType):
std::invalid_argument(attributeType + " cannot be empty!"){}
