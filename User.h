#ifndef USER_H
#define USER_H

#include <string>

class User {
protected:
    std::string name;
    std::string campusId;

public:
    User(const std::string& name, const std::string& campusId);
    virtual ~User();

    std::string getName() const;
    std::string getCampusId() const;

    virtual double applyDiscount(double subtotal) const;
    virtual std::string getRole() const = 0;

    void printInfo() const;
};

#endif // USER_H
