#pragma once
#include "human.h"

enum class ParamedicActivity {RESTING, IN_AMBULANCE};

class Paramedic : public Human {
    private:
        ParamedicActivity activity;
        ushort hourlyWage = 80;
    public:
        Paramedic(std::string PESEL, std::string name, std::string surname, Sex sex, ushort age, ParamedicActivity activity = ParamedicActivity::RESTING);
        ParamedicActivity getActivity() const;
        void setActivity(ParamedicActivity newActivity);
        ushort getHourlyWage() const;
    friend std::ostream& operator<<(std::ostream& os, const Paramedic& Paramedic);
};