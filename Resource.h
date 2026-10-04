#ifndef RESOURCE_H
#define RESOURCE_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

using namespace std;

class Resource {
protected:
    int id;
    string name;
    double price;
    string category;
    int stock;

public:
    Resource(int id, const string& name, double price, const string& category, int stock);
    virtual ~Resource();

    // Getters
    int getId() const;
    string getName() const;
    double getPrice() const;
    string getCategory() const;
    int getStock() const;

    void restock(int qty);
    void purchase(int qty);

    virtual void printExtraInfo() const = 0;
    virtual void printReport() const;
    void fullReport() const;

    bool operator>(const Resource& other) const;
    friend ostream& operator<<(ostream& os, const Resource& r);

    virtual void saveToFile(ofstream& ofs) const = 0;
};

#endif // RESOURCE_H
