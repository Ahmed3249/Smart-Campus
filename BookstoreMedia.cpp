#include "BookstoreMedia.h"
#include <iomanip>

BookstoreMedia::BookstoreMedia(int id, const string& name, double price,
                               const string& category, int stock,
                               const string& author)
    : Resource(id, name, price, category, stock), author(author) {}

string BookstoreMedia::getAuthor() const { return author; }

void BookstoreMedia::printExtraInfo() const {
    const int w = 36;
    string border(w, '-');
    cout << "+" << border << "+" << endl;
    cout << "|  [Bookstore Media]" << setw(w - 19) << " " << "|" << endl;
    cout << "|  Author   : " << setw(w - 13) << left << author << "|" << endl;
    cout << "+" << border << "+" << endl;
}

void BookstoreMedia::saveToFile(ofstream& ofs) const {
    ofs << "BOOK," << id << "," << name << "," << fixed << setprecision(2)
        << price << "," << category << "," << stock << "," << author << "\n";
}
