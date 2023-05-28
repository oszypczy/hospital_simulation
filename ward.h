#pragma once

#include "place.h"
#include "doctor.h"
#include "nurse.h"
#include "room.h"
#include "treatmentRoom.h"
#include "consultationRoom.h"
#include "generalRoom.h"

#include <memory>
#include <list>
#include <string>



class Ward : public Place{
    private:
        std::string name;
        std::unique_ptr<TreatmentRoom> treatmentRoom;
        std::unique_ptr<ConsultationRoom> consultationRoom;
        std::list<std::unique_ptr<GeneralRoom>> generalRoomList;
    public:
        Ward(std::string id, std::string name, std::unique_ptr<TreatmentRoom> treatmentRoom,
            std::unique_ptr<ConsultationRoom> consultationRoom, std::list<std::unique_ptr<GeneralRoom>> generalRoomList);
        std::string getName() const;
        std::list<std::unique_ptr<GeneralRoom>>& getGeneralRoomList();
        std::unique_ptr<TreatmentRoom>& getTreatmentRoom();
        std::unique_ptr<ConsultationRoom>& getConsultationRoom();
};
