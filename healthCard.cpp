#include "healthCard.h"
#include <algorithm>
#include "objectAlreadyExistsException.h"
#include "objectNotFoundException.h"

HealthCard::HealthCard(std::string PESEL) : PESEL(PESEL) {}

std::string HealthCard::getPESEL() const {
    return PESEL;
}

void HealthCard::addDisease(Diseases disease) {
    if (!checkDisease(disease)){
        diseases.push_back(disease);
    } else {
        throw ObjectAlreadyExistsException("Disease");
    }
}

void HealthCard::cureDisease(Diseases disease) {
    auto it = std::find(diseases.begin(), diseases.end(), disease);
    if (it != diseases.end()) {
        diseases.erase(it);
    } else {
        throw ObjectNotFoundException("Disease");
    }
}

bool HealthCard::checkDisease(Diseases disease) const {
    auto it = std::find(diseases.begin(), diseases.end(), disease);
    return it != diseases.end();
}

bool HealthCard::checkService(ushort serviceID) const {
    auto it = std::find(servicesPlanned.begin(), servicesPlanned.end(), serviceID);
    return it != servicesPlanned.end();
}

void HealthCard::planService(ushort serviceID) {
    if (!checkService(serviceID)) {
        servicesPlanned.push_back(serviceID);
    } else {
        throw ObjectAlreadyExistsException("Service with given ID");
    }
}

void HealthCard::finishService(ushort serviceID) {
    auto it = std::find(servicesPlanned.begin(), servicesPlanned.end(), serviceID);
    if (it != servicesPlanned.end()) {
        servicesPlanned.erase(it);
    } else {
        throw ObjectNotFoundException("Service with given ID");
    }
}
