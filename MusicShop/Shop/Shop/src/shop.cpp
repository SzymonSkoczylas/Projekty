#include "shop.h"


//********************************************************************
//************     Macra dla systemowych komend     ******************
//********************************************************************
#define clearScreen system("cls")
#define pressAnyKey system("pause")

//********************************************************************
//*******  Deklaracja statycznych zmiennych klasy Shop    ************
//********************************************************************
userPermission                  Shop::loggedUserRights;
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
std::unique_ptr<User>			currentUser = std::make_unique<User>();
std::unique_ptr<AlbumScheme>	album = std::make_unique<AlbumScheme>();
std::unique_ptr<StockAlbum>		stockAlbum = std::make_unique<StockAlbum>();

//********************************************************************
//******************         Funkcje            **********************
//********************************************************************

void StockAlbum::makeAlbumStock(const AlbumScheme& scheme)
{
	this->setName(scheme.getName());
	this->setArtist(scheme.getArtist());
	this->setGenre(scheme.getGenre());
	this->setPrize(scheme.getPrize());
}

double roundoff(const double& value,const unsigned char& prec)
{
	double pow_10 = pow(10.0f, (double)prec);
	return round(value * pow_10) / pow_10;
}

void StockAlbum::addAlbumCopies()
{
	clearScreen;
	double rounded = round(this->getPrize() * 100);
	rounded /= 100;
	std::string prizeS = std::to_string(rounded);

	int counter{};
	for (;counter < prizeS.length(); counter++)
	{
		if (prizeS[counter] == '.')
		{
			counter += 3;
			break;
		}
	}
	prizeS = prizeS.substr(0,counter);
	counter = 0;

	const std::string deleteline = this->getName() + " " + this->getArtist() + " " + this->getGenre() + " "
		+ prizeS + " " + std::to_string(this->getAmount());
	int x;
	std::cout << "Ilosc kopii do dodania: ";
	std::cin >> x;
	this->m_InStock += x;
	const std::string replaceline = this->getName() + " " + this->getArtist() + " " + this->getGenre() + " "
		+ prizeS + " " + std::to_string(this->getAmount());
	clearScreen;

	std::ifstream stockAlbumFile;
	stockAlbumFile.open(ALBUM_STOCK_FILE_NAME, std::ios_base::in | std::ios_base::app);
	std::ofstream temp;
	temp.open("data\\temp.txt");
	std::string line;
	while (std::getline(stockAlbumFile, line))
	{
		if (line == deleteline)
			temp << replaceline << '\n';
		else
			temp << line << '\n';
	}
	temp.close();
	stockAlbumFile.close();
	remove("data\\album_stock.txt");
	if (rename("data\\temp.txt", "data\\album_stock.txt") == 0)
		std::cout << "Udalo sie dodac! Dodano " << x << " kopii!\n";
	else 
		std::cout << "Nie udalo sie dodac kopii!\n";
}

StockAlbum::StockAlbum()
{
	this->m_NameOfArtist = "";
	this->m_NameOfAlbum = "";
	this->m_Genre = "";
	this->m_Prize = 0.0;
	this->m_InStock = 0;
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
	std::string albumName{}, artist{}, genre{}, prizeS{}, amountS{};
	double prize{};
	int amount{};

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


	std::ifstream inAlbumStockFile;
	inAlbumStockFile.open(ALBUM_STOCK_FILE_NAME);

	while (std::getline(inAlbumStockFile, line))
	{
		ss << line;
		ss >> albumName >> artist >> genre >> prizeS >> amountS;

		prize = std::stof(prizeS);
		amount = std::stoi(amountS);

		stockAlbum->setName(albumName);
		stockAlbum->setArtist(artist);
		stockAlbum->setGenre(genre);
		stockAlbum->setPrize(prize);
		stockAlbum->setAmount(amount);

		stockAlbums.push_back(*stockAlbum);
		ss.str(std::string());
		ss.clear();
	}
	inAlbumStockFile.close();
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
				ViewStockContent();
				pressAnyKey;
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
			std::cout << "   Zalogowano na koncie usera!\t" << 
				"Saldo: " << std::fixed <<std::setprecision(2) << currentUser->getBalance() << " zl\n";
			std::cout << "1. Zasil saldo\n";
			std::cout << "2. Przegladaj albumy\n";
			std::cout << "3. Zakoncz\n";

			std::cin >> input;
			switch (input)
			{
			case '1':
				AddToUserBalance();
				break;
			case '2':
				BuyAlbum();
				break;
			case '3':
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
			if (userFound)
			{
				currentUser->setName(login);
				currentUser->setPassword(password);
				currentUser->setPermission(loggedUserRights);
				currentUser->setBalance(LookForUserBalance(login, password));
				MenuInterface(loggedUserRights);
			}
			else
			{
				clearScreen;
				std::cout << "Nie znaleziono uzytkownika!\n";
				pressAnyKey;
				break;
			}
		case '2':
			clearScreen;
			std::cout << "Wprowadz login: ";
			std::cin >> login;
			std::cout << "Wprowadz haslo: ";
			std::cin >> password;
			clearScreen;
			CreateUser(login, password);
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

void Shop::CreateUser(const std::string& username, const std::string& password, const userPermission& rights)
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
				std::cout << "User o podanym loginie istnieje!\n";
				inUserFile.close();
				pressAnyKey;
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
		outUserFile << "Saldo: 0.00\n";
		outUserFile.close();

		std::cout << "Utworzono uzytkownika!\n";
	}
	else
		std::cout << "Blad odczytu pliku!\n";
	outUserFile.close();
	pressAnyKey;
}

void User::setPermission(bool rights)
{
	if (rights)
		this->m_rights = userPermission::ADMIN;
	else
		this->m_rights = userPermission::USER;
}

void Shop::AddAlbumScheme(const std::string& albumName, const std::string& artistName, const std::string& genre, const double& prize)
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
	
	double temp = std::stof(prize);


	clearScreen;
	AddAlbumScheme(name, artist, genre, temp);
	pressAnyKey;
}

void Shop::AddToUserBalance()
{
	std::ifstream userFile;
	std::ofstream temp;
	userFile.open(USER_FILE_NAME);
	temp.open("data\\temp.txt");

	double addingValue{};

	clearScreen;

	std::cout << "Ile pieniedzy chcesz dodac?\n";
	std::cin >> addingValue;
	

	std::string line, balance;
	std::stringstream ss;
	std::string name{}, ph{};
	double tempBalance{};
	bool userFound = false;
	while (std::getline(userFile,line))
	{
		ss.clear();
		ss.str(std::string());
		ss << line;
		ss >> ph >> name;

		if(name == currentUser->getName())
			userFound = true;
		if ( (ph == "Saldo:") && (userFound) )
		{
			tempBalance = std::stod(name);
			tempBalance += addingValue;
			temp << ph << " " << std::fixed << std::setprecision(2) << tempBalance << '\n';
			userFound = false;
		}
		else
			temp << line <<'\n';
	}
	userFile.close();
	temp.close();
	remove("data\\users_data.txt");
	rename("data\\temp.txt", "data\\users_data.txt");

	clearScreen;
	std::cout << "Dodano: " << addingValue <<" zl do konta\n";
	std::cout << "Stan konta: " << std::fixed << std::setprecision(2) << tempBalance << " zl";
	pressAnyKey;
}

void Shop::ViewStockContent()
{
	clearScreen;
	int i{};
	for (auto& sa : stockAlbums)
	{
		std::cout << i++ << ". " << sa.getName() << " " << sa.getArtist() << " " 
			<< sa.getGenre() << " Cena:" << sa.getPrize() << " Na stanie:" << sa.getAmount() << '\n';
	}
}

bool Shop::LookForAlbumInStock(const AlbumScheme& scheme)
{
	for (auto& sa : stockAlbums)
	{
		if (sa.getName() == scheme.getName())
			if (sa.getArtist() == scheme.getArtist())
				return true;
	}
	return false;
}

void Shop::FillStockFile(const StockAlbum& scheme)
{
	std::ofstream outAlbumStockFile;
	outAlbumStockFile.open(ALBUM_STOCK_FILE_NAME, std::ios_base::app);
	if (outAlbumStockFile.is_open())
	{
		outAlbumStockFile << scheme.getName() << " " << scheme.getArtist() << " " << scheme.getGenre() << " ";
		outAlbumStockFile << std::fixed << std::setprecision(2) << scheme.getPrize() << " ";
		outAlbumStockFile << scheme.getAmount() << '\n';
	}
	else
		std::cout << "Blad odczytu pliku!\n";
	outAlbumStockFile.close();
}

void Shop::AddAlbumToStock()
{
	clearScreen;
	unsigned int i{};
	unsigned int ch{};
	std::cout << "Wybierz album: \n";
	for (auto& a : albums)
		std::cout << i++ << ". " << a.getName() << " " << a.getArtist() << " " << a.getGenre() << "\n";
	std::cin >> ch;
	while(ch >= i)
	{
		clearScreen;
		i = 0;
		std::cout << "Wybierz album: \n";
		for (auto& a : albums)
			std::cout << i++ << ". " << a.getName() << " " << a.getArtist() << " " << a.getGenre() << "\n";
		std::cout << "\nWybierz numer z zakresu 0-" << (--i) << "!\n";
		std::cin >> ch;
	}
	StockAlbum stAlbum;
	bool doStockAlbumExist = LookForAlbumInStock(albums[ch]);
	stAlbum.makeAlbumStock(albums[ch]);
	if (!doStockAlbumExist) 
	{
		FillStockFile(stAlbum);
		stAlbum.addAlbumCopies();
		stockAlbums.push_back(stAlbum);
	}
	else
	{
		for (auto& sa : stockAlbums)
			if (sa.getName() == stAlbum.getName())
				if (sa.getArtist() == stAlbum.getArtist())
					sa.addAlbumCopies();
	}
	pressAnyKey;
}

double Shop::LookForUserBalance(const std::string& username, const std::string& password)
{
	std::ifstream userFile;
	userFile.open(USER_FILE_NAME);
	std::stringstream ss;
	std::string ph{}, name{}, line{};
	bool userFound = false;

	while (std::getline(userFile, line))
	{
		ss.clear();
		ss.str(std::string());
		ss << line;
		ss >> ph >> name;
		if (name == username)
			userFound = true;
		if ((userFound) && (ph == "Saldo:"))
		{
			userFile.close();
			return std::stod(name);
		}
	}
}

void Shop::UpdateUserBalance(const double& balance)
{
	std::ifstream userFile;
	std::ofstream temp;
	userFile.open(USER_FILE_NAME);
	temp.open("data\\temp.txt");

	std::string line;
	std::stringstream ss;
	std::string name{}, ph{};
	bool userFound = false;
	while (std::getline(userFile, line))
	{
		ss.clear();
		ss.str(std::string());
		ss << line;
		ss >> ph >> name;

		if (name == currentUser->getName())
			userFound = true;
		if ((ph == "Saldo:") && (userFound))
		{;
			temp << ph << " " << std::fixed << std::setprecision(2) << balance << '\n';
			userFound = false;
		}
		else
			temp << line << '\n';
	}
	userFile.close();
	temp.close();
	remove("data\\users_data.txt");
	rename("data\\temp.txt", "data\\users_data.txt");
}

void Shop::BuyAlbum()
{
	size_t stockSize = stockAlbums.size();
	size_t temp;
	while (true)
	{
		clearScreen;
		ViewStockContent();
		std::cout << "Wybierz album: ";
		std::cin >> temp;
		if (temp < stockSize)
		{
			if (currentUser->getBalance() >= stockAlbums[temp].getPrize())
			{
				currentUser->setBalance(currentUser->getBalance() - stockAlbums[temp].getPrize());
				UpdateUserBalance(currentUser->getBalance());
				UpdatePurchaseHistory(stockAlbums[temp]);
			}
			else
			{
				clearScreen;
				std::cout << "Nie masz wystarczajacej ilosci pieniedzy!\n";
				pressAnyKey;
				return;
			}
		}
		else
			std::cout << "Wybierz album z zakresu\n";
	}
}

void Shop::UpdatePurchaseHistory(const StockAlbum& album)
{
	std::ofstream purchaseFile;
	purchaseFile.open(PURCHASE_FILE_NAME, std::ios_base::app);
	if (purchaseFile.good())
	{
		
	}
	purchaseFile.close();
}
