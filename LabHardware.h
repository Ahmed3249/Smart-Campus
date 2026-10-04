#ifndef LABHARDWARE_H
#define LABHARDWARE_H

#include "Resource.h"

class LabHardware : public Resource {
private:
    string warrantyInfo;

public:
    LabHardware(int id, const string& name, double price, const string& category,
                int stock, const string& warrantyInfo);

    string getWarranty() const;

    void printExtraInfo() const override;
    void saveToFile(ofstream& ofs) const override;
};

#endif // LABHARDWARE_H
