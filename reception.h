#pragma once

#include "place.h"
#include "nurse.h"
#include "patient.h"

#include <memory>
#include <deque>

class Reception : public Place{
    private:
        std::unique_ptr<Nurse> nurse;
        std::deque<Patient> patentsQueue;
    public:
        void addPatientToQueueFirst();
        void addPatientToQueueLast();
        void setNurse();
        //moze usuwanie i dodawanie pielegniarki
};