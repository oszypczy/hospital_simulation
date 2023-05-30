#include "simulation.h"
#include <fstream>
#include "objectNotFoundException.h"

Simulation::Simulation(std::unique_ptr<Hospital> hospital, unsigned int iterations, ushort waitTime, std::string outputFileName)
    : hospital(std::move(hospital)), iterations(iterations), waitTime(waitTime), outputFileName(outputFileName) {
        std::tm timeInfo = {};
        timeInfo.tm_year = 123;
        timeInfo.tm_mon = 0;
        timeInfo.tm_mday = 1;
        timeInfo.tm_hour = 12;
        std::time_t time = std::mktime(&timeInfo);
        dateTime = std::chrono::system_clock::from_time_t(time);
    }

void Simulation::incrementDateTime() {
    dateTime += std::chrono::minutes(15);
}

std::string Simulation::getDateTime() {
    std::stringstream ss;
    std::time_t time = std::chrono::system_clock::to_time_t(dateTime);
    std::tm* timeInfo = std::localtime(&time);
    ss << "Date and time: " << std::put_time(timeInfo, "%d.%m.%Y, %H:%M") << std::endl;
    return ss.str();
}

void Simulation::newPatientInReception(){
    std::stringstream ss;
    std::unique_ptr<Patient> patient = randomPatientGenerator.generatePatient();
    ss << *patient << " - just came to hospital." << std::endl;
    messages.push_back(ss.str());
    hospital->getReception()->addPatientToQueueLast(std::move(patient));
}

void Simulation::patientCalled911(){
    std::stringstream ss;
    std::unique_ptr<Patient> patient = randomPatientGenerator.generatePatient();
    try {
        std::string tempMessage = hospital->getAmbulanceDispatch()->sendAmbulance(std::move(patient));
        ss << tempMessage;
    } catch (const ObjectNotFoundException& e){
        ss << e.what() << " - patient died D:" << std::endl;
    }
    messages.push_back(ss.str());
}

void Simulation::checkForReturnedAmbulances(){
    std::string tempMessage = hospital->getAmbulanceDispatch()->checkIfFinishedIntervention();
    if (tempMessage != ""){
        messages.push_back(tempMessage);
        std::unique_ptr<Patient> patient = hospital->getAmbulanceDispatch()->getPatient();
        std::stringstream ss;
        ss << *patient << " - just came to hospital by ambulance." << std::endl;
        hospital->getReception()->addPatientToQueueFirst(std::move(patient));
        messages.push_back(ss.str());
    }
}

void Simulation::printMessages(){
    int count = 1;
    for (auto message : messages) {
        if (count > 1) {
            std::cout << count - 1 << ". ";
        }
        std::cout << message;
        count++;
    }
    std::cout << std::endl;
    messages.clear();
}

void Simulation::writeMessagesToFile(){
    std::ofstream outputFile(outputFileName, std::ios::app);
    for (const auto& message : messages) {
        outputFile << message;
    }
    outputFile << std::endl;
}

void Simulation::run(){
    messages.push_back(hospital->getName() + " - simulation started.\n\n");
    printMessages();
    std::ofstream file(outputFileName, std::ios::trunc);
    if (!file.good()) {
        outputFileName = "output.txt";
    }
    while (iterations--){
        messages.push_back(getDateTime());
        incrementDateTime();
        if (randomNumberGenerator.percentage(70)){
            newPatientInReception();
        }
        hospital->getAmbulanceDispatch()->constinueAllInterventions();
        checkForReturnedAmbulances();
        if (randomNumberGenerator.percentage(30)){
            patientCalled911();
        }
        moveToGeneralRoom();
        goThroughGeneralRooms();

        // rest of simulation

        writeMessagesToFile();
        printMessages();
        std::this_thread::sleep_for(std::chrono::seconds(waitTime));
    }
}

void Simulation::goThroughGeneralRooms(){
    for(auto& ward : hospital->getWardsList()){
        for(auto& room : ward->getGeneralRoomList()){
            for(auto& patient : room->getPatientsList()){
                if(patient->getHealthCard().getDiseases().empty()){
                    std::stringstream ss;
                    ss << *patient << " - left hospital." << std::endl;
                    messages.push_back(ss.str());
                    room->removePatient(std::move(patient));
                    continue;
                }
                auto it = hospital->getReception()->getServiceDataBase().getServiceByID(patient->getHealthCard().getServicesPlanned()[0]);
                if(typeid(**it) == typeid(Consultation)){
                    std::stringstream ss;
                    ss << *patient << " - moved to Consultation Room queue" << std::endl;
                    messages.push_back(ss.str());
                    auto temp = std::move(patient);
                    ward->getConsultationRoom()->addPatientToQueue(std::move(temp));
                } else {
                    std::stringstream ss;
                    ss << *patient << " - moved to Treatment Room queue" << std::endl;
                    messages.push_back(ss.str());
                    auto temp = std::move(patient);
                    ward->getTreatmentRoom()->addPatientToQueue(std::move(temp));
                }
            }
            room->getPatientsList().clear();
        }
    }
}

void Simulation::conductConsultations(){
    for(auto& ward : hospital->getWardsList()){
        // if no consultation is being conducted
        if (ward->getConsultationRoom()->getFirstPatientInQueue() != nullptr){
            if (ward->getConsultationRoom()->getCurrentServiceID() == -1){
                std::unique_ptr<Patient> patient = ward->getConsultationRoom()->getFirstPatientInQueue();
                ushort serviceID = patient->getHealthCard().getServicesPlanned()[0];
                std::unique_ptr<Doctor> doctor = hospital->getFreeDoctor();
                auto consultation = hospital->getReception()->getServiceDataBase().getServiceByID(serviceID);
                std::stringstream ss;
                ss << *patient << " is consulting with " << *doctor << "in room: " << ward->getConsultationRoom()->getID() << std::endl;
                messages.push_back(ss.str());
                (*consultation)->addDoctor(std::move(doctor));
                (*consultation)->startService(std::move(patient));
            } else {
                ushort serviceID = ward->getConsultationRoom()->getCurrentServiceID();
                auto consultation = hospital->getReception()->getServiceDataBase().getServiceByID(serviceID);
                (*consultation)->continueService();
                if ((*consultation)->getServiceState() == ServiceState::FINISHED){
                    RandomNumberGenerator randomNumberGenerator;
                    std::unique_ptr<Patient> patient = (*consultation)->returnPatient();
                    std::string tempPESEL = (*consultation)->getFirstDoctor().getPESEL();
                    std::unique_ptr<Doctor> doctor = (*consultation)->returnDoctor(tempPESEL);
                    // trzeba ogranąć żeby nie miał tych samych dwóch chorób
                    if (randomNumberGenerator.percentage(30)){
                        patient->getHealthCard().addDisease(static_cast<Diseases>(randomNumberGenerator.chooseNumber(0, 1)));
                    }
                    // do każdej choroby zaplanować operację
                    patient->getHealthCard().finishService(serviceID);
                    hospital->getReception()->getServiceDataBase().removeService(serviceID);
                    ward->getConsultationRoom()->setCurrentServiceID(-1);
                    //gererowanie wiadomości o przebiegu konsultacji
                    //wysyłka pacjenta do pokoju łóżkowego
                    //wysyłka lekarza do szpitala
                }
            }
        }
    }
}


void Simulation::moveToGeneralRoom(){
    if (!hospital->getReception()->getPatientsQueue().empty()){
        std::unique_ptr<Patient> patient = std::move(hospital->getReception()->movePatient());
        for (auto& ward : hospital->getWardsList()){
            for (auto& room : ward->getGeneralRoomList()){
                if (!room->isFull()){
                    std::stringstream ss;
                    ss << *patient << " - moved to general room " << room->getID() << " ID." << std::endl;
                    room->addPatient(std::move(patient));
                    messages.push_back(ss.str());
                    return;
                }
            }
        }
        std::stringstream ss;
        ss << *patient << " must wait in reception. No general rooms free at the moment!" << std::endl;
        messages.push_back(ss.str());
        hospital->getReception()->waitInReception(std::move(patient));
    }
}