#include "doctor.h"
#include <iostream>

Doctor::Doctor(std::string PESEL, std::string name, std::string surname, Sex sex, ushort age, DoctorSpecialty specialty, DoctorActivity activity)
    : Human(PESEL, name, surname, sex, age), specialty(specialty), activity(activity) {
        hourlyWage = {
            {DoctorSpecialty::GENERAL, 100},
            {DoctorSpecialty::CARDIOLOGIST, 150},
            {DoctorSpecialty::NEUROLOGIST, 200},
            {DoctorSpecialty::GYNECOLOGYST, 220},
            {DoctorSpecialty::ANESTESIOLOGYST, 250}};
}

DoctorSpecialty Doctor::getSpecialty() const {
    return specialty;
}

DoctorActivity Doctor::getActivity() const {
    return activity;
}

void Doctor::setSpecialty(DoctorSpecialty newSpecialty) {
    specialty = newSpecialty;
}

void Doctor::setActivity(DoctorActivity newActivity) {
    activity = newActivity;
}

ushort Doctor::getHourlyWage(){
    return hourlyWage[specialty];
}

std::ostream& operator<<(std::ostream& os, const Doctor& doctor) {
        switch(doctor.getSpecialty()) {
        case DoctorSpecialty::GENERAL:
            os << "General";
            break;
        case DoctorSpecialty::CARDIOLOGIST:
            os << "Cardiology";
            break;
        case DoctorSpecialty::NEUROLOGIST:
            os << "Neurology";
            break;
        case DoctorSpecialty::GYNECOLOGYST:
            os << "Gynecology";
            break;
        case DoctorSpecialty::ANESTESIOLOGYST:
            os << "Anesthesiology";
            break;
    }
    os << " doctor " << doctor.getName() << " " << doctor.getSurname() << " (" << doctor.getPESEL() << ").";
    return os;
}
