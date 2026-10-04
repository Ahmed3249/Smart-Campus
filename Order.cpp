#include "Order.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

// ─────────────────────────────────────────────
// Constructor
// ─────────────────────────────────────────────
Order::Order(User* user, bool isDormDelivery)
    : user(user), isDormDelivery(isDormDelivery)
{
    if (!user) {
        throw std::invalid_argument("Order requires a valid (non-null) User.");
    }
}

// ─────────────────────────────────────────────
// addItem
// ─────────────────────────────────────────────
void Order::addItem(Resource* res, int qty) {
    if (!res) {
        throw std::invalid_argument("Cannot add a null Resource to an Order.");
    }
    if (qty <= 0) {
        throw std::invalid_argument("Quantity must be greater than zero.");
    }

    // Attempt to reserve stock via Resource::purchase()
    res->purchase(qty);

    OrderItem item;
    item.resource = res;
    item.quantity = qty;
    items.push_back(item);
}

// ─────────────────────────────────────────────
// calculateTotal
// ─────────────────────────────────────────────
double Order::calculateTotal() const {
    double subtotal = 0.0;
    for (const OrderItem& item : items) {
        subtotal += item.lineTotal();
    }

    // applyDiscount() returns the discounted amount to subtract
    double discount = user->applyDiscount(subtotal);
    double afterDiscount = subtotal - discount;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Subtotal          : $" << subtotal      << "\n";
    if (discount > 0.0) {
        std::cout << "  Discount applied  : -$" << discount << "\n";
    }
    std::cout << "  After discount    : $" << afterDiscount << "\n";

    double total = afterDiscount;
    if (isDormDelivery) {
        std::cout << "  Dorm delivery fee : +$" << DELIVERY_FEE << "\n";
        total += DELIVERY_FEE;
    }

    std::cout << "  ─────────────────────────────\n";
    std::cout << "  Order total       : $" << total << "\n";

    return total;
}

// ─────────────────────────────────────────────
// printSummary
// ─────────────────────────────────────────────
void Order::printSummary() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "=== Order Summary for " << user->getName()
              << " (ID: " << user->getCampusId() << ") ===\n";

    if (items.empty()) {
        std::cout << "  (no items in this order)\n";
        return;
    }

    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(28) << "Item"
              << std::setw(8)  << "Qty"
              << std::setw(12) << "Unit $"
              << "Line Total\n";
    std::cout << std::string(60, '-') << "\n";

    for (const OrderItem& item : items) {
        std::cout << std::left
                  << std::setw(6)  << item.resource->getId()
                  << std::setw(28) << item.resource->getName()
                  << std::setw(8)  << item.quantity
                  << std::setw(12) << item.resource->getPrice()
                  << item.lineTotal() << "\n";
    }
    std::cout << std::string(60, '-') << "\n";
}

// ─────────────────────────────────────────────
// saveToFile
// ─────────────────────────────────────────────
void Order::saveToFile() const {
    std::ofstream ofs("transactions.txt", std::ios::app);
    if (!ofs.is_open()) {
        std::cerr << "[!] Could not open transactions.txt for writing.\n";
        return;
    }

    ofs << std::fixed << std::setprecision(2);
    ofs << "--- Transaction ---\n";
    ofs << "Customer   : " << user->getName()
        << "  (Campus ID: " << user->getCampusId() << ")\n";
    ofs << "Dorm Deliv : " << (isDormDelivery ? "Yes" : "No") << "\n";

    double subtotal = 0.0;
    for (const OrderItem& item : items) {
        ofs << "  [" << item.resource->getId() << "] "
            << item.resource->getName()
            << " x" << item.quantity
            << " @ $" << item.resource->getPrice()
            << " = $" << item.lineTotal() << "\n";
        subtotal += item.lineTotal();
    }

    double discount = user->applyDiscount(subtotal);
    double total = subtotal - discount;
    if (isDormDelivery) {
        total += DELIVERY_FEE;
    }

    ofs << "Subtotal   : $" << subtotal << "\n";
    if (discount > 0.0) {
        ofs << "Discount   : -$" << discount << "\n";
    }
    if (isDormDelivery) {
        ofs << "Delivery   : +$" << DELIVERY_FEE << "\n";
    }
    ofs << "Total      : $" << total << "\n";
    ofs << "-------------------\n\n";

    ofs.close();
}
