#pragma once
#include "human.h"

enum class PatientState {IN_QUE, RESTING, ON_APPOITMENT, IN_AMBULANCE, LEFT_HOSPITAL, IN_OR};

class Patient : public Human {
    private:
        PatientState state;
        // health card
    public:
        Patient(std::string PESEL, std::string name, std::string surname, Sex sex, ushort age, PatientState state = PatientState::RESTING);
        PatientState getState() const;
        void setState(PatientState newState);
    friend std::ostream& operator<<(std::ostream& os, const Patient& patient);
};
