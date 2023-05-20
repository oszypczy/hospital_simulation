#pragma once

#include <string>

class Place{
    protected:
        std::string id;
    public:
        ~Place();
        virtual bool if_unique_id() = 0;
};