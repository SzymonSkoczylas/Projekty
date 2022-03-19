#include "shop.h"

//Macra dla systemowych komend
#define clearScreen system("cls")
#define pressAnyKey system("pause")

//Deklaracja statycznych zmiennych klasy Shop
std::ifstream						Shop::inUserFile;
std::ofstream						Shop::outUserFile;
userPermission						Shop::loggedUserRights;

//Zmienne globalne
const std::string		userFileName = "data\\users_data.txt";
std::unique_ptr<User>	user = std::make_unique<User>();

void StockAlbum::addToStock(const int& amount)
{
	this->m_InStock += amount;
}

void StockAlbum::sellAlbum()
{
	if (this->m_InStock > 0)
		this->m_InStock -= 1;
}

void Shop::InitAlbums()
{
	AlbumScheme* album;
	album = new AlbumScheme("Nevermind", "Nirvana", "Grunge", 70);
}

void Shop::InitUsers()
{
	user->createUser("admin", "admin", userPermission::ADMIN);
	user->createUser("user", "user");
}

void Shop::MenuInterface(const userPermission& user)
{
	clearScreen;
	if (user == userPermission::ADMIN)
	{
		std::cout << "   Zalogowano na koncie admina!\n";
		std::cout << "1. Przejrzyj zawartosc magazynu\n";
		std::cout << "2. Dodaj album do magazynu\n";
		std::cout << "3. Przejrzyj historie zamowien\n";
	}
	else
	{
		std::cout << "   Zalogowano na koncie usera!\n";
		std::cout << "1. Zasil saldo\n";
		std::cout << "2. Przegladaj albumy\n";
		std::cout << "3. Przejrzyj swoja historie zamowien\n";
	}
}


bool Shop::LookForUser(const std::string& username, const std::string& password, userPermission& rights)
{
	inUserFile.open(userFileName);
	std::stringstream ss;
	std::string line;
	char ph{};
	std::string name,pass,temp;

	clearScreen;
	while (std::getline(inUserFile, line))
	{
		ss.str(std::string());
		ss << line;
		ss >> ph >> name;
		if (username == name)
		{
			ss.str(std::string());
			std::getline(inUserFile, line);
			ss << line;
			ss >> name >> pass >> temp;
			if (pass == password)
			{
				std::cout << "Poprawnie zalogowano!\n";
				if (temp == "ADMIN") 
					rights = userPermission::ADMIN;
				else 
					rights = userPermission::USER;
				inUserFile.close();
				return true;
			}
			else
			{
				std::cout << "Wprowadz poprawne haslo!\n";
				inUserFile.close();
				return false;
			}
		}
	}

	std::cout << "Nie znaleziono usera!\n";
	return false;
}

void Shop::LoggingSystem()
{
	bool logginInProccess = true;
	bool userFound;
	std::string login, password;
	char symbol{};
	while (logginInProccess) {
		clearScreen;
		std::cout << "1. Zaloguj\t2.Zarejestruj sie\t3.Zakoncz\n";
		std::cin >> symbol;
		switch (symbol)
		{
		case '1':
			clearScreen;
			std::cout << "Login: ";
			std::cin >> login;
			std::cout << "Haslo: ";
			std::cin >> password;
			userFound = LookForUser(login, password, loggedUserRights);
			if(userFound) 
				MenuInterface(loggedUserRights);
			pressAnyKey;
			break;
		case '2':
			clearScreen;
			std::cout << "Wprowadz login: ";
			std::cin >> login;
			std::cout << "Wprowadz haslo: ";
			std::cin >> password;
			clearScreen;
			user->createUser(login, password);
			pressAnyKey;
			break;
		case '3':
			return;
		default:
			clearScreen;
			std::cout << "Zly input";
			break;
		}
	}
}

void User::createUser(const std::string& username, const std::string& password, const userPermission& rights)
{
	this->m_Name = username;
	this->m_Password = password;
	this->m_rights = rights;


	std::string line;
	std::stringstream ss;
	char ph{};
	std::string name{};

	std::ifstream inUserFile;

	inUserFile.open(userFileName);
	while (std::getline(inUserFile, line))
	{
		ss.str(std::string());
		ss << line;
		if (line[0] == '[')
		{
			ss >> ph >> name;
			if (name == username)
			{
				inUserFile.close();
				return;
			}
		}
	}
	inUserFile.close();

	std::string srights;
	std::ofstream outUserFile;

	if (rights == userPermission::USER)  srights = "USER";
	if (rights == userPermission::ADMIN) srights = "ADMIN";

	outUserFile.open(userFileName, std::ios_base::app);
	if (outUserFile.is_open())
	{
		outUserFile << "[ " << username << " ]\n";
		outUserFile << username << " " << password << " " << srights << '\n';
		outUserFile << "Purchase history:\n";
		outUserFile.close();

		std::cout << "Utworzono uzytkownika!\n";
	}
	else
		std::cout << "Blad odczytu pliku!\n";
	outUserFile.close();
}

void User::setPermission(bool rights)
{
	if (rights)
		this->m_rights = userPermission::ADMIN;
	else
		this->m_rights = userPermission::USER;
}
