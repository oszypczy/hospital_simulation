#include "place.h"

Place::Place(std::string id) : id(id){};

std::string Place::getId() const{
    return id;
}