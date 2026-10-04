#include "CafeteriaPerishable.h"
#include <iomanip>

CafeteriaPerishable::CafeteriaPerishable(int id, const string& name, double price,
                                         const string& category, int stock,
                                         const string& expiryDate)
    : Resource(id, name, price, category, stock), expiryDate(expiryDate) {}

string CafeteriaPerishable::getExpiry() const { return expiryDate; }

void CafeteriaPerishable::printExtraInfo() const {
    const int w = 36;
    string border(w, '-');
    cout << "+" << border << "+" << endl;
    cout << "|  [Cafeteria Perishable]" << setw(w - 23) << " " << "|" << endl;
    cout << "|  Expiry   : " << setw(w - 13) << left << expiryDate << "|" << endl;
    cout << "+" << border << "+" << endl;
}

void CafeteriaPerishable::saveToFile(ofstream& ofs) const {
    ofs << "CAFE," << id << "," << name << "," << fixed << setprecision(2)
        << price << "," << category << "," << stock << "," << expiryDate << "\n";
}
