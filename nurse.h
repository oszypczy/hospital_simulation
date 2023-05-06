#pragma once
#include "human.h"

enum class NurseActivity {RESTING, RECEPTION_SERVICE, EXAMINATION_ASSISTANCE, OPERATION_ASSISTANCE};

class Nurse : public Human {
    private:
        NurseActivity activity;
        ushort hourlyWage = 60;
    public:
        Nurse(std::string PESEL, std::string name, std::string surname, Sex sex, ushort age, NurseActivity activity = NurseActivity::RESTING);
        NurseActivity getActivity() const;
        void setActivity(NurseActivity newActivity);
        ushort getHourlyWage() const;
    friend std::ostream& operator<<(std::ostream& os, const Nurse& nurse);
};
