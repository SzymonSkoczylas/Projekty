#ifndef SHOP_H
#define SHOP_H

//Includes
#include <iostream>
#include <vector>
#include <fstream>


class AlbumScheme {						//Klasa przechowujaca informacje o albumie
protected:
	std::string m_NameOfAlbum{};		//nazwa albumu
	std::string m_NameOfArtist{};		//nazwa artysty/zespolu
	std::string m_Genre{};				//Gatunek
	float m_Prize{};					//Cena
public:
	AlbumScheme(const std::string& albumName, const std::string& artistName, const std::string& genre, const float& prize)
		: m_NameOfAlbum(albumName), m_NameOfArtist(artistName), m_Genre(genre), m_Prize(prize) {}
};

class StockAlbum : public AlbumScheme {		//Klasa przechowujaca informacje o albumie dostepnym w magazynie
private:
	int m_InStock{};						//Ilosc na magazynie
public:
	void addToStock(const int& amount);
	void sellAlbum();
};


class SoldAlbum : public AlbumScheme {			//Klasa przechowujaca informacje o sprzedanym albumie
private:
	std::string m_ownerName;
	std::string m_dateOfPurchase;
public:

};


enum class userPermission { USER = 0, ADMIN = 1 };

class User {									//Klasa przechowujaca informacje o uzytkowniku
private:
	std::string m_Name;							//Login i nazwa
	std::string m_Password;						//Haslo
	std::vector<AlbumScheme> m_ownedAlbums;		//Kupione albumy
	userPermission m_rights = userPermission::USER;
public:
	User(const std::string& username, const std::string& password)
		: m_Name(username), m_Password(password) {}
	const std::string getName() const { return this->m_Name; }
	const std::string getPassword() const { return this->m_Password; }
	const userPermission getPermission() const { return this->m_rights; }

	void setPermission(bool rights);
};


class Shop {							//Klasa dzia³aj¹ca jak "silnik" programu
private:
	static std::vector<User> users;
public:
	Shop() = delete;

	/*
	* Funkcja InitAlbums inicjalizujaca albumy do programu.
	* Wstepnie czyta wpisane "z palca" przez programiste wartosci
	* w przyszlosci beda one zapisywane i wczytywane z odpowiedniego pliku
	*/
	static void InitAlbums();

	/*
	* Funkcja InitUsers inicjalizujaca uzytkownikow do programu
	* Wstepnie czyta wpisane "z palca" przez programiste konta
	* w przyszlosci beda one zapisywane i wczytywane z odowiedniego pliku
	*/
	static void InitUsers();

	/*
	* Funkcja LoggingSystem wczytujaca ekran logowania
	* system sprawdza czy dane konto istnieje
	* w przyszlosci po zalogowaniu wyswietli sie odpowiedni interfejs
	* dla kazdego konta, bedzie jedno konto admin administrujace zawartoscia
	* magazynu oraz inne konta uzytkownikow, beda dostepne takie opcje
	* jak historia zakupow, doladowanie salda, mozliwosc zakupow
	* dla uzytkownikow oraz organizacja magazynem dla administratora
	*/
	static void LoggingSystem();
	
	/*
	* Funkcja MenuInterface odpowiedzialna za wyswietlenie interfejsu dla danego 
	* uzytkownika. Potrzebna do poruszania sie po programie w wygodny sposob
	*/
	static void MenuInterface(const User& user);
};


#endif