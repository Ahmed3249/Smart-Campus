#ifndef STORE_H
#define STORE_H

#include <vector>
#include <string>
#include "Resource.h"

// Store owns its Resource* pointers (allocated with new, freed in destructor).
// Conceptually this is aggregation from the caller's perspective, but the
// Store manages the lifetime of every Resource it holds.
class Store {
public:
    // Destructor frees every Resource* in inventory
    ~Store();

    // Take ownership of a heap-allocated Resource
    void addResource(Resource* r);

    // Return pointer to resource with matching id, or nullptr if not found
    Resource* findById(int id) const;

    // Print a formatted table of all resources in inventory
    void listAll() const;

    // Use operator> to compare two resources; print the result
    void compareResources(int id1, int id2) const;

    // Write entire inventory to inventoryFile using each resource's saveToFile()
    void saveInventory() const;

    // Parse inventoryFile and recreate Resource objects
    // Format per line: TYPE,id,name,price,category,stock[,extra]
    void loadInventory();

    // Read-only access to the raw inventory vector
    const std::vector<Resource*>& getInventory() const;

private:
    std::vector<Resource*> inventory;
    std::string inventoryFile = "inventory.txt";
};

#endif // STORE_H
