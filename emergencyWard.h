#pragma once

#include "ward.h"

#include <list>

class EmergencyWard : public Place{
    private:
        std::list<std::unique_ptr<Nurse>> nurseList;
        // PatientQueue queue;
    public:
};