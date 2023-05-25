#include "ward.h"

Ward::Ward(std::string id, std::string name,  std::unique_ptr<TreatmentRoom> treatmentRoom,
            std::unique_ptr<ConsultationRoom> consultationRoom, std::list<std::unique_ptr<Room>> generalRoomList):
            Place(id), name(name), treatmentRoom(std::move(treatmentRoom)),
            consultationRoom(std::move(consultationRoom)), generalRoomList(std::move(generalRoomList)){}

std::string Ward::getName() const{
    return name;
}

std::list<std::unique_ptr<Room>>& Ward::getGeneralRoomList(){
    return generalRoomList;
}

std::unique_ptr<TreatmentRoom>& Ward::getTreatmentRoom(){
    return treatmentRoom;
}

std::unique_ptr<ConsultationRoom>& Ward::getConsultationRoom(){
    return consultationRoom;
}
