#include "ward.h"

Ward::Ward(std::string id, std::string name, std::list<std::unique_ptr<Room>> roomList):
            Place(id), name(name), roomList(std::move(roomList)){}

std::string Ward::getName() const{
    return name;
}
