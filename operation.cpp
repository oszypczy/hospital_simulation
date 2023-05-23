#include "operation.h"
#include "wrongPersonelException.h"
#include "wrongServiceStateException.h"
#include <iostream>
#include <iomanip>

Operation::Operation(ushort ID, unsigned short totalTime, bool NFZ, OperationType type)
: MedicalService(ID, totalTime, NFZ), type(type), requiredSecialty({{OperationType::HEART_TRANSPLANT, DoctorSpecialty::CARDIOLOGIST}, {OperationType::BRAIN_TUMOR_REMOVAL, DoctorSpecialty::NEUROLOGIST}}) {
}

Operation::~Operation() {}

bool Operation::checkPersonel() const {
    if (nurses.size() < 1) {
        return false;
    }
    try {
        this->getDoctor(DoctorSpecialty::ANESTESIOLOGYST);
        this->getDoctor(requiredSecialty.at(type));
    } catch (std::invalid_argument& e) {
        return false;
    }
    return true;
}

void Operation::startService(std::unique_ptr<Patient> newPatient) {
    if (!checkPersonel()){
        throw WrongPersonelException();
    }
    this->state = ServiceState::IN_PROGRESS;
    this->patient = std::move(newPatient);
    (*patient).setState(PatientState::IN_OR);
    for (auto& doctor : doctors) {
        (*doctor).setActivity(DoctorActivity::OPERATING);
    }
    for (auto& nurse : nurses) {
        (*nurse).setActivity(NurseActivity::OPERATION_ASSISTANCE);
    }
    this->operator++();
}

void Operation::continueService() {
    if (state == ServiceState::IN_PROGRESS) {
        this->operator++();
    }
    if (state == ServiceState::FINISHED) {
        finishService();
    }
    if (state == ServiceState::NOT_READY){
        throw WrongServiceStateException("NOT_READY", "IN_PROGRESS");
    }
}

void Operation::finishService() {
    if (state == ServiceState::FINISHED) {
        (*patient).setState(PatientState::RESTING);
        for (auto& doctor : doctors) {
            (*doctor).setActivity(DoctorActivity::RESTING);
        }
        for (auto& nurse : nurses) {
            (*nurse).setActivity(NurseActivity::RESTING);
        }
    } else if (state == ServiceState::IN_PROGRESS) {
        throw WrongServiceStateException("IN_PROGRESS", "FINISHED");
    } else {
        throw WrongServiceStateException("NOT_READY", "FINISHED");
    }
}

uint Operation::calculateCost() const {
    uint cost = 0;
    if (NFZ) {
        return cost;
    }
    for (const auto& doctor : doctors) {
        cost += doctor->getHourlyWage();
    }
    for (const auto& nurse : nurses) {
        cost += nurse->getHourlyWage();
    }
    cost = ((100 * cost) / 60) * totalTime; // cost of the personel service
    cost += 90000; // hospital fee for operation
    cost = 1.23 * cost; // +22% VAT
    return cost;
}

std::string Operation::print() const {
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
    if (nurses.size() != 0){
        for (auto& nurse : nurses) {
            os << (*nurse) << std::endl;
        }
    }
    ushort zl = calculateCost() / 100;
    ushort gr = calculateCost() % 100;
    os << "Operation ID: " << ID << std::endl;
    os << "Cost of Operation: " << zl << "." << std::setw(2) << std::setfill('0') << std::to_string(gr) << " zł" << std::endl;
    os << "Time of the Operation left: " << totalTime - progressTime << "min" << std::endl;
    os << "NFZ: " << NFZ;
    return os.str();
}

std::ostream& operator<<(std::ostream& os, const Operation& operation) {
    os << operation.print();
    return os;
}