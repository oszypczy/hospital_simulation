#pragma once
#include <string>
#include <vector>

enum class Diseases {FLU, BRAIN_TUMOR, HEART_ATTACK};

class HealthCard{
    private:
        std::string PESEL;
        std::vector<Diseases> diseases;
        std::vector<ushort> servicesPlanned;
    public:
        HealthCard(std::string PESEL);
        std::string getPESEL() const;
        void addDisease(Diseases disease);
        void cureDisease(Diseases disease);
        bool checkDisease(Diseases disease) const;
        void planService(ushort serviceID);
        void finishService(ushort serviceID);
        bool checkService(ushort serviceID) const;
};