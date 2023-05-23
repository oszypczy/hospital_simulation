#include "human.h"
#include <iostream>
#include "emptyAttributeException.h"
#include "zeroAgeException.h"
#include "invalidPeselException.h"

Human::Human(std::string PESEL, std::string name, std::string surname, Sex sex, ushort age) {
    checkData(PESEL, name, surname, age);
    this->PESEL = PESEL;
    this->name = name;
    this->surname = surname;
    this->sex = sex;
    this->age = age;
}

Human::~Human(){}

void Human::checkData(std::string PESEL, std::string name, std::string surname, ushort age) const {
    if (name.empty()) {
        throw EmptyAttributeException("Name");
    }
    if (surname.empty()) {
        throw EmptyAttributeException("Surname");
    }
    if (age == 0) {
        throw ZeroAgeException();
    }
    if (PESEL.size() != 11){
        throw InvalidPeselException(PESEL);
    }
    try {
        std::stoul(PESEL);
    } catch(const std::invalid_argument& e) {
        throw InvalidPeselException(PESEL);
    }
}

std::string Human::getPESEL() const {
    return PESEL;
}

std::string Human::getName() const {
    return name;
}

std::string Human::getSurname() const {
    return surname;
}

Sex Human::getSex() const {
    return sex;
}

ushort Human::getAge() const {
    return age;
}

bool Human::didStateChaneged() const {
    return state;
}

void Human::setPESEL(std::string newPESEL) {
    checkData(newPESEL, name, surname, age);
    PESEL = newPESEL;
}

void Human::setName(std::string newName) {
    checkData(PESEL, newName, surname, age);
    name = newName;
}

void Human::setSurname(std::string newSurname) {
    checkData(PESEL, name, newSurname, age);
    surname = newSurname;
}

void Human::setSex(Sex newSex) {
    sex = newSex;
}

void Human::setAge(ushort newAge) {
    checkData(PESEL, name, surname, newAge);
    age = newAge;
}

void Human::changeState() {
    state = !state;
}

bool Human::operator==(const Human& secondHuman) const {
    return PESEL == secondHuman.PESEL;
}

bool Human::operator!=(const Human& secondHuman) const {
    return !(*this == secondHuman);
}

std::ostream& operator<<(std::ostream& os, const Human& human) {
    os << human.name << " " << human.surname << " (" << human.PESEL << "), " << human.age << " years old.";
    return os;
}

// Localization logic could be added here
