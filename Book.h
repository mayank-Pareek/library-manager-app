#include "Publication.h"
namespace sdds {
    class Book :
        public Publication {
        char* authorName;
    public:
        Book();
        ~Book();
        void setEmpty();
        Book(const Book& book);
        Book& operator=(const Book& book);
        char type() const;
        std::ostream& write(std::ostream& os) const;
        std::istream& read(std::istream& is);
        virtual void set(int mId);
        operator bool() const;
    };
}

