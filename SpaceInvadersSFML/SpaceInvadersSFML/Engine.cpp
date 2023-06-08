#include "Headers\Engine.h"

using namespace sf;

void Engine::Game::CreateWindow()
{
	window.create(VideoMode(SCREEN_WIDTH * SCREEN_RESIZE, SCREEN_HEIGHT * SCREEN_RESIZE), "Space Invaders", Style::Titlebar | Style::Close);
	window.setFramerateLimit(60);

}


void Engine::Game::Render()
{
	LoadBackground();

	window.clear();
	window.draw(background_sprite); // zaladowanie tla

	player.Draw(window);

	for (auto& bullet : player.bullets)
		bullet.Draw(window);

	window.display();
}

void Engine::Game::Update()
{
	player.Update();

	for (auto& bullet : player.bullets)
		bullet.Update();
}

void Engine::Game::LoadBackground()
{
	background_texture.loadFromFile("Images/space.png");
	background_sprite.setTexture(background_texture);
	background_sprite.setScale(2.5f, 2.f);
}

void Engine::Game::AppPolling()
{
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			player.Shoot();

		Update();

		Render();
	}
}