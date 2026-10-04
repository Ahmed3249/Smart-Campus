#include "User.h"
#include <iostream>

User::User(const std::string& name, const std::string& campusId)
    : name(name), campusId(campusId) {}

User::~User() {}

std::string User::getName() const {
    return name;
}

std::string User::getCampusId() const {
    return campusId;
}

double User::applyDiscount(double subtotal) const {
    return subtotal;
}

void User::printInfo() const {
    std::cout << "[" << getRole() << "] " << name << " (ID: " << campusId << ")" << std::endl;
}
