#include "hospital.h"
#include "invalidHumanPointer.h"
#include "invalidHospitalName.h"

#include <algorithm>

Hospital::Hospital(std::string name, std::list<std::unique_ptr<Place>> placesList,
                    std::list<std::unique_ptr<Human>> humanList):
                    name(name), placesList(placesList), humansList(humansList){
                        if(name.empty()){
                            throw InvalidHospitalName();
                        }
                    }

bool Hospital::checkIfPlacesIDUnique()const{
    std::list<std::string> duplicatePlacesList;

    for(const auto& place : placesList){
        duplicatePlacesList.push_back(place->getID());
    }

    duplicatePlacesList.unique();
    return (duplicatePlacesList.size() == placesList.size());
}

void Hospital::addHuman(std::unique_ptr<Human> human){
    humansList.push_back(std::move(human));
}

void Hospital::removeHuman(std::unique_ptr<Human> human) {
    auto it = std::find_if(humansList.begin(), humansList.end(), [&](const std::unique_ptr<Human>& h) {
        return *h == *human;
    });

    if (it != humansList.end()) {
        humansList.erase(it);
    }
    else throw InvalidHumanPointer();
}
