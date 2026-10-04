#ifndef BOOKSTOREMEDIA_H
#define BOOKSTOREMEDIA_H

#include "Resource.h"

class BookstoreMedia : public Resource {
private:
    string author;

public:
    BookstoreMedia(int id, const string& name, double price, const string& category,
                   int stock, const string& author);

    string getAuthor() const;

    void printExtraInfo() const override;
    void saveToFile(ofstream& ofs) const override;
};

#endif // BOOKSTOREMEDIA_H
