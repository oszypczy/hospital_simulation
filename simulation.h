#pragma once
#include "hospital.h"
#include "randomNumberGenerator.h"
#include "randomPatientGenerator.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

class Simulation {
    private:
        std::unique_ptr<Hospital> hospital;
        unsigned int iterations;
        std::vector<std::string> messages;
        ushort waitTime;
        std::string outputFileName;
        std::chrono::system_clock::time_point dateTime;
        RandomNumberGenerator randomNumberGenerator;
        RandomPatientGenerator randomPatientGenerator;
    public:
        Simulation(std::unique_ptr<Hospital> hospital, unsigned int iterations, ushort waitTime, std::string outputFileName);
        void run();
        void incrementDateTime();
        std::string getDateTime();
        void printMessages();
        void writeMessagesToFile();
        void newPatientInReception();
        void patientCalled911();
        void checkForReturnedAmbulances();

//         Zaczynamy symulację:
// Losujemy czy w danej iteracji ma przyjść do recepcji pacjent: 75%
// Pacjent jeśli przyjdzie do recepcji to:
// Ma losowane z pewnej puli dane (klasa pomocnicza)
// karta zdrowia jest generowana z jakąś chorobą i bez zaplanowanych usług
// Umieszczamy go w kolejce do recepcji
// Generujemy wiadomość o tym że pacjent przyszedł do szpitala
// Losujemy czy w danej iteracji ma przyjechać karetką pacjent: 25%
// Wysyłamy karetkę po pacjenta 
// Generujemy pacjenta tak jak wyżej
// Generujemy, że karetka wyjechała na interwencję
// Przechodzimy po karetkach i szukamy zakończonych kursów
// Jeśli jakiś się zakończył to przenosimy pacjenta do kolejki na recepcję ale na początek
// Generujemy wiadomość, że karetka zakończyła kurs
// Przechodzimy po pacjentach z sali ogólnej:
// Jeśli pacjent nie ma chorób to może wyjść ze szpitala
// Generujemy wiadomość, że pacjent opuścił szpital
// Jeśli pacjent ma zaplanowaną operację to wysyłamy go do gabinetu do kolejki do operacji 
// Jeśli pacjent ma zaplanowaną konsultację to wysyłamy go do gabinetu so kolejki do konsultacji

    void goThroughGeneralRooms();

// Przechodzimy po kolejce do gabinetów:
// Gabinet operacyjny:
// Jeśli w gabinecie nie odbywa się operacja to bierzemy pierwszą osobę z kolejki i dodajemy do operacji (rozpoczynamy operację)
// Wiadomość o rozpoczęciu operacji
// Jeśli w gabinecie jest już operacja to kontynuujemy operację
// Jeśli operacja się kończy to z szansą 60% leczymy chorobę
// Jeśli choroba wyleczona to usuwamy mu zaplanowaną operację
// Wysyłamy do sali ogólnej
// Wiadomość o zakończonej operacji z informacją o powodzeniu
// Gabinet konsultacyjny:
// Jeśli w gabinecie nie odbywa się konsultacja to bierzemy pierwszą osobę z kolejki i dodajemy do konsultacji (rozpoczynamy konsultację)
// Generujemy wiadomość o rozpoczęciu konsultacji
// Jeśli w gabinecie jest już konsultacje to ją kontynuujemy
// Jeśli konsultacja się skończy to z szansą 30% dodajemy nową chorobę 
// Do wszystkich chorób pacjenta planujemy mu operację 
// Wysyłamy do ogólnej
// Generujemy wiadomość, że konsultacja się zakończyła i o tym czy zostały wykryte choroby

// Przechodzimy po kolejce do recepcji:
// Bierzemy pierwszą osobę z kolejki
// Planujemy mu z szansą 50/50 konsultację lub operację
// Wysyłamy do sali ogólnej

};