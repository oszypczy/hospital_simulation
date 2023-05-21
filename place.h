#pragma once
#include <string>

class Place{
    protected:
        std::string id;
    public:
        Place(std::string id);
        std::string getId() const;
};
