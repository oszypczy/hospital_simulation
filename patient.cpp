#include "patient.h"
#include <iostream>

Patient::Patient(std::string PESEL, std::string name, std::string surname, Sex sex, ushort age, PatientState state)
    : Human(PESEL, name, surname, sex, age), state(state) {}

PatientState Patient::getState() const {
    return state;
}

void Patient::setState(PatientState newState) {
    state = newState;
}

std::ostream& operator<<(std::ostream& os, const Patient& patient) {
    os << "Patient: " << patient.name << " " << patient.surname << " (" << patient.PESEL << "), " << patient.age << " years old.";
    return os;
}