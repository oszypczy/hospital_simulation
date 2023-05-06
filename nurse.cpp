#include "nurse.h"
#include <iostream>

Nurse::Nurse(std::string PESEL, std::string name, std::string surname, Sex sex, ushort age, NurseActivity activity)
    : Human(PESEL, name, surname, sex, age), activity(activity) {}

NurseActivity Nurse::getActivity() const {
    return activity;
}

void Nurse::setActivity(NurseActivity newActivity) {
    activity = newActivity;
}

ushort Nurse::getHourlyWage() const {
    return hourlyWage;
}

std::ostream& operator<<(std::ostream& os, const Nurse& nurse) {
    os << "Nurse: " << nurse.name << " " << nurse.surname << " (" << nurse.PESEL << ").";
    return os;
}
