#include "emptyIDException.h"

EmptyIDException::EmptyIDException(): std::invalid_argument("ID cannot be empty"){}
