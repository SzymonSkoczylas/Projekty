#include "shop.h"

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

void Shop::initAlbums()
{
	AlbumScheme("Nevermind", "Nirvana", "Grunge", 70);
}

void Shop::initUsers()
{
	User user("user", "user");
	User admin("admin", "admin");
	this->users.push_back(user);
	this->users.push_back(admin);
}

void Shop::loggingSystem()
{
	std::string login, password;
	std::cout << "Login: ";
	std::cin >> login;
	std::cout << "Password: ";
	std::cin >> password;
	for (auto& u : this->users)
	{
		if (u.getName() == login && u.getPassword() == password)
		{
			std::cout << "Zalogowano!\n";
			return;
		}
	}
	std::cout << "Nie znaleziono usera!\n";
}
