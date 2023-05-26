#include "randomPatientGenerator.h"
#include <algorithm>

RandomPatientGenerator::RandomPatientGenerator(){
    maleNames = {"Jan", "Adam", "Piotr", "Krzysztof", "Stanisław", "Andrzej", "Józef", "Tomasz", "Marcin", "Marek", "Michał", "Grzegorz", "Jerzy", "Tadeusz", "Łukasz", "Mateusz", "Ryszard", "Dariusz", "Zbigniew", "Kazimierz", "Mariusz", "Wojciech", "Robert", "Marian", "Rafał", "Jakub", "Henryk", "Daniel", "Mirosław", "Karol", "Władysław", "Wiesław", "Adam", "Zdzisław", "Edward", "Kamil", "Mieczysław", "Patryk", "Paweł", "Bartosz", "Szymon", "Włodzimierz", "Roman", "Dawid", "Maciej", "Artur", "Zdzisław", "Kacper", "Przemysław", "Sebastian", "Czesław", "Leszek", "Robert", "Damian", "Janusz", "Bogdan", "Antoni", "Stefan", "Aleksander", "Filip", "Bogusław", "Adrian", "Zygmunt", "Leon", "Kajetan", "Ignacy", "Konrad", "Bartłomiej", "Julian", "Witold", "Waldemar", "Wacław", "Jerzy", "Krzysztof", "Wojciech", "Janusz", "Andrzej", "Piotr", "Stanisław", "Józef", "Tadeusz", "Marek", "Jerzy", "Adam", "Kazimierz", "Dariusz", "Henryk", "Zbigniew", "Ryszard", "Kamil", "Marian", "Wiesław", "Władysław", "Paweł", "Robert", "Michał", "Wojciech", "Jan", "Piotr", "Krzysztof", "Andrzej", "Stanisław", "Tomasz", "Paweł"};
    femaleNames = {"Lidia", "Anna", "Maria", "Krystyna", "Barbara", "Teresa", "Elżbieta", "Janina", "Zofia", "Jadwiga", "Danuta", "Halina", "Irena", "Ewa", "Małgorzata", "Helena", "Grażyna", "Bożena", "Stanisława", "Jolanta", "Marianna", "Urszula", "Wanda", "Alicja", "Dorota", "Agnieszka", "Beata", "Katarzyna", "Joanna", "Magdalena", "Aleksandra", "Wiesława", "Krystyna", "Anna", "Barbara", "Elżbieta", "Zofia", "Maria", "Teresa", "Janina", "Jadwiga", "Danuta", "Halina", "Irena", "Ewa", "Małgorzata", "Helena", "Grażyna", "Bożena", "Stanisława", "Jolanta", "Marianna", "Urszula", "Wanda", "Alicja", "Dorota", "Agnieszka", "Beata", "Katarzyna", "Joanna", "Magdalena", "Aleksandra", "Wiesława", "Krystyna", "Anna", "Barbara", "Elżbieta", "Zofia", "Maria", "Teresa", "Janina", "Jadwiga", "Danuta", "Halina", "Irena", "Ewa", "Małgorzata", "Helena", "Grażyna", "Bożena", "Stanisława", "Jolanta", "Marianna", "Urszula", "Wanda", "Alicja", "Dorota", "Agnieszka", "Beata", "Katarzyna", "Joanna", "Magdalena", "Aleksandra", "Wiesława"};
    surnames = {"Nowak", "Kowalsk", "Wiśniewsk", "Dąbrowsk", "Lewandowsk", "Wójcik", "Kamińsk", "Kowalczyk", "Zielińsk", "Szymańsk", "Woźniak", "Kozłowsk", "Jankowsk", "Wojciechowsk", "Kwiatkowsk", "Kaczmarek", "Mazur", "Krawczyk", "Piotrowsk", "Grabowsk", "Nowakowsk", "Pawłowsk", "Michalsk", "Adamczyk", "Dudek", "Zając", "Wieczorek", "Jabłońsk", "Król", "Majewsk", "Olszewsk", "Jaworsk", "Wróbel", "Malinowsk", "Pawlak", "Witkowsk", "Walczak", "Stępniewsk", "Górsk", "Rutkowsk", "Michalak", "Sikora", "Ostrowsk", "Baran", "Dudek", "Szewczyk", "Tomaszewsk", "Pietrzak", "Marciniak", "Wróblewsk", "Zalewsk", "Jakubowsk", "Jasińsk", "Zawada", "Sadowsk", "Bąk", "Chmielewsk", "Włodarczyk", "Borkowsk", "Czarnek", "Sawik", "Sokołowsk", "Urbańsk", "Kubiak", "Maciejewsk", "Szczepańsk", "Kucharsk", "Wilk", "Kalinowsk", "Mazurek", "Wysock", "Adamsk", "Kaźmierczak", "Wasilewsk", "Sobczak", "Czerwińsk", "Andrzejewsk", "Cieślak", "Głowack", "Zakrzewsk", "Kołodzie", "Sikorsk", "Krajewsk"};
};

std::string RandomPatientGenerator::generatePESEL(){
    std::string PESEL = "";
    do{
        PESEL = "";
        for(int i = 0; i < 11; i++){
            PESEL += std::to_string(generator.chooseNumber(0, 9));
        }
    } while(isPESELUsed(PESEL));
    alreadyUsedPESELS.push_back(PESEL);
    return PESEL;
};

bool RandomPatientGenerator::isPESELUsed(std::string PESEL){
    auto it = std::find(alreadyUsedPESELS.begin(), alreadyUsedPESELS.end(), PESEL);
    return it != alreadyUsedPESELS.end();
};

Sex RandomPatientGenerator::generateSex(){
    int sex = generator.chooseNumber(0, 1);
    return static_cast<Sex>(sex);
}

ushort RandomPatientGenerator::generateAge(){
    return generator.chooseNumber(1, 100);
}

std::string RandomPatientGenerator::generateName(Sex sex){
    std::string name = "";
    if (sex == Sex::male){
        int i = generator.chooseNumber(0, maleNames.size() - 1);
        name = maleNames[i];
    } else {
        int i = generator.chooseNumber(0, femaleNames.size() - 1);
        name = femaleNames[i];
    }
    return name;
}

std::string RandomPatientGenerator::generateSurname(Sex sex) {
    int i = generator.chooseNumber(0, surnames.size() - 1);
    std::string surname = surnames[i];
    if (sex == Sex::male && surname.substr(surname.size() - 2) == "sk") {
        surname += "i";
    } else if (sex == Sex::female && surname.substr(surname.size() - 2) == "sk") {
        surname += "a";
    }
    return surname;
}

std::unique_ptr<Patient> RandomPatientGenerator::generatePatient(){
    std::string PESEL = generatePESEL();
    Sex sex = generateSex();
    std::string name = generateName(sex);
    std::string surname = generateSurname(sex);
    ushort age = generateAge();
    std::unique_ptr<Patient> patient = std::make_unique<Patient>(PESEL, name, surname, sex, age);
    patient->getHealthCard().addDisease(static_cast<Diseases>(generator.chooseNumber(0, 2)));
    return patient;
};
