#include <iostream>
#include <fstream>
#include <stdexcept>
#include <limits>
#include <iomanip>

#include "Store.h"
#include "Order.h"
#include "User.h"
#include "Student.h"
#include "Staff.h"
#include "LabHardware.h"
#include "CafeteriaPerishable.h"
#include "BookstoreMedia.h"

using namespace std;

// ─────────────────────────────────────────────
//  PAYMENT HELPER
// ─────────────────────────────────────────────
void processPayment(double total) {
    cout << "\nTotal Due: $" << fixed << setprecision(2) << total << "\n";
    cout << "Payment method: (1) Cash  (2) Card\n> ";
    int choice; cin >> choice;

    if (choice == 2) {
        cout << "Enter 16-digit card number: ";
        string card; cin >> card;

        if (card.size() != 16 || card.find_first_not_of("0123456789") != string::npos)
            throw invalid_argument("Invalid card number. Must be exactly 16 numeric digits.");

        cout << "[+] Card payment of $" << fixed << setprecision(2) << total << " approved.\n";
    } else {
        cout << "[+] Cash payment of $" << fixed << setprecision(2) << total << " received.\n";
    }
}

// ─────────────────────────────────────────────
//  USER CREATION HELPER
// ─────────────────────────────────────────────
User* createUser() {
    string name, id;
    int role;
    cout << "\nUser type: (1) Student  (2) Staff\n> ";
    cin >> role;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Name: ";
    getline(cin, name);
    cout << "Campus ID: ";
    getline(cin, id);

    if (role == 2) return new Staff(name, id);
    return new Student(name, id);
}

// ─────────────────────────────────────────────
//  PLACE ORDER FLOW
// ─────────────────────────────────────────────
void placeOrder(Store& store) {
    User* user = createUser();

    cout << "Delivery: (1) Pickup  (2) Dorm Delivery\n> ";
    int delivery; cin >> delivery;
    bool isDorm = (delivery == 2);

    Order order(user, isDorm);
    store.listAll();

    while (true) {
        cout << "\nAdd item by ID (0 to finish): ";
        int id; cin >> id;
        if (id == 0) break;

        Resource* res = store.findById(id);
        if (!res) { cout << "Resource not found.\n"; continue; }

        cout << "Quantity: ";
        int qty; cin >> qty;

        try {
            res->purchase(qty);
            order.addItem(res, qty);
            cout << "[+] Added to order.\n";
        } catch (const runtime_error& e) {
            cout << "[!] " << e.what() << "\n";
        }
    }

    order.printSummary();
    double total = order.calculateTotal();

    try {
        processPayment(total);
        order.saveToFile();
        store.saveInventory();
    } catch (const invalid_argument& e) {
        cout << "[!] Payment failed: " << e.what() << "\n";
        cout << "    Order cancelled.\n";
    }

    delete user;
}

// ─────────────────────────────────────────────
//  SEED DEFAULT INVENTORY
// ─────────────────────────────────────────────
void seedInventory(Store& store) {
    if (!store.getInventory().empty()) return;
    store.addResource(new LabHardware(1, "Oscilloscope",               850.00, "Lab Hardware", 5,  "2 Years"));
    store.addResource(new LabHardware(2, "Arduino Kit",                45.00, "Lab Hardware", 20, "1 Year"));
    store.addResource(new CafeteriaPerishable(3, "Sandwich",            3.50, "Cafeteria Perishable", 50,  "2026-05-15"));
    store.addResource(new CafeteriaPerishable(4, "Juice Box",           1.25, "Cafeteria Perishable", 100, "2026-06-01"));
    store.addResource(new BookstoreMedia(5, "Data Structures Textbook", 60.00, "Bookstore Media", 15, "Mark Allen Weiss"));
    store.addResource(new BookstoreMedia(6, "OOP in C++",              45.00, "Bookstore Media", 10, "Robert Lafore"));
    cout << "[+] Default inventory seeded.\n";
}

// ─────────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────────
int main() {
    cout << "╔══════════════════════════════════════╗\n";
    cout << "║     SMART CAMPUS ECOSYSTEM v1.0      ║\n";
    cout << "║  Cairo University | CS213 | OOP      ║\n";
    cout << "╚══════════════════════════════════════╝\n\n";

    Store store;

    try {
        store.loadInventory();
    } catch (const runtime_error& e) {
        cout << "[!] " << e.what() << "\n";
    }

    seedInventory(store);

    int choice = -1;
    while (choice != 0) {
        cout << "\n╔═══════════ MAIN MENU ═══════════╗\n";
        cout << "║ 1. View Inventory                ║\n";
        cout << "║ 2. Resource Report (by ID)       ║\n";
        cout << "║ 3. Restock a Resource            ║\n";
        cout << "║ 4. Compare Resource Costs        ║\n";
        cout << "║ 5. Place an Order                ║\n";
        cout << "║ 6. Save Inventory                ║\n";
        cout << "║ 0. Exit                          ║\n";
        cout << "╚══════════════════════════════════╝\n";
        cout << "> ";
        cin >> choice;

        switch (choice) {

        case 1:
            store.listAll();
            break;

        case 2: {
            cout << "Enter Resource ID: ";
            int id; cin >> id;
            Resource* r = store.findById(id);
            if (r) r->fullReport();
            else   cout << "[!] Resource not found.\n";
            break;
        }

        case 3: {
            cout << "Enter Resource ID to restock: ";
            int id; cin >> id;
            Resource* r = store.findById(id);
            if (!r) { cout << "[!] Resource not found.\n"; break; }
            cout << "Quantity to add: ";
            int qty; cin >> qty;
            r->restock(qty);
            try { store.saveInventory(); }
            catch (const runtime_error& e) { cout << "[!] " << e.what() << "\n"; }
            break;
        }

        case 4: {
            cout << "Enter first Resource ID: ";
            int id1; cin >> id1;
            cout << "Enter second Resource ID: ";
            int id2; cin >> id2;
            store.compareResources(id1, id2);
            break;
        }

        case 5:
            try {
                placeOrder(store);
            } catch (const exception& e) {
                cout << "[!] Order error: " << e.what() << "\n";
            }
            break;

        case 6:
            try {
                store.saveInventory();
            } catch (const runtime_error& e) {
                cout << "[!] " << e.what() << "\n";
            }
            break;

        case 0:
            cout << "Goodbye!\n";
            break;

        default:
            cout << "[!] Invalid option.\n";
        }
    }

    return 0;
}
