#include "shop.h"

//Macra dla systemowych komend
#define clearScreen system("cls")
#define pressAnyKey system("pause")

//Deklaracja statycznych zmiennych klasy Shop
std::ifstream						Shop::inUserFile;
std::ofstream						Shop::outUserFile;

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
	user->createUser("user", "user");
	user->setPermission(0);
	user->createUser("admin", "admin");
	user->setPermission(1);
}

void Shop::MenuInterface(const User& user)
{
	clearScreen;
	if (user.getPermission() == userPermission::ADMIN)
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

void Shop::RegisterUser(const std::string& login, const std::string& pass)
{
	std::string line;
	std::stringstream ss;
	char ph{};
	std::string userName{};

	inUserFile.open(userFileName);
	while (std::getline(inUserFile,line))
	{
		ss.str(std::string());
		ss << line;
		if (line[0] == '[')
		{
			ss >> ph >> userName >> ph;
			if (userName == login)
			{
				std::cout << "Istnieje uzytkownik o podanym loginie!\n";
				pressAnyKey;
				inUserFile.close();
				return;
			}
		}
	}
	inUserFile.close();

	outUserFile.open(userFileName, std::ios_base::app);
	if (outUserFile.is_open())
	{
		outUserFile << "[ " << login << " ]\n";
		outUserFile << login << " " << pass <<'\n';
		outUserFile << "Purchase history:\n";
		outUserFile.close();

		user->createUser(login, pass);
		user->setPermission(0);

		std::cout << "Utworzono uzytkownika!\n";
		outUserFile.close();
	}
	else
		std::cout << "Blad odczytu pliku!\n";
	system("pause");
}

void Shop::LoggingSystem()
{
	bool logginInProccess = true;
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
			for (;;) //Sprawdzanie czy uzytkownik istnieje DO ZROBIENIA !!!
			{
			}
			std::cout << "Nie znaleziono usera!\n";
			break;
		case '2':
			clearScreen;
			std::cout << "Wprowadz login: ";
			std::cin >> login;
			std::cout << "Wprowadz haslo: ";
			std::cin >> password;
			clearScreen;
			RegisterUser(login, password);
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

void User::createUser(const std::string& username, const std::string& password)
{
	this->m_Name = username;
	this->m_Password = password;
}

void User::setPermission(bool rights)
{
	if (rights)
		this->m_rights = userPermission::ADMIN;
	else
		this->m_rights = userPermission::USER;
}
