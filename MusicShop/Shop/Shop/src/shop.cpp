#include "shop.h"


//********************************************************************
//************     Macra dla systemowych komend     ******************
//********************************************************************
#define clearScreen system("cls")
#define pressAnyKey system("pause")




//********************************************************************
//*******  Deklaracja statycznych zmiennych klasy Shop    ************
//********************************************************************
userPermission					Shop::loggedUserRights;
bool							Shop::isProgrammeRunning;
std::vector<AlbumScheme>		Shop::albums;
std::vector<StockAlbum>			Shop::stockAlbums;




//********************************************************************
//*****************       Zmienne globalne      **********************
//********************************************************************

//--------------------------- CONST -----------------------------------				
const std::string				USER_FILE_NAME = "data\\users_data.txt";
const std::string				ALBUM_STOCK_FILE_NAME = "data\\album_stock.txt";
const std::string				ALBUM_LIST_FILE_NAME = "data\\album_list.txt";
const std::string				PURCHASE_FILE_NAME = "data\\purchase_history.txt";

//-------------------------- NON-CONST --------------------------------		
std::unique_ptr<User>			user = std::make_unique<User>();
std::unique_ptr<AlbumScheme>	album = std::make_unique<AlbumScheme>();


//********************************************************************
//******************         Funkcje            **********************
//********************************************************************

void StockAlbum::addAlbumCopies()
{
	clearScreen;
	int x;
	std::cout << "Ilosc kopii do dodania: ";
	std::cin >> x;
	this->m_InStock = x;
	clearScreen;
	std::cout << "Dodano " << x << " kopii!\n";
	pressAnyKey;
}

void StockAlbum::sellAlbum()
{
	if (this->m_InStock > 0)
		this->m_InStock -= 1;
}

void Shop::InitAlbums()
{
	std::ifstream inAlbumListFile;
	inAlbumListFile.open(ALBUM_LIST_FILE_NAME);

	std::string line{};
	std::stringstream ss;
	std::string albumName{}, artist{}, genre{}, prizeS{};
	float prize{};

	while (std::getline(inAlbumListFile, line))
	{
		ss << line;
		ss >> albumName >> artist >> genre >> prizeS;

		prize = std::stof(prizeS);

		album->setName(albumName);
		album->setArtist(artist);
		album->setGenre(genre);
		album->setPrize(prize);

		albums.push_back(*album);
		ss.str(std::string());
		ss.clear();
	}
	inAlbumListFile.close();
}

void Shop::InitUsers()
{
	user->createUser("admin", "admin", userPermission::ADMIN);
	user->createUser("user", "user");
}

void Shop::MenuInterface(const userPermission& user)
{
	clearScreen;
	char input{};
	while (true) 
	{
		if (user == userPermission::ADMIN)
		{
			clearScreen;
			std::cout << "   Zalogowano na koncie admina!\n";
			std::cout << "1. Przejrzyj zawartosc magazynu\n";
			std::cout << "2. Dodaj album do systemu\n";
			std::cout << "3. Dodaj album do magazynu\n";
			std::cout << "4. Przejrzyj historie zamowien\n";
			std::cout << "5. Zakoncz\n";

			std::cin >> input;
			switch (input)
			{
			case '1':
				break;
			case '2':
				AddAlbumToSystem();
				break;
			case '3':
				AddAlbumToStock();
				break;
			case '4':
				break;
			case '5':
				isProgrammeRunning = false;
				return;
				break;
			default:
				break;
			}
		}
		else
		{
			clearScreen;
			std::cout << "   Zalogowano na koncie usera!\n";
			std::cout << "1. Zasil saldo\n";
			std::cout << "2. Przegladaj albumy\n";
			std::cout << "3. Przejrzyj swoja historie zamowien\n";
			std::cout << "4. Zakoncz\n";

			std::cin >> input;
			switch (input)
			{
			case '1':
				break;
			case '2':
				break;
			case '3':
				break;
			case '4':
				isProgrammeRunning = false;
				return;
				break;
			default:
				break;
			}
		}
	}
}

bool Shop::LookForUser(const std::string& username, const std::string& password, userPermission& rights)
{
	std::ifstream inUserFile;
	inUserFile.open(USER_FILE_NAME);
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

bool Shop::LookForAlbum()
{
	for (auto& a : albums)
		if (a.getName() == album->getName())
			if (a.getArtist() == album->getArtist())
				return true;
	return false;
}

void Shop::LoggingSystem()
{
	bool logginInProccess = true;
	bool userFound;
	std::string login, password;
	char symbol{};
	isProgrammeRunning = true;
	while (logginInProccess) {
		clearScreen;
		if (!isProgrammeRunning)
			break;
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
	std::string line;
	std::stringstream ss;
	char ph{};
	std::string name{};
	std::ifstream inUserFile;
	inUserFile.open(USER_FILE_NAME);

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

	outUserFile.open(USER_FILE_NAME, std::ios_base::app);
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

void Shop::AddAlbumScheme(const std::string& albumName, const std::string& artistName, const std::string& genre, const float& prize)
{
	album->setName(albumName);
	album->setArtist(artistName);
	album->setGenre(genre);
	album->setPrize(prize);

	bool doAlbumExist = LookForAlbum();
	if (!doAlbumExist)
	{
		std::ofstream outAlbumListFile;
		outAlbumListFile.open(ALBUM_LIST_FILE_NAME, std::ios_base::app);
		if (outAlbumListFile.is_open())
		{
			std::string temp;
			std::stringstream stream;
			outAlbumListFile << '\n' << albumName << " " << artistName << " " << genre << " ";
			outAlbumListFile << std::fixed << std::setprecision(2) << prize;
			
			std::cout << albumName << " " << artistName << " " << genre << " ";
			std::cout << std::fixed << std::setprecision(2) << prize << '\n';

			albums.push_back(*album);

			std::cout << "Album dodany!\n";
		}
		else
			std::cout << "Blad odczytu pliku!\n";
		outAlbumListFile.close();
	}

	std::cout << "Podany album istnieje! ";
}

void Shop::AddAlbumToSystem()
{
	std::string name{};
	std::string artist{};
	std::string genre{};
	std::string prize{};

	clearScreen;
	std::cout << "/ W miejscu spacji wpisz _ /\n";
	std::cout << "Podaj nazwe albumu: ";
	std::cin  >> name;
	std::cout << "Podaj nazwe artysty: ";
	std::cin  >> artist;
	std::cout << "Podaj nazwe gatunku: ";
	std::cin  >> genre;
	std::cout << "Podaj cene albumu: ";
	std::cin  >> prize;
	
	float temp = std::stof(prize);


	clearScreen;
	AddAlbumScheme(name, artist, genre, temp);
	pressAnyKey;
}

void Shop::AddAlbumToStock()
{
	clearScreen;
	int i = 0;
	char ch{};
	std::cout << "Wybierz album: \n";
	for (auto& a : albums)
		std::cout << i++ << ". " << a.getName() << " " << a.getArtist() << " " << a.getGenre() << "\n";
	//std::cin >> ch;


	pressAnyKey;
}
