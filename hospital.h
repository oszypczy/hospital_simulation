#pragma once
#include "place.h"
#include "human.h"
#include "serviceDataBase.h"

#include <string>
#include <list>
#include <memory>


class Hospital{
    private:
        std::string name;
        std::list<std::unique_ptr<Place>> placesList;
        std::list<std::unique_ptr<Human>> humansList;
        ServiceDataBase serviceDataBase;
    public:
        Hospital(std::string name, std::list<std::unique_ptr<Place>> placesList, std::list<std::unique_ptr<Human>> humansList);
        bool checkIfPlacesIDUnique() const;
        void addHuman(std::unique_ptr<Human> human);
        void removeHuman(std::unique_ptr<Human> human);
};
