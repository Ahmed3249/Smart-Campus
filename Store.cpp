#include "Store.h"
#include "LabHardware.h"
#include "CafeteriaPerishable.h"
#include "BookstoreMedia.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <stdexcept>

// ─────────────────────────────────────────────
// Helper: split a string on a delimiter up to maxSplits times.
// The last element of the result may contain the remainder (including
// any additional delimiters), matching the "max 6 splits" requirement.
// ─────────────────────────────────────────────
static std::vector<std::string> splitCSV(const std::string& line, char delim, int maxSplits) {
    std::vector<std::string> tokens;
    std::string token;
    int splits = 0;
    for (char ch : line) {
        if (ch == delim && splits < maxSplits) {
            tokens.push_back(token);
            token.clear();
            ++splits;
        } else {
            token += ch;
        }
    }
    tokens.push_back(token);   // push last (possibly multi-comma) segment
    return tokens;
}

// ─────────────────────────────────────────────
// Destructor
// ─────────────────────────────────────────────
Store::~Store() {
    for (Resource* r : inventory) {
        delete r;
    }
    inventory.clear();
}

// ─────────────────────────────────────────────
// addResource
// ─────────────────────────────────────────────
void Store::addResource(Resource* r) {
    if (!r) {
        throw std::invalid_argument("Cannot add a null Resource to the Store.");
    }
    inventory.push_back(r);
}

// ─────────────────────────────────────────────
// findById
// ─────────────────────────────────────────────
Resource* Store::findById(int id) const {
    for (Resource* r : inventory) {
        if (r->getId() == id) {
            return r;
        }
    }
    return nullptr;
}

// ─────────────────────────────────────────────
// listAll
// ─────────────────────────────────────────────
void Store::listAll() const {
    if (inventory.empty()) {
        std::cout << "(Inventory is empty)\n";
        return;
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(28) << "Name"
              << std::setw(10) << "Price"
              << std::setw(8)  << "Stock"
              << "Category\n";
    std::cout << std::string(68, '-') << "\n";

    for (const Resource* r : inventory) {
        std::cout << std::left
                  << std::setw(6)  << r->getId()
                  << std::setw(28) << r->getName()
                  << "$" << std::setw(9) << r->getPrice()
                  << std::setw(8)  << r->getStock()
                  << "\n";
    }
    std::cout << std::string(68, '-') << "\n";
}

// ─────────────────────────────────────────────
// compareResources
// ─────────────────────────────────────────────
void Store::compareResources(int id1, int id2) const {
    Resource* r1 = findById(id1);
    Resource* r2 = findById(id2);

    if (!r1) {
        std::cout << "[!] Resource with ID " << id1 << " not found.\n";
        return;
    }
    if (!r2) {
        std::cout << "[!] Resource with ID " << id2 << " not found.\n";
        return;
    }

    std::cout << "Comparing: \"" << r1->getName()
              << "\" vs \"" << r2->getName() << "\"\n";

    if (*r1 > *r2) {
        std::cout << "  >> \"" << r1->getName()
                  << "\" is greater (price: $"
                  << std::fixed << std::setprecision(2) << r1->getPrice() << ")\n";
    } else if (*r2 > *r1) {
        std::cout << "  >> \"" << r2->getName()
                  << "\" is greater (price: $"
                  << std::fixed << std::setprecision(2) << r2->getPrice() << ")\n";
    } else {
        std::cout << "  >> Both resources have the same price ($"
                  << std::fixed << std::setprecision(2) << r1->getPrice() << ")\n";
    }
}

// ─────────────────────────────────────────────
// saveInventory
// ─────────────────────────────────────────────
void Store::saveInventory() const {
    std::ofstream ofs(inventoryFile);
    if (!ofs.is_open()) {
        std::cerr << "[!] Could not open " << inventoryFile << " for writing.\n";
        return;
    }

    for (const Resource* r : inventory) {
        r->saveToFile(ofs);
    }
    ofs.close();
    std::cout << "[+] Inventory saved (" << inventory.size() << " items).\n";
}

// ─────────────────────────────────────────────
// loadInventory
// Line format: TYPE,id,name,price,category,stock[,extra]
//   TYPE  → LAB | CAFE | BOOK
//   extra  → type-specific field (may itself contain commas)
// ─────────────────────────────────────────────
void Store::loadInventory() {
    std::ifstream ifs(inventoryFile);
    if (!ifs.is_open()) {
        std::cout << "[!] No existing inventory file found. Starting fresh.\n";
        return;
    }

    // Free any previously loaded resources before reloading
    for (Resource* r : inventory) {
        delete r;
    }
    inventory.clear();

    std::string line;
    int count = 0;

    while (std::getline(ifs, line)) {
        if (line.empty() || line[0] == '#') {
            continue;   // skip blank lines and comment lines
        }

        // Split into at most 7 tokens (6 commas = fields 0..6)
        // so the 7th token (index 6) holds the remainder including any commas
        std::vector<std::string> fields = splitCSV(line, ',', 6);

        if (fields.size() < 6) {
            std::cerr << "[!] Skipping malformed line: " << line << "\n";
            continue;
        }

        std::string type     = fields[0];
        int         id       = 0;
        std::string name     = fields[2];
        double      price    = 0.0;
        std::string category = fields[4];
        int         stock    = 0;
        std::string extra    = (fields.size() >= 7) ? fields[6] : "";

        try {
            id    = std::stoi(fields[1]);
            price = std::stod(fields[3]);
            stock = std::stoi(fields[5]);
        } catch (const std::exception& e) {
            std::cerr << "[!] Parse error on line: " << line
                      << "\n    Reason: " << e.what() << "\n";
            continue;
        }

        Resource* resource = nullptr;

        if (type == "LAB") {
            resource = new LabHardware(id, name, price, category, stock, extra);
        } else if (type == "CAFE") {
            resource = new CafeteriaPerishable(id, name, price, category, stock, extra);
        } else if (type == "BOOK") {
            resource = new BookstoreMedia(id, name, price, category, stock, extra);
        } else {
            std::cerr << "[!] Unknown resource type \"" << type
                      << "\" on line: " << line << "\n";
            continue;
        }

        inventory.push_back(resource);
        ++count;
    }

    ifs.close();
    std::cout << "[+] Inventory loaded (" << count << " items).\n";
}

// ─────────────────────────────────────────────
// getInventory
// ─────────────────────────────────────────────
const std::vector<Resource*>& Store::getInventory() const {
    return inventory;
}
