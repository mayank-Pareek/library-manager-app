#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <iomanip>
#include "Menu.h"

namespace sdds {
	MenuItem::MenuItem() {
		m_item = nullptr;
	}

	MenuItem::MenuItem(const char* item) {
		m_item = new char[strlen(item) + 1];
		strcpy(m_item, item);
	}

	MenuItem::~MenuItem() {
		delete[] m_item;
	}

	MenuItem::operator bool() const {
		return m_item != nullptr;
	}

	MenuItem::operator const char* () {
		return m_item;
	}

	std::ostream& MenuItem::display(std::ostream& os) {
		os << m_item;
		return os;
	}

	Menu::Menu() {
		int i = 0;
		for (i = 0;i < MAX_MENU_ITEMS;i++) {
			m_items[i] = nullptr;
		}
		m_title.m_item = nullptr;
		m_itemCount = 0;
	}

	Menu::Menu(const char* title) {
		int i = 0;
		m_itemCount = 0;
		for (i = 0;i < MAX_MENU_ITEMS;i++) {
			m_items[i] = nullptr;
		}
		m_title.m_item = new char[strlen(title) + 1];
		strcpy(m_title.m_item, title);
	}

	Menu::~Menu() {
		int i = 0;
		for (i = 0; i < m_itemCount; i++) {
			delete m_items[i];
		}
	}

	std::ostream& Menu::displayTitle(std::ostream& os) {
		if (m_title.m_item != nullptr) {
			m_title.display();
		}
		return os;
	}

	std::ostream& Menu::displayMenu(std::ostream& os) {
		int i = 0;
		if (m_title.m_item != nullptr) {
			m_title.display();
			os << "\n";
		}

		for (i = 0; i < m_itemCount; i++) {
			if (i < 9) {
				os << " " << i + 1;
			}
			else {
				os << i + 1;
			}
			os << "- ";
			if (m_items[i] != nullptr) {
				m_items[i]->display();
			}
			std::cout << std::endl;
		}
		os << " 0- Exit\n> ";
		return os;
	}

	int Menu::run()
	{
		displayMenu(std::cout);
		int selection = 0;
		std::cin >> selection;
		while (selection < 0 || selection > m_itemCount || std::cin.fail())
		{
			std::cout << "Invalid Selection, try again: ";
			std::cin.clear();
			while ((getchar()) != '\n');
			std::cin >> selection;
		}
		return selection;
	}

	int Menu::operator~()
	{
		return run();
	}

	Menu& Menu::operator<<(const char* item)
	{
		if (m_itemCount < 20 && item != nullptr) {
			MenuItem* temp = new MenuItem(item);
			m_items[m_itemCount] = temp;
			m_itemCount += 1;
		}
		return *this;
	}

	Menu::operator bool() const
	{
		return m_itemCount > 0;
	}

	Menu::operator unsigned int() const {
		return m_itemCount;
	}

	Menu::operator int() const {
		return m_itemCount;
	}

	char* Menu::operator[](int i) const {
		if (i + 1 > m_itemCount) {
			return m_items[i % m_itemCount]->m_item;
		}
		return m_items[i]->m_item;
	}

	std::ostream& operator<<(std::ostream& os, Menu& menu) {
		return menu.displayTitle(os);
	}
}