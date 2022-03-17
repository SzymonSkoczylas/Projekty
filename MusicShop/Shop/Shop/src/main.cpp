#include "shop.h"

int main()
{
	std::vector<User> users;

	Shop::InitAlbums();
	Shop::InitUsers(users);
	Shop::InitLoggingSystem(users);

}