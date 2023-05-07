#include "consultation.h"
#include "wrongPersonelException.h"
#include "wrongServiceStateException.h"
#include "wrongPatientException.h"
#include <iostream>
#include <iomanip>

Consultation::Consultation(ushort ID, unsigned short totalTime, bool NFZ) : MedicalService(ID, totalTime, NFZ) {}

Consultation::~Consultation() {}

bool Consultation::checkPersonel() const {
    if (doctors.size() < 1) {
        return false;
    }
    return true;
}

void Consultation::startService(std::unique_ptr<Patient> newPatient) {
    if (!checkPersonel()){
        this->patient = std::move(newPatient);
        throw WrongPersonelException();
    }
    if (!(newPatient->getHealthCard().checkService(ID))){
        this->patient = std::move(newPatient);
        throw WrongPatientException();
    }
    this->state = ServiceState::IN_PROGRESS;
    this->patient = std::move(newPatient);
    (*patient).setState(PatientState::ON_APPOITMENT);
    for (auto& doctor : doctors) {
        (*doctor).setActivity(DoctorActivity::CONSULTING);
    }
    this->operator++();
}

void Consultation::continueService() {
    if (state == ServiceState::IN_PROGRESS) {
        this->operator++();
    }
    if (state == ServiceState::FINISHED) {
        finishService();
    } else {
        throw WrongServiceStateException("NOT_READY", "IN_PROGRESS");
    }
}

void Consultation::finishService() {
    if (state == ServiceState::FINISHED) {
        (*patient).setState(PatientState::RESTING);
        for (auto& doctor : doctors) {
            (*doctor).setActivity(DoctorActivity::RESTING);
        }
    } else if (state == ServiceState::IN_PROGRESS) {
        throw WrongServiceStateException("IN_PROGRESS", "FINISHED");
    } else {
        throw WrongServiceStateException("NOT_READY", "FINISHED");
    }
}

uint Consultation::calculateCost() const {
    uint cost = 0;
    if (NFZ) {
        return cost;
    }
    for (const auto& doctor : doctors) {
        cost += doctor->getHourlyWage();
    }
    cost = (100 * cost) / 60 * totalTime; // cost of the personel service
    cost += 15000; // hospital fee for consulatation
    cost = 1.2 * cost; // +20% VAT
    return cost;
}

std::string Consultation::print() const {
    std::stringstream os;
    if (patient){
        os << *patient << std::endl;
    }
    if (doctors.size() != 0){
        os << "Medical personel: ";
        for (auto& doctor : doctors) {
            os << (*doctor) << std::endl;
    }
    }
    ushort zl = calculateCost() / 100;
    ushort gr = calculateCost() % 100;
    os << "Consultation ID: " << ID << std::endl;
    os << "Cost of consultation: " << zl << "." << std::setw(2) << std::setfill('0') << std::to_string(gr) << " zł" << std::endl;
    os << "Time of the consultation left: " << totalTime - progressTime << "min" << std::endl;
    os << "NFZ: " << NFZ;
    return os.str();
}

std::ostream& operator<<(std::ostream& os, const Consultation& consultation) {
    os << consultation.print();
    return os;
}
