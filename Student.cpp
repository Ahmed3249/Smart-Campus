#include "Student.h"

Student::Student(const std::string& name, const std::string& campusId)
    : User(name, campusId) {}

std::string Student::getRole() const {
    return "Student";
}
