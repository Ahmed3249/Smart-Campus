#include "LabHardware.h"
#include <iomanip>

LabHardware::LabHardware(int id, const string& name, double price, const string& category,
                         int stock, const string& warrantyInfo)
    : Resource(id, name, price, category, stock), warrantyInfo(warrantyInfo) {}

string LabHardware::getWarranty() const { return warrantyInfo; }

void LabHardware::printExtraInfo() const {
    const int w = 36;
    string border(w, '-');
    cout << "+" << border << "+" << endl;
    cout << "|  [Lab Hardware]" << setw(w - 16) << " " << "|" << endl;
    cout << "|  Warranty : " << setw(w - 13) << left << warrantyInfo << "|" << endl;
    cout << "+" << border << "+" << endl;
}

void LabHardware::saveToFile(ofstream& ofs) const {
    ofs << "LAB," << id << "," << name << "," << fixed << setprecision(2)
        << price << "," << category << "," << stock << "," << warrantyInfo << "\n";
}
