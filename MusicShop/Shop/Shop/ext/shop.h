#ifndef SHOP_H
#define SHOP_H

//Includes
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <memory>



class AlbumScheme{	//Klasa przechowujaca informacje o albumie
protected:
	std::string m_NameOfAlbum{};		//nazwa albumu
	std::string m_NameOfArtist{};		//nazwa artysty/zespolu
	std::string m_Genre{};				//Gatunek
	float m_Prize{};					//Cena
public:

	// Funkcje dostepu
	const std::string getName()   const	{ return m_NameOfAlbum; }
	const std::string getArtist() const { return m_NameOfArtist; }
	const std::string getGenre()  const { return m_Genre; }
	const float getPrize()		  const	{ return m_Prize; }

	// Funkcje setujace
	void setName(const std::string& name)	  { m_NameOfAlbum = name; }
	void setArtist(const std::string& artist) { m_NameOfArtist = artist; }
	void setGenre(const std::string& genre)   { m_Genre = genre; }
	void setPrize(const float& prize)		  { m_Prize = prize; }
};

class StockAlbum : public AlbumScheme {		//Klasa przechowujaca informacje o albumie dostepnym w magazynie
private:
	int m_InStock{};						//Ilosc na magazynie
public:
	//Konstruktory
	StockAlbum();

	// Funkcje dostepu
	const int getAmount() const { return m_InStock; }

	// Funkcje setujace
	void setAmount(const int& amount) { m_InStock = amount; }

	/*
	* Funkcja makeAlbumStock ktora na podstawie danych
	* z podanego obiektu AlbumScheme tworzy obiekt klasy StockAlbum.
	*/
	void makeAlbumStock(const AlbumScheme& scheme);


	/*
	* Funkcja addAlbumCopies ktora dodaje kopie albumu do magazynu
	* funkcja sprawdza czy dany album jest juz na magazynie oraz
	* modyfikuje zawartosc pliku przechowujacego dane o albumach
	* znajdujacych sie na magazynie
	*/
	void addAlbumCopies();

	/*
	* Funkcja sellAlbum usuwa album z magazynu i dodaje go
	* do listy zakupow uzytkownika, dbajac o takie rzeczy, jak
	* usuniecie pieniedzy z salda uzytkownika oraz aktualizacje
	* liczby albumow dostepnych na stanie i historii sprzedazy
	*/
	void sellAlbum();
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
	void createUser(const std::string& username, const std::string& password, const userPermission& rights=userPermission::USER);

	//Funkcja getName zwraca nazwe uzytkownika
	const std::string getName() const { return this->m_Name; }

	//Funkcja getPassword zwraca haslo uzytkownika
	const std::string getPassword() const { return this->m_Password; }

	//Funkcja getPermission zwraca uprawnienia uzytkownika
	const userPermission getPermission() const { return this->m_rights; }

	//Funkcja setPermisson zmienia uprawnienia uzytkownikowi
	void setPermission(bool rights);
};

class Shop {									//Klasa dzia³aj¹ca jak "silnik" programu
private:
	static userPermission loggedUserRights;		//Sprawdzanie jakie sa uprawnienia usera, ktory aktualnie przeglada sklep
	static bool isProgrammeRunning;				//Bool sprawdzajacy czy program jest
	static std::vector<AlbumScheme> albums;		//Lista albumow znajdujacych sie w systemie
	static std::vector<StockAlbum> stockAlbums; //Lista albumow znajdujacych sie na magazynie
public:
	Shop() = delete;

	// Funkcje dostepowe
	static userPermission getCurrentUserPermission() { return loggedUserRights; }

	/*
	* Funkcja InitAlbums inicjalizujaca albumy do programu.
	* Wstepnie czyta wpisane "z palca" przez programiste wartosci
	* w przyszlosci beda one zapisywane i wczytywane z odpowiedniego pliku
	*/
	static void InitAlbums();


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
	static void MenuInterface(const userPermission& user);

	// Funkcja LookForUser sprawdzajaca czy w spisie uzykownikow istnieje uzytkownik o podanym loginie 
	static bool LookForUser(const std::string& username, const std::string& password, userPermission& rights);

	// Funkcja LookForAlbum sprawdzajaca czy w vectorze albumow istnieje podany album
	static bool LookForAlbum();
	// Funkcja AddAlbumScheme dodaje schemat albumu do programu
	static void AddAlbumScheme(const std::string& albumName, const std::string& artistName, const std::string& genre, const float& prize);

	//Funckja AddAlbumToSystem wyswietla interfejs dodawania albumu do systemu
	static void AddAlbumToSystem();

	//Funkcja AddAlbumToStock wyswietla interfejs dodawania albumu do magazynu
	static void AddAlbumToStock();

	//Funkcja LookForAlbumInStock sprawdzajaca czy podany album jest na magazynie
	static bool LookForAlbumInStock(const AlbumScheme& scheme);

	//Funkcja FillStockFile uzupelniajaca plik przechowujacy dane o albumach znajdujacych sie na magazynie
	static void FillStockFile(const StockAlbum& scheme);

	//Funkcja ViewStockContent wyswietlajaca w konsoli albumy znajdujace sie na magazynie
	static void ViewStockContent();
};


#endif