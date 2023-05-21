#include "place.h"
#include "emptyIDException.h"

Place::Place(std::string id): id(id){
    if(id.empty()){
        throw EmptyIDException();
    }
}

Place::~Place(){}

std::string Place::getID(){
    return id;
}

