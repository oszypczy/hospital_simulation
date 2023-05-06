#pragma once
#include "human.h"
#include <map>

enum class DoctorSpecialty {GENERAL, CARDIOLOGIST, NEUROLOGIST, GYNECOLOGYST, ANESTESIOLOGYST};

enum class DoctorActivity {RESTING, EXAMINING, CONSULTING, OPERATING};

class Doctor : public Human {
    private:
        DoctorSpecialty specialty;
        DoctorActivity activity;
        std::map<DoctorSpecialty, ushort> hourlyWage;
    public:
        Doctor(std::string PESEL, std::string name, std::string surname, Sex sex, ushort age, DoctorSpecialty specialty, DoctorActivity activity = DoctorActivity::RESTING);
        DoctorSpecialty getSpecialty() const;
        DoctorActivity getActivity() const;
        void setSpecialty(DoctorSpecialty newSpecialty);
        void setActivity(DoctorActivity newActivity);
        ushort getHourlyWage();
    friend std::ostream& operator<<(std::ostream& os, const Doctor& doctor);
};
