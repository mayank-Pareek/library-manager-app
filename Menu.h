#ifndef SDDS_MENU_H
#define SDDS_MENU_H
#include <iostream>

namespace sdds {
	int const MAX_MENU_ITEMS = 20;

	class Menu;

	class MenuItem {
		char* m_item;
		MenuItem();
		MenuItem(const char*);
		~MenuItem();
		operator bool() const;
		operator const char* ();
		std::ostream& display(std::ostream& os = std::cout);
		friend class Menu;
	};

	class Menu {
		MenuItem m_title;
		MenuItem* m_items[MAX_MENU_ITEMS];
		int m_itemCount;

	public:

		Menu();
		Menu(const char*);
		~Menu();
		int run();
		int operator~();
		std::ostream& displayTitle(std::ostream& os = std::cout);
		std::ostream& displayMenu(std::ostream& os = std::cout);
		operator bool() const;
		Menu& operator<<(const char*);
		operator int() const;
		operator unsigned int() const;
		char* operator[](int i) const;
	};
	std::ostream& operator<<(std::ostream& os, Menu& item);
}

#endif