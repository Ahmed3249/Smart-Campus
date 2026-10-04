#ifndef CAFETERIAPERISHABLE_H
#define CAFETERIAPERISHABLE_H

#include "Resource.h"

class CafeteriaPerishable : public Resource {
private:
    string expiryDate;

public:
    CafeteriaPerishable(int id, const string& name, double price, const string& category,
                        int stock, const string& expiryDate);

    string getExpiry() const;

    void printExtraInfo() const override;
    void saveToFile(ofstream& ofs) const override;
};

#endif // CAFETERIAPERISHABLE_H
