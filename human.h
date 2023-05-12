#pragma once
#include <string>

enum class Sex {male, female, other};

class Human{
    protected:
        std::string PESEL;
        std::string name;
        std::string surname;
        Sex sex;
        ushort age = 0;
        bool simulationState = false;
        void checkData(std::string PESEL, std::string name, std::string surname, ushort age) const;
    public:
        Human(std::string PESEL, std::string name, std::string surname, Sex sex, ushort age);
        virtual ~Human();
        std::string getPESEL() const;
        std::string getName() const;
        std::string getSurname() const;
        Sex getSex() const;
        ushort getAge() const;
        bool didStateChaneged() const;
        void setPESEL(std::string newPESEL);
        void setName(std::string newName);
        void setSurname(std::string newSurname);
        void setSex(Sex newSex);
        void setAge(ushort newAge);
        void changeState();
        bool operator==(const Human& secondHuman) const;
        bool operator!=(const Human& secondHuman) const;
    friend std::ostream& operator<<(std::ostream& out, const Human& human);
};