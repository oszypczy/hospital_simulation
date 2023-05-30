#include "paramedic.h"
#include <iostream>

Paramedic::Paramedic(std::string PESEL, std::string name, std::string surname, Sex sex, ushort age, ParamedicActivity activity)
    : Human(PESEL, name, surname, sex, age), activity(activity) {}

ParamedicActivity Paramedic::getActivity() const {
    return activity;
}

void Paramedic::setActivity(ParamedicActivity newActivity) {
    activity = newActivity;
}

ushort Paramedic::getHourlyWage() const {
    return hourlyWage;
}

std::ostream& operator<<(std::ostream& os, const Paramedic& Paramedic) {
    os << "Paramedic: " << Paramedic.name << " " << Paramedic.surname << " (" << Paramedic.PESEL << ")";
    return os;
}