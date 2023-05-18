#pragma once

#include "room.h"
#include "doctor.h"
#include "nurse.h"
#include "patient.h"
#include "operation.h"
#include "operation.h"
//#include "employee.h" // tego nie ma ale mogloby sie przydac zeby przechowywac nizej w liscie pracownikow, chyba ze zrobic liste doktorow + liste pielegniarek

#include <memory>
#include <list>

class TreatmentRoom : public Room{
    private:
        std::list<std::unique_ptr<Doctor>> doctors;
        std::list<std::unique_ptr<Nurse>> nurses;
        Patient treatedPatient;
        Operation operation;
    public:
        void proceedTreatment();


};