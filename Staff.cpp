#include "Staff.h"
#include <iostream>
#include <iomanip>

Staff::Staff(const std::string& name, const std::string& campusId)
    : User(name, campusId) {}

std::string Staff::getRole() const {
    return "Staff";
}

double Staff::applyDiscount(double subtotal) const {
    double discountAmount = subtotal * DISCOUNT;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Staff discount (10%) applied: -$" << discountAmount << std::endl;
    return subtotal * 0.90;
}
