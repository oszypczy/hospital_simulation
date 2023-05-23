#include "healthCard.h"
#include <algorithm>
#include "objectAlreadyExistsException.h"
#include "objectNotFoundException.h"

HealthCard::HealthCard(std::string PESEL) : PESEL(PESEL) {}

std::string HealthCard::getPESEL() const {
    return PESEL;
}

void HealthCard::addDisease(Diseases disease) {
    /*
    * Add a disease to the health card.
    * @param disease The disease to be added.
    * @throws ObjectAlreadyExistsException if the disease already exists in the health card.
    */
    if (!checkDisease(disease)){
        diseases.push_back(disease);
    } else {
        throw ObjectAlreadyExistsException("Disease");
    }
}

void HealthCard::cureDisease(Diseases disease) {
    /*
    * Cure a disease in the health card.
    * @param disease The disease to be cured.
    * @throws ObjectNotFoundException if the disease does not exist in the health card.
    */
    auto it = std::find(diseases.begin(), diseases.end(), disease);
    if (it != diseases.end()) {
        diseases.erase(it);
    } else {
        throw ObjectNotFoundException("Disease");
    }
}

bool HealthCard::checkDisease(Diseases disease) const {
    /*
    * Check if a specific disease exists in the health card.
    * @param disease The disease to be checked.
    * @return True if the disease exists in the health card, false otherwise.
    */
    auto it = std::find(diseases.begin(), diseases.end(), disease);
    return it != diseases.end();
}

bool HealthCard::checkService(ushort serviceID) const {
    /*
    * Check if a specific service is planned in the health card.
    * @param serviceID The ID of the service to be checked.
    * @return True if the service is planned, false otherwise.
    */
    auto it = std::find(servicesPlanned.begin(), servicesPlanned.end(), serviceID);
    return it != servicesPlanned.end();
}

void HealthCard::planService(ushort serviceID) {
    /*
    * Plan a service in the health card.
    * @param serviceID The ID of the service to be planned.
    * @throws ObjectAlreadyExistsException if the service with the given ID already exists in the health card.
    */
    if (!checkService(serviceID)) {
        servicesPlanned.push_back(serviceID);
    } else {
        throw ObjectAlreadyExistsException("Service with given ID");
    }
}

void HealthCard::finishService(ushort serviceID) {
    /*
    * Finish a planned service in the health card.
    * @param serviceID The ID of the service to be finished.
    * @throws ObjectNotFoundException if the service with the given ID does not exist in the health card.
    */
    auto it = std::find(servicesPlanned.begin(), servicesPlanned.end(), serviceID);
    if (it != servicesPlanned.end()) {
        servicesPlanned.erase(it);
    } else {
        throw ObjectNotFoundException("Service with given ID");
    }
}
