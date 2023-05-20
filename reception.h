#pragma once

#include "place.h"
#include "nurse.h"
#include "patient.h"

#include <memory>
#include <deque>

class Reception : public Place{
    private:
        Nurse nurse;
        std::deque<Patient> patentsQueue;
    public:

};