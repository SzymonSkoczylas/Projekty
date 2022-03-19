#ifndef SHOP_H
#define SHOP_H

//Includes
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>


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

	//Funkcja addToStock dodaje album do magazynu
	void addToStock(const int& amount);

	/*
	* Funkcja sellAlbum usuwa album z magazynui dodaje go
	* do listy zakupow uzytkownika, dbajac o takie rzeczy, jak
	* usuniecie pieniedzy z salda uzytkownika oraz aktualizacje
	* liczby albumow dostepnych na stanie i historii sprzedazy
	*/
	void sellAlbum();
};


class SoldAlbum : public AlbumScheme {			//Klasa przechowujaca informacje o sprzedanym albumie
private:
	std::string m_ownerName;					//Nazwa uzytkownika ktory kupil album
	std::string m_dateOfPurchase;				//Data sprzedazy
public:

};


enum class userPermission { USER = 0, ADMIN = 1 };

class User {											//Klasa przechowujaca informacje o uzytkowniku
private:
	std::string m_Name;									//Login i nazwa
	std::string m_Password;								//Haslo
	std::vector<AlbumScheme> m_ownedAlbums;				//Kupione albumy
	userPermission m_rights = userPermission::USER;		//Uprawnienia uzytkownika
public:


	//Funkcja createUser ustawia login i haslo uzytkownikowi
	void createUser(const std::string& username, const std::string& password);

	//Funkcja getName zwraca nazwe uzytkownika
	const std::string getName() const { return this->m_Name; }

	//Funkcja getPassword zwraca haslo uzytkownika
	const std::string getPassword() const { return this->m_Password; }

	//Funkcja getPermission zwraca uprawnienia uzytkownika
	const userPermission getPermission() const { return this->m_rights; }

	//Funkcja setPermisson zmienia uprawnienia uzytkownikowi
	void setPermission(bool rights);
};

class Shop {							//Klasa dzia³aj¹ca jak "silnik" programu
private:
	static std::ifstream inUserFile;	//Kontrola wejscia z pliku uzykownikow
	static std::ofstream outUserFile;	//Kontrola wyjscia z pliku uzytkownikow
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

	/*
	* Funkcja RegisterUser odpowiadajaca za tworzenie uzytkownika
	* i dodanie go do "systemu". Za pomoca stworzonego uzytkownika
	* bedzie mozna sie zalogowac do sklepu jako konsument
	*/
	static void RegisterUser(const std::string& login, const std::string& pass);
};


#endif