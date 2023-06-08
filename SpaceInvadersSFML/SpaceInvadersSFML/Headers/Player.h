#ifndef PLAYER_H
#define PLAYER_H

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <SFML\Network.hpp>
#include <SFML\Audio.hpp>

#include <vector>
#include <algorithm>
#include <chrono>
#include "Bullet.h"

class Player {
public:
	Player();
	//funkcja odpowiadajaca za rysowanie modelu statku gracza
	void Update();
	void Draw(sf::RenderWindow& window);
	void Shoot();
public:
	std::vector<Bullet> bullets;
	double reload;
private:
	void ClearingDeadBullets();
	void Movement();
private:
	bool dead; // zmienna informujaca o tym, czy gracz umarl
	sf::Sprite sprite; //sprite dla modelu
	sf::Texture texture; // tekstura modelu
	unsigned int x; // koordynat x
	unsigned int y; // koordynat y
	unsigned char speed; // predkosc gracza
	std::chrono::time_point<std::chrono::steady_clock> start_reloading, stop_reloading;
	std::chrono::duration<double> time_of_reloading;
};

#endif