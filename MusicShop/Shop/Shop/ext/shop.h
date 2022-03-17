#ifndef SHOP_H
#define SHOP_H

//Includes
#include <iostream>
#include <vector>


class AlbumScheme {						//Klasa przechowuj¹ca informacje o albumie
protected:
	std::string m_NameOfAlbum{};		//nazwa albumu
	std::string m_NameOfArtist{};		//nazwa artysty/zespo³u
	std::string m_Genre{};				//Gatunek
	float m_Prize{};					//Cena
public:
	AlbumScheme(const std::string& albumName, const std::string& artistName, const std::string& genre, const float& prize)
		: m_NameOfAlbum(albumName), m_NameOfArtist(artistName), m_Genre(genre), m_Prize(prize) {}
};

class StockAlbum : public AlbumScheme {		//Klasa przechowuj¹ca informacje o albumie dostêpnym w magazynie
private:
	int m_InStock{};						//Iloœæ na magazynie
public:
	void addToStock(const int& amount);
	void sellAlbum();
};


class SoldAlbum : public AlbumScheme {			//Klasa przechowuj¹ca informacje o sprzedanym albumie
private:
	std::string m_ownerName;
	std::string m_dateOfPurchase;
public:

};

class User {									//Klasa przechowuj¹ca informacje o u¿ytkowniku
private:
	std::string m_Name;							//Login i nazwa
	std::string m_Password;						//Has³o
	std::vector<AlbumScheme> m_ownedAlbums;		//Kupione albumy
public:
	User(const std::string& username, const std::string& password)
		: m_Name(username), m_Password(password) {}
	const std::string getName() const { return this->m_Name; }
	const std::string getPassword() const { return this->m_Password; }
};


class Shop {							//Klasa dzia³aj¹ca jak "silnik" programu
private:
	
public:
	Shop() = delete;

	static void InitAlbums();
	static void InitUsers(std::vector<User>& users);
	static void InitLoggingSystem(const std::vector<User>& users);
};


#endif