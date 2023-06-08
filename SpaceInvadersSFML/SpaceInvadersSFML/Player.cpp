#include "Headers\Player.h"
#include "Headers\Global.h"

void Player::Shoot()
{
	if (this->reload == 0.0)
	{
		this->reload = PLAYER_RELOAD_SPEED;
		this->start_reloading = std::chrono::high_resolution_clock::now();
		this->bullets.push_back(Bullet(this->x, this->y));
	}
	else
	{
		stop_reloading = std::chrono::high_resolution_clock::now();
		this->time_of_reloading = stop_reloading - start_reloading;
		if (time_of_reloading.count()  > this->reload)
			this->reload = 0.0;
	}

}

void Player::ClearingDeadBullets()
{
	this->bullets.erase(remove_if(bullets.begin(), bullets.end(), [](const Bullet& bullet) { return bullet.dead; }), bullets.end());
}

void Player::Movement()
{
	char direction_x = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) -
		sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	this->x = std::clamp<int>(x + speed * direction_x, 0, SCREEN_WIDTH * SCREEN_RESIZE - 2 * BASE_SIZE);
}

void Player::Draw(sf::RenderWindow& window)
{
	this->sprite.setPosition(x, y);
	window.draw(sprite);
}

void Player::Update()
{
	this->stop_reloading = std::chrono::high_resolution_clock::now();

	Movement();
	ClearingDeadBullets();
}

Player::Player() :
	x(0.5f * (SCREEN_WIDTH * SCREEN_RESIZE - BASE_SIZE)),
	y(SCREEN_HEIGHT * SCREEN_RESIZE - 2.5f * BASE_SIZE),
	dead(false),
	speed(PLAYER_BASE_SPEED),
	reload(0.0)
{
	this->texture.loadFromFile("Images/statek1.png");
	this->sprite.setTexture(texture);
	this->sprite.setScale(2.f, 2.f);
}