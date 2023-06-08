#include "Headers\Enemy.h"
#include "Headers\Global.h"

void Enemy::Update()
{

}

void Enemy::Draw(sf::RenderWindow& window)
{
}

void Enemy::Shoot()
{

}

void Enemy::ClearingDeadBullets()
{
	this->bullets.erase(remove_if(bullets.begin(), bullets.end(), [](const Bullet& bullet) { return bullet.dead; }), bullets.end());
}

void Enemy::Movement()
{
}


Enemy::Enemy(unsigned char X, unsigned char Y, const std::string& model) :
	x(X), y(Y), dead(false), reload(0.0), speed(ENEMY_BASE_SPEED)
{
	this->texture.loadFromFile(model);
	this->sprite.setTexture(texture);
	this->sprite.setScale(2.f, 2.f);
}