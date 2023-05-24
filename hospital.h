#pragma once
#include "place.h"
#include "human.h"
#include "serviceDataBase.h"
#include "reception.h"
#include "ambulanceDispatch.h"

#include <string>
#include <list>
#include <memory>


class Hospital{
    private:
        std::string name;
        std::list<std::unique_ptr<Place>> placesList;
        std::list<std::unique_ptr<Human>> humansList;
        std::unique_ptr<ServiceDataBase> serviceDataBase;
        std::unique_ptr<Reception> reception;
        std::unique_ptr<AmbulanceDispatch> ambulanceDispatch;
    public:
        Hospital(std::string name, std::list<std::unique_ptr<Place>> placesList,
                std::list<std::unique_ptr<Human>> humansList,
                std::unique_ptr<Reception> reception,
                std::unique_ptr<AmbulanceDispatch> ambulanceDispatch);
        bool checkIfPlacesIDUnique() const;
        void addHuman(std::unique_ptr<Human> human);
        void removeHuman(std::unique_ptr<Human> human);
};
