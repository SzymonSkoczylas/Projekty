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

void Shop::InitAlbums()
{
	AlbumScheme* album;
	album = new AlbumScheme("Nevermind", "Nirvana", "Grunge", 70);
}

void Shop::InitUsers(std::vector<User>& users)
{
	User user("user", "user");
	User admin("admin", "admin");
	users.push_back(user);
	users.push_back(admin);
}

void Shop::InitLoggingSystem(const std::vector<User>& users)
{
	std::string login, password;
	std::cout << "Login: ";
	std::cin >> login;
	std::cout << "Password: ";
	std::cin >> password;
	for (auto& u : users)
	{
		if (u.getName() == login && u.getPassword() == password)
		{
			std::cout << "Zalogowano!\n";
			return;
		}
	}
	std::cout << "Nie znaleziono usera!\n";
}
