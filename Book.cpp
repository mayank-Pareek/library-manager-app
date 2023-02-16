#include <iostream>
#include <cstring>
#include <iomanip>
#include "Book.h"

namespace sdds {
    Book::Book() : Publication() {
        setEmpty();
    }
    Book::~Book() {
        delete[] authorName;
        authorName = nullptr;
    }
    void Book::setEmpty() {
        authorName = nullptr;
    }
    Book::Book(const Book& book) : Publication(book) {
        if (book.authorName) {
            authorName = new char[strlen(book.authorName) + 1];
            strcpy(authorName, book.authorName);
        }
        else setEmpty();
    }
    Book& Book::operator=(const Book& book) {

        Publication::operator=(book);
        if (authorName!=nullptr) {
            delete[] authorName;
            authorName = nullptr;
        }
        if (book.authorName) {
            authorName = new char[strlen(book.authorName) + 1];
            strcpy(authorName, book.authorName);
        }
        return *this;
    }
    char Book::type() const {
        return 'B';
    };

    std::ostream& Book::write(std::ostream& os) const {

        Publication::write(os);
        if (conIO(os)) {
            char author[SDDS_AUTHOR_WIDTH + 1]="\0";
            std::strncpy(author, authorName, SDDS_AUTHOR_WIDTH);
            os << " ";
            os.width(SDDS_AUTHOR_WIDTH);
            os << std::setfill(' ') << std::left << author << " |";
        }
        else {
            os <<"\t" << authorName;
        }
        return os;
    }

    std::istream& Book::read(std::istream& is) {

        char authorN[100] = "\0";
        Publication::read(is);

        delete[] authorName;
        authorName = nullptr;
        if (conIO(is)) {
            is.ignore(); 
            std::cout << "Author: ";
        }
        else {
            is.ignore(1000, '\t');
        }
        is.get(authorN, 100);
        if (!is.fail()) {
            authorName = new char[strlen(authorN) + 1];
            strcpy(authorName, authorN);
        }
        return is;
    }

    void Book::set(int mId) {
        Publication::set(mId);
        Publication::resetDate();
    }

    Book::operator bool() const {
        return (authorName!=nullptr && Publication::operator bool());
    }
}
