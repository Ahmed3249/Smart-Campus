#include "Resource.h"
#include <iomanip>

Resource::Resource(int id, const string& name, double price, const string& category, int stock)
    : id(id), name(name), price(price), category(category), stock(stock) {}

Resource::~Resource() {}

int Resource::getId() const { return id; }
string Resource::getName() const { return name; }
double Resource::getPrice() const { return price; }
string Resource::getCategory() const { return category; }
int Resource::getStock() const { return stock; }

void Resource::restock(int qty) {
    stock += qty;
    cout << "Restocked \"" << name << "\" by " << qty << " units. New stock: " << stock << endl;
}

void Resource::purchase(int qty) {
    if (qty > stock) {
        throw runtime_error("Insufficient stock for \"" + name + "\". Requested: " +
                            to_string(qty) + ", Available: " + to_string(stock));
    }
    stock -= qty;
}

void Resource::printReport() const {
    const int w = 36;
    string border(w, '=');
    cout << "+" << border << "+" << endl;
    cout << "|" << setw(w) << left << "  RESOURCE REPORT" << "|" << endl;
    cout << "+" << border << "+" << endl;
    cout << "|  ID       : " << setw(w - 13) << left << id        << "|" << endl;
    cout << "|  Name     : " << setw(w - 13) << left << name      << "|" << endl;
    cout << "|  Price    : " << setw(w - 13) << left << fixed << setprecision(2) << price << "|" << endl;
    cout << "|  Category : " << setw(w - 13) << left << category  << "|" << endl;
    cout << "|  Stock    : " << setw(w - 13) << left << stock     << "|" << endl;
    cout << "+" << border << "+" << endl;
}

void Resource::fullReport() const {
    printReport();
    printExtraInfo();
}

bool Resource::operator>(const Resource& other) const {
    return (price * stock) > (other.price * other.stock);
}

ostream& operator<<(ostream& os, const Resource& r) {
    os << r.id << "," << r.name << "," << fixed << setprecision(2)
       << r.price << "," << r.category << "," << r.stock;
    return os;
}
