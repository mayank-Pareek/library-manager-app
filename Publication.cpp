#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <iomanip>
#include "Publication.h"

namespace sdds {
    Publication::Publication() {
        setEmpty();
    }

    void Publication::setEmpty() {
        m_title = nullptr;
        strcpy(m_shelfId,"\0");
        m_membership = 0;
        m_libRef = -1;
        resetDate();
    }

    Publication::~Publication() {
        if(this->m_title)
        delete[] this->m_title;
    }

    Publication& Publication::operator=(const Publication& publication) {
        set(publication.m_membership);
        strcpy(m_shelfId, publication.m_shelfId);
        setRef(publication.m_libRef);
        m_date = publication.m_date;
        if(m_title) {
            delete[] m_title;
            m_title = nullptr;
        }
        if (publication.m_title != nullptr) {
            m_title = new char[strlen(publication.m_title) + 1];
            strcpy(m_title, publication.m_title);
        }
        else m_title = nullptr;
        return *this;
    }

    Publication::Publication(const Publication& publication) {
        *this = publication;
    }

    void Publication::set(int member_id) {
        m_membership = member_id;
    }

    void Publication::setRef(int value) {
        m_libRef = value;
    }

    void Publication::resetDate() {
        m_date = Date();
    }

    char Publication::type() const {
        return 'P';
    }

    bool Publication::onLoan() const {
        return m_membership != 0;
    }

    Date Publication::checkoutDate() const {
        return m_date;
    }

    bool Publication::operator==(const char* title) const {
        return strstr(m_title, title) != nullptr;
    }

    Publication::operator const char* () const {
        return m_title;
    }

    int Publication::getRef() const {
        return m_libRef;
    }

    bool Publication::conIO(std::ios& io) const {
        if (&io == &std::cout || &io == &std::cin) {
            return true;
            }
        return false;
    }

    std::ostream& Publication::write(std::ostream& os) const {
        char title[SDDS_TITLE_WIDTH + 1] = "\0";
        std::strncpy(title, m_title, SDDS_TITLE_WIDTH);
        if (conIO(os)) {
            os << "| " << m_shelfId << " | ";
            os << std::setfill('.') << std::left << std::setw(30) << title << " | ";
            if (m_membership != 0) os << m_membership;
            else os<< " N/A ";
            os << " | " << m_date << " |";
        }
        else {
            os << type() << "\t";
            os << m_libRef << "\t" << m_shelfId << "\t" << m_title << "\t";
            if (m_membership != 0) os << m_membership;
            else os << "0";
            os << "\t" << m_date;
        }
        return os;
    }

    std::istream& Publication::read(std::istream& is) {
        char title[50];
        char shelfId[SDDS_SHELF_ID_LEN + 1];
        Date date;
        int libRef = -1, membership = 0;

        if (this->m_title!=nullptr) {
            delete[] this->m_title;
            m_title = nullptr;
        }
        setEmpty();

        if (conIO(is)) {

            std::cout << "Shelf No: ";
            is.getline(shelfId, SDDS_SHELF_ID_LEN + 1);


            if (strlen(shelfId) != SDDS_SHELF_ID_LEN) {
                is.setstate(std::ios::failbit);
            }
            std::cout << "Title: ";
            is.getline(title, 50);
            std::cout << "Date: ";
            is >> date;
        }

        else {
            is >> libRef;
            is.ignore();
            is.getline(shelfId, SDDS_SHELF_ID_LEN + 1, '\t');
            is.getline(title, 100, '\t');
            is >> membership;
            is.ignore();
            is >> date;
        }

        if (!date) {
            is.setstate(std::ios::failbit);
        }

        if (is) {
            m_title = new char[strlen(title) + 1];
            strcpy(this->m_title, title);
            m_date = date;
            strcpy(this->m_shelfId, shelfId);
            m_libRef = libRef;
            m_membership = membership;
        }

        return is;
    }

    Publication::operator bool() const {
        return (m_title != nullptr && m_shelfId[0] != '\0');
    }
}