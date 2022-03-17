#include "shop.h"

#define clear system("cls")

void StockAlbum::addToStock(const int& amount)
{
	this->m_InStock += amount;
}

void StockAlbum::sellAlbum()
{
	if (this->m_InStock > 0)
	{
		this->m_InStock -= 1;
	}
}

//Deklaracja statycznej zmiennej
std::vector<User> Shop::users;

void Shop::InitAlbums()
{
	AlbumScheme* album;
	album = new AlbumScheme("Nevermind", "Nirvana", "Grunge", 70);
}

void Shop::InitUsers()
{
	User user("user", "user");
	User admin("admin", "admin");
	user.setPermission(0);
	admin.setPermission(1);
	users.push_back(user);
	users.push_back(admin);
}

void Shop::MenuInterface(const User& user)
{
	clear;
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

void Shop::LoggingSystem()
{
	std::string login, password;
	char symbol{};
	std::cout << "1. Zaloguj\t2.Zarejestruj sie\n";
	std::cin >> symbol;
	switch (symbol)
	{
	case '1':
		clear;
		std::cout << "Login: ";
		std::cin >> login;
		std::cout << "Haslo: ";
		std::cin >> password;
		for (auto& u : users)
		{
			if (u.getName() == login && u.getPassword() == password)
			{
				MenuInterface(u);
				return;
			}
		}
		std::cout << "Nie znaleziono usera!\n";
		break;
	case '2':
		clear;
		std::cout << "Wprowadza login: ";
		std::cin >> login;
		std::cout << "Wprowadz haslo: ";
		std::cin >> password;
		clear;
		break;
	default:
		clear;
		std::cout << "Zly input";
		break;
	}
}

void User::setPermission(bool rights)
{
	if (rights)
		this->m_rights = userPermission::ADMIN;
	else
		this->m_rights = userPermission::USER;
}
