#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include "LibApp.h"

namespace sdds {
	bool LibApp::confirm(const char* message) {
		bool confirm = false;
		int selection = 0;
		Menu newMenu(message);
		newMenu << "Yes";
		selection = newMenu.run();
		if (selection == 1) {
			confirm = true;
		}
		return confirm;
	}

	void LibApp::load() {
		std::cout << "Loading Data" << std::endl;

		std::ifstream infile(fileName);
		char type{};
		Publication* temp = nullptr;
		int i = 0, j = 0, newIndex = 0, flag = 0;
		for (i = 0;i < SDDS_LIBRARY_CAPACITY && flag == 0;i++) {
			if (PPA[i] == nullptr) {
				newIndex = i;
				j = newIndex;
				flag = 1;
			}
		}
		for (i = newIndex; infile; i++) {
			infile >> type;
			infile.ignore();
			if (infile) {
				if (type == 'P')
					temp = new Publication;
				else if (type == 'B')
					temp = new Book;
				infile >> *temp;
				if (temp) {
					PPA[j] = temp;
					j++;
					NOLP++;
				}
			}
		}
		LLRN = temp->getRef();
	}

	void LibApp::save() {
		std::cout << "Saving Data" << std::endl;
		std::ofstream outfile(fileName);
		int i = 0;
		for (i = 0;i < NOLP;i++) {
			if (PPA[i]->getRef() != 0) {
				outfile << *PPA[i];
				outfile << std::endl;
			}
		}
	}

	int LibApp::search(int searchType) {
		int i = 0;
		char searchTitle[256] = "\0";
		char pubType;
		PublicationSelector ps("Select one of the following found matches:", 15);
		int psSelection = 0;
		int searchSelection = searchType;
		if (searchSelection == 0) {
			std::cout << "Aborted!" << std::endl;
			return 0;
		}

		int typeSelection = pubMenu.run();
		if (typeSelection == 0) {
			std::cout << "Aborted!" << std::endl;
			return 0;
		}
		else if (typeSelection == 1) {
			pubType = 'B';
		}
		else pubType = 'P';
		std::cout << "Publication Title: ";
		std::cin.getline(searchTitle, 256);
		if (searchSelection == 1) {
			for (i = 0;i < NOLP;i++) {
				if ((PPA[i]->getRef() != 0) && (PPA[i]->type() == pubType) && (*PPA[i] == searchTitle)) {
					ps << *PPA[i];
				}
			}
		}
		else if (searchSelection == 2) {
			for (i = 0;i < NOLP;i++) {
				if ((PPA[i]->getRef() != 0) && (PPA[i]->type() == pubType) && (PPA[i]->onLoan() == 0) && (*PPA[i] == searchTitle)) {
					ps << *PPA[i];
				}
			}
		}
		else if (searchSelection == 3) {
			for (i = 0;i < NOLP;i++) {
				if ((PPA[i]->getRef() != 0) && (PPA[i]->type() == pubType) && (PPA[i]->onLoan() != 0) && (*PPA[i] == searchTitle)) {
					ps << *PPA[i];
				}
			}
		}
		if (ps) {
			ps.sort();
			std::cin.clear();
			while ((getchar()) != '\n');
			psSelection = ps.run();
			if (psSelection == 0) {
				std::cout << "Aborted!";
				std::cout<< std::endl;
				return 0;
			}
			else return psSelection;
		}
		else std::cout << "No matches found!\n";
		return 0;
	}

	void LibApp::returnPub() {
		bool isConfirm;
		int resRef;
		Publication* temp;
		Date date;
		int days = 0;
		double fine = 0;
		std::cout << "Return publication to the library" << std::endl;
		resRef = search(3);
		std::cin.clear();
		if (resRef) {
			temp = getPub(resRef);
			std::cout << *temp << std::endl;
			isConfirm = confirm("Return Publication?");
			if (isConfirm) {
				days = date - temp->checkoutDate();
				if (days > 15) {
					fine = (days - 15) * 0.5;
					std::cout << "Please pay $" << std::fixed << std::setprecision(2) << fine << " penalty for being " << (days - 15) << " days late!" << std::endl;
				}
				temp->set(0);
				std::cout << "Publication returned" << std::endl << std::endl;
				m_changed = true;
			}
		}
		std::cout << std::endl;
	}

	void LibApp::newPublication() {
		bool isConfirm;
		int pubType = 0;
		if (NOLP == SDDS_LIBRARY_CAPACITY) {
			std::cout << "Library is at its maximum capacity!\n\n";
		}
		else {
			std::cout << "Adding new publication to the library" << std::endl;
			Publication* pub = nullptr;
			pubType = pubMenu.run();
			if (pubType == 1) {
				pub = new Book;
			}
			else if (pubType == 2) {
				pub = new Publication;
			}
			else {
				std::cout << "Aborted!\n\n";
				return;
			}
			std::cin.clear();
			while ((getchar()) != '\n');
			if(pub)
			std::cin >> *pub;
			if (std::cin.fail()) {
				std::cin.clear();
				while ((getchar()) != '\n');
				std::cout << "Aborted!";
				return;
			}

			isConfirm = confirm("Add this publication to the library?");
			if (isConfirm) {
				if (*pub) {
					LLRN++;
					pub->setRef(LLRN);
					PPA[NOLP] = pub;
					NOLP++;
					m_changed = true;
					std::cout << "Publication added" << std::endl;
				}
				else {
					std::cout << "Failed to add publication!";
					delete pub;
					pub = nullptr;
				}
			}
			else {
				std::cout << "Aborted!";
				return;
			}
			std::cout << std::endl;
		}
	}

	void LibApp::removePublication() {
		int resRef = 0;
		bool isConfirm;
		Publication* temp;
		std::cout << "Removing publication from the library" << std::endl;
		resRef = search(1);
		if (resRef) {
			temp = getPub(resRef);
			std::cout << *temp << std::endl;
			isConfirm = confirm("Remove this publication from the library?");
			if (isConfirm) {
				temp->setRef(0);
				m_changed = true;
				std::cout << "Publication removed" << std::endl;
			}
		}
		std::cout << std::endl;
	}

	void LibApp::checkOutPub() {
		bool isConfirm;
		int resRef, memId = 0;
		Publication* temp;
		std::cout << "Checkout publication from the library" << std::endl;
		resRef = search(2);
		if (resRef) {
			temp = getPub(resRef);
			std::cout << *temp << std::endl;
			isConfirm = confirm("Check out publication?");
			if (isConfirm) {
				std::cout << "Enter Membership number: ";
				do {
					std::cin >> memId;
					if (memId < 10000 || memId>99999) {
						std::cout << "Invalid membership number, try again: ";
					}
				} while (memId < 10000 || memId>99999);
				temp->set(memId);
				m_changed = true;
				std::cout << "Publication checked out" << std::endl;
			}
		}
		std::cout << std::endl;
	}

	LibApp::LibApp(const char* fName) : m_mainMenu("Library Application"), m_exitMenu("Changes have been made to the data, what would you like to do?"), pubMenu("Choose the type of publication:"), searchMenu() {
		m_changed = false;
		m_mainMenu << "Add New Publication" << "Remove Publication" << "Checkout publication from library" << "Return publication to library";
		m_exitMenu << "Save changes and exit" << "Cancel and go back to the main menu";
		pubMenu << "Book" << "Publication";
		searchMenu << "Search all" << "Search Checkout Items Search only those publications which are on loan by library members" << "Search Available Items Search only those publications which are not on loan";
		int i = 0;
		NOLP = 0;
		LLRN = 0;
		for (i = 0;i < SDDS_LIBRARY_CAPACITY;i++) {
			PPA[i] = nullptr;
		}
		if (fName) {
			strcpy(this->fileName, fName);
		}
		load();
	}

	void LibApp::run() {
		int menuSelection = 0, exitSelection = 0;
		bool exit = false;
		while (exit == false) {
			menuSelection = m_mainMenu.run();
			if (menuSelection == 0) {
				if (m_changed == true) {
					exitSelection = m_exitMenu.run();
					if (exitSelection == 1) {
						save();
						exit = true;
					}
					else if (exitSelection == 0) {
						if (confirm("This will discard all the changes are you sure?")) {
							m_changed = false;
							exit = true;
						}
					}
				}
				else exit = true;
				std::cout << std::endl;
			}
			else if (menuSelection == 1) {
				newPublication();
			}
			else if (menuSelection == 2) {
				removePublication();
			}
			else if (menuSelection == 3) {
				checkOutPub();
			}
			else if (menuSelection == 4) {
				returnPub();
			}
		}
		std::cout << "-------------------------------------------" << std::endl;
		std::cout << "Thanks for using Seneca Library Application" << std::endl;
	}

	LibApp::~LibApp() {
		int i = 0;
		for (i = 0;i < NOLP;i++) {
			delete PPA[i];
		}
	}

	Publication* LibApp::getPub(int libRef) {
		int i = 0;
		for (i = 0;i < NOLP;i++) {
			if (PPA[i]->getRef() == libRef) {
				return PPA[i];
			}
		}
		return nullptr;
	}
}