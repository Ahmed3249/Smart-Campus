#ifndef ORDER_H
#define ORDER_H

#include <vector>
#include <string>
#include <fstream>
#include "Resource.h"
#include "User.h"

// Struct representing one line item in an order
struct OrderItem {
    Resource* resource;  // non-owning pointer (lifetime managed by Store)
    int quantity;

    double lineTotal() const {
        return resource->getPrice() * quantity;
    }
};

// Order composes OrderItem objects and associates with a User
class Order {
public:
    Order(User* user, bool isDormDelivery);

    // Add a resource and quantity to this order
    void addItem(Resource* res, int qty);

    // Compute the final total: discount + optional delivery fee
    double calculateTotal() const;

    // Print a formatted summary of all items to stdout
    void printSummary() const;

    // Append this order's details to "transactions.txt"
    void saveToFile() const;

private:
    std::vector<OrderItem> items;   // composition — Order owns these items
    User* user;                     // association — Order does NOT own User
    bool isDormDelivery;

    static constexpr double DELIVERY_FEE = 5.00;
};

#endif // ORDER_H
