#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include <string>

class Student : public User {
public:
    Student(const std::string& name, const std::string& campusId);

    std::string getRole() const override;
};

#endif // STUDENT_H
