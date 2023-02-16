#ifndef PUBLICATION_H
#define PUBLICATION_H
#include <iostream>
#include "Streamable.h"
#include "Date.h"
#include "Lib.h"

namespace sdds {
    class Publication : public Streamable {
        char* m_title;
        char m_shelfId[SDDS_SHELF_ID_LEN + 1];
        int m_membership;
        int m_libRef;
        Date m_date;
    public:
        Publication();
        void setEmpty();
        ~Publication();

        Publication(const Publication& publication);
        Publication& operator=(const Publication& publication);


        virtual void set(int);

        void setRef(int value);

        void resetDate();

        virtual char type() const;

        bool onLoan() const;

        Date checkoutDate() const;

        bool operator==(const char* title) const;

        operator const char* () const;

        int getRef() const;

        bool conIO(std::ios& io) const;

        std::ostream& write(std::ostream& os) const;

        std::istream& read(std::istream& is);

        operator bool() const;
    };
}

#endif