#ifndef SDDS_LIBAPP_H
#define SDDS_LIBAPP_H
#include <iostream>
#include "Menu.h"
#include "Publication.h"
#include "Book.h"
#include "PublicationSelector.h"

namespace sdds {
	class LibApp {
		bool m_changed;
		Menu m_mainMenu;
		Menu m_exitMenu;
		char fileName[256];
		Publication* PPA[SDDS_LIBRARY_CAPACITY];//Publication pointers array
		int NOLP; //Number of loaded publications
		int LLRN; //Last library reference number
		Menu pubMenu;
		Menu searchMenu;
		bool confirm(const char* message);
		void load();
		void save();
		int search(int);
		void returnPub();
		void newPublication();
		void removePublication();
		void checkOutPub();
		Publication* getPub(int libRef);

	public:
		LibApp(const char*);
		~LibApp();
		void run();
	};
}

#endif