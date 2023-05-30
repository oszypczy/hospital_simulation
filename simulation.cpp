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
    ss << *patient << " - just registered in reception." << std::endl;
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
        ss << e.what() << " - patient died!" << std::endl;
    }
    messages.push_back(ss.str());
}

void Simulation::checkForReturnedAmbulances(){
    std::string tempMessage = hospital->getAmbulanceDispatch()->checkIfFinishedIntervention();
    if (tempMessage != ""){
        messages.push_back(tempMessage);
        std::unique_ptr<Patient> patient = hospital->getAmbulanceDispatch()->getPatient();
        std::stringstream ss;
        ss << *patient << " - patient was brought by ambulance." << std::endl;
        hospital->getReception()->addPatientToQueueFirst(std::move(patient));
        messages.push_back(ss.str());
    }
}

void Simulation::printMessages(){
    int count = 1;
    for (auto message : messages) {
        if (message != ""){
            if (count > 1) {
                std::cout << count - 1 << ". ";
            }
            std::cout << message;
            count++;
        }
    }
    std::cout << std::endl;
    messages.clear();
}

void Simulation::writeMessagesToFile(){
    std::ofstream outputFile(outputFileName, std::ios::app);
    int count = 1;
    for (const auto& message : messages) {
        if (message != ""){
            if (count > 1) {
                outputFile << count - 1 << ". ";
            }
            outputFile << message;
            count++;
        }
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
        conductConsultations();
        conductTreatments();
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
                    ss << *patient << " - patient left hospital." << std::endl;
                    messages.push_back(ss.str());
                    room->removePatient(std::move(patient));
                    continue;
                }
                auto it = hospital->getReception()->getServiceDataBase().getServiceByID(patient->getHealthCard().getServicesPlanned()[0]);
                if(typeid(**it) == typeid(Consultation)){
                    std::stringstream ss;
                    ss << *patient << " - patient was moved to CR queue." << std::endl;
                    messages.push_back(ss.str());
                    auto temp = std::move(patient);
                    ward->getConsultationRoom()->addPatientToQueue(std::move(temp));
                } else {
                    std::stringstream ss;
                    ss << *patient << " - patient was moved to OR queue." << std::endl;
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
        if (ward->getConsultationRoom()->checkIfAnyoneInQueue()){
            if (ward->getConsultationRoom()->getCurrentServiceID() == -1){
                if (!hospital->checkIfFreeDoctorExists()){
                    std::stringstream ss;
                    ss << "There are no free doctors right now. Operation cannot start!" << std::endl;
                    messages.push_back(ss.str());
                    return;
                }
                std::unique_ptr<Patient> patient = ward->getConsultationRoom()->getFirstPatientInQueue();
                ushort serviceID = patient->getHealthCard().getServicesPlanned()[0];
                std::unique_ptr<Doctor> doctor = hospital->getFreeDoctor();
                auto consultation = hospital->getReception()->getServiceDataBase().getServiceByID(serviceID);
                std::stringstream ss;
                ss << *patient << " is consulting with " << *doctor << " in room: " << ward->getConsultationRoom()->getID() << std::endl;
                messages.push_back(ss.str());
                (*consultation)->addDoctor(std::move(doctor));
                (*consultation)->startService(std::move(patient));
                ward->getConsultationRoom()->setCurrentServiceID(serviceID);
            } else {
                ushort serviceID = ward->getConsultationRoom()->getCurrentServiceID();
                auto consultation = hospital->getReception()->getServiceDataBase().getServiceByID(serviceID);
                (*consultation)->continueService();
                if ((*consultation)->getServiceState() == ServiceState::FINISHED){
                    RandomNumberGenerator randomNumberGenerator;
                    std::stringstream ss;
                    std::unique_ptr<Patient> patient = (*consultation)->returnPatient();
                    std::string tempPESEL = (*consultation)->getFirstDoctor().getPESEL();
                    std::unique_ptr<Doctor> doctor = (*consultation)->returnDoctor(tempPESEL);
                    ss << *patient << " finished consultation with " << *doctor << " in room: " << ward->getConsultationRoom()->getID() << std::endl;
                    messages.push_back(ss.str());
                    ss.str("");
                    if (randomNumberGenerator.percentage(30)){
                        if (patient->getHealthCard().checkDisease(Diseases::HEART_ATTACK)){
                            patient->getHealthCard().addDisease(Diseases::BRAIN_TUMOR);
                        } else {
                            patient->getHealthCard().addDisease(Diseases::HEART_ATTACK);
                        }
                    }
                    for (auto& disease : patient->getHealthCard().getDiseases()){
                        ushort tempID = randomNumberGenerator.chooseNumber(1, 65000);
                        auto it = hospital->getReception()->getServiceDataBase().getServiceByID(tempID);
                        while (it != hospital->getReception()->getServiceDataBase().getMedicalServices().end()){
                            serviceID = randomNumberGenerator.chooseNumber(1, 65000);
                            it = hospital->getReception()->getServiceDataBase().getServiceByID(tempID);
                        }
                        bool NFZ = randomNumberGenerator.chooseNumber(0, 1);
                        ushort totalTime = randomNumberGenerator.chooseNumber(1, 8) * 15;
                        OperationType type;
                        if (disease == Diseases::HEART_ATTACK){
                            type = OperationType::HEART_TRANSPLANT;
                            ss << *patient << " found out that he/she will need a heart transplant." << std::endl;
                            messages.push_back(ss.str());
                            ss.str("");
                        } else {
                            type = OperationType::BRAIN_TUMOR_REMOVAL;
                            ss << *patient << " found out that he/she will need a brain tumor removal." << std::endl;
                            messages.push_back(ss.str());
                            ss.str("");
                        }
                        hospital->getReception()->getServiceDataBase().addOperation(tempID, totalTime, NFZ, type, disease);
                        patient->getHealthCard().planService(tempID);
                    }
                    patient->getHealthCard().finishService(serviceID);
                    hospital->getReception()->getServiceDataBase().removeService(serviceID);
                    ward->getConsultationRoom()->setCurrentServiceID(-1);
                    for (auto& ward : hospital->getWardsList()){
                        for (auto& room : ward->getGeneralRoomList()){
                            if (room->checkIfPatientAssigned(patient->getPESEL())){
                                std::stringstream ss;
                                ss << *patient << " - patient was moved back to general room after consultation." << std::endl;
                                room->returnPatient(std::move(patient));
                                messages.push_back(ss.str());
                                break;
                            }
                        }
                    }
                    hospital->addDoctor(std::move(doctor));
                    messages.push_back(ss.str());
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
                    ss << *patient << " - patient was moved to general room " << room->getID() << std::endl;
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

void Simulation::conductTreatments(){
    for(auto& ward : hospital->getWardsList()){
        if (ward->getTreatmentRoom()->checkIfAnyoneInQueue()){
            if (ward->getTreatmentRoom()->getCurrentServiceID() == -1){
                Diseases disease = ward->getTreatmentRoom()->getDiseaseOfFirstPatientInQueue();
                DoctorSpecialty specialty;
                if (disease == Diseases::BRAIN_TUMOR){
                    specialty = DoctorSpecialty::NEUROLOGIST;
                } else {
                    specialty = DoctorSpecialty::CARDIOLOGIST;
                }
                if (!hospital->checkIfStaffReadyForOperation(specialty)){
                    std::stringstream ss;
                    ss << "There are no free doctors right now. Operation cannot start!" << std::endl;
                    messages.push_back(ss.str());
                    return;
                }
                std::unique_ptr<Patient> patient = ward->getTreatmentRoom()->getFirstPatientInQueue();
                ushort serviceID = patient->getHealthCard().getServicesPlanned()[0];
                auto operation = hospital->getReception()->getServiceDataBase().getServiceByID(serviceID);
                std::unique_ptr<Nurse> nurse = hospital->getFreeNurse();

                std::unique_ptr<Doctor> doctor1 = hospital->getDoctorBySpeciality(DoctorSpecialty::ANESTESIOLOGYST);
                std::unique_ptr<Doctor> doctor2(nullptr);
                if(disease == Diseases::BRAIN_TUMOR){
                    doctor2 = hospital->getDoctorBySpeciality(DoctorSpecialty::NEUROLOGIST);
                } else{
                    doctor2 = hospital->getDoctorBySpeciality(DoctorSpecialty::CARDIOLOGIST);
                }
                std::stringstream ss;
                ss << *patient << " is being operated by " << *doctor1 << " and "<< *doctor2 << " in room: " << ward->getTreatmentRoom()->getID() << std::endl;
                messages.push_back(ss.str());
                (*operation)->addDoctor(std::move(doctor1));
                (*operation)->addDoctor(std::move(doctor2));
                (*operation)->addNurse(std::move(nurse));
                (*operation)->startService(std::move(patient));
                ward->getTreatmentRoom()->setCurrentServiceID(serviceID);
            } else {
                ushort serviceID = ward->getTreatmentRoom()->getCurrentServiceID();
                auto operation = hospital->getReception()->getServiceDataBase().getServiceByID(serviceID);
                (*operation)->continueService();
                if ((*operation)->getServiceState() == ServiceState::FINISHED){
                    RandomNumberGenerator randomNumberGenerator;
                    std::stringstream ss;
                    std::unique_ptr<Patient> patient = (*operation)->returnPatient();

                    std::string tempPESEL = (*operation)->getFirstDoctor().getPESEL();
                    std::unique_ptr<Doctor> doctor1 = (*operation)->returnDoctor(tempPESEL);
                    std::string tempPESEL2 = (*operation)->getFirstDoctor().getPESEL();
                    std::unique_ptr<Doctor> doctor2 = (*operation)->returnDoctor(tempPESEL2);

                    std::string tempPESEL3 = (*operation)->getFirstNurse().getPESEL();
                    std::unique_ptr<Nurse> nurse = (*operation)->returnNurse(tempPESEL3);

                    ss << *patient << " finished operation with " << *doctor1 << " and " << *doctor2 << " in room: " << ward->getTreatmentRoom()->getID() << std::endl;
                    messages.push_back(ss.str());
                    ss.str("");
                    patient->getHealthCard().getDiseases().clear();
                    for (auto& planned : patient->getHealthCard().getServicesPlanned()){
                        patient->getHealthCard().finishService(planned);
                        hospital->getReception()->getServiceDataBase().removeService(planned); //tutaj mega rzadko wyrzuca błąd że nie ma takiego serwisu w bazie
                    }
                    ward->getTreatmentRoom()->setCurrentServiceID(-1);
                    for (auto& ward : hospital->getWardsList()){
                        for (auto& room : ward->getGeneralRoomList()){
                            if (room->checkIfPatientAssigned(patient->getPESEL())){
                                ss << *patient << " - patient was moved back to general room after operation." << std::endl;
                                room->returnPatient(std::move(patient));
                                messages.push_back(ss.str());
                                ss.str("");
                                break;
                            }
                        }
                    }
                    hospital->addDoctor(std::move(doctor1));
                    hospital->addDoctor(std::move(doctor2));
                    hospital->addNurse(std::move(nurse));
                    messages.push_back(ss.str());
                }
            }
        }
    }
}