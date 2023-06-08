#include "Headers\Bullet.h"
#include "Headers\Global.h"

void Bullet::Draw(sf::RenderWindow& window)
{
	texture.loadFromFile("Images/bulletP.png");
	sprite.setTexture(texture);
	sprite.setScale(0.6f, 0.6f);
	sprite.setPosition(x, y);
	window.draw(sprite);
}

void Bullet::Update()
{
	if (y < (0 - BASE_SIZE))
		y -= speed;
	else
		dead = true;
}

Bullet::Bullet(unsigned int xx, unsigned int yy) :
	x(xx - BASE_SIZE/10),
	y(yy - BASE_SIZE),
	speed(PLAYER_BULLET_BASE_SPEED),
	dead(false)
{

}
