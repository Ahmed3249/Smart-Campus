#ifndef STAFF_H
#define STAFF_H

#include "User.h"
#include <string>

class Staff : public User {
public:
    static constexpr double DISCOUNT = 0.10;

    Staff(const std::string& name, const std::string& campusId);

    std::string getRole() const override;
    double applyDiscount(double subtotal) const override;
};

#endif // STAFF_H
