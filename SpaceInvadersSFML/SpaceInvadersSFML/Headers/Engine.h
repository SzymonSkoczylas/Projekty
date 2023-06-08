#ifndef ENGINE_H
#define ENGINE_H

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <SFML\Network.hpp>
#include <SFML\Audio.hpp>

#include <chrono>

#include "Player.h"
#include "Global.h"
#include "Interface.h"
#include "UnitFunctions.h"
#include "Enemy.h"

namespace Engine {
	static class Game {
	public:
		static void CreateWindow();
		static void AppPolling();
	private:
		static inline sf::Event event;
		static inline sf::RenderWindow window;
		static inline std::vector<Enemy> enemies;
		static inline Player player;
		static inline sf::Sprite background_sprite;
		static inline sf::Texture background_texture;
	private:
		static void Render();
		static void Update();
		static void LoadBackground();
	};
}
#endif