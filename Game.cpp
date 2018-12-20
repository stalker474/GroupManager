#include "Game.h"
#include "Exceptions.h"

#include <thread>
#include <chrono>
#include <condition_variable>

#include <SFML/Graphics.hpp>

using namespace TestGame;

Game::Game(const GameSettings& NewSettings)
{
	Settings = NewSettings;
}

Game::~Game()
{
}

void Game::LoadMap(const shared_ptr<Map> NewMap)
{
	CurrentMap = NewMap;
}

void Game::Run(
	/*const function<void (double)>& Func, */
	const function<void(double, double, bool)>& FuncEventPress, 
	const function<void(double, double, bool)>& FuncEventRelease, 
	const function<void(shared_ptr<Actor>,bool)>& FuncEventActorClick,
	const function<void(sf::Keyboard::Key Key)>& FuncKeyPressed)
{
	if (!CurrentMap)
		throw GameException("Map required");

	sf::RenderWindow window(sf::VideoMode(Settings.Resolution.Width, Settings.Resolution.Height), CurrentMap->GetName());

	using namespace std::chrono;
	auto time = high_resolution_clock::now();
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				auto actor = CurrentMap->GetActorOnLocation({ (float)event.mouseButton.x, (float)event.mouseButton.y });
				if (actor != nullptr)
				{
					FuncEventActorClick(actor, event.mouseButton.button == sf::Mouse::Left);
					continue;
				}	
				double afineCoordX = event.mouseButton.x / static_cast<double>(window.getSize().x);
				double afineCoordY = event.mouseButton.y / static_cast<double>(window.getSize().y);
				FuncEventPress(afineCoordX * CurrentMap->GetBounds().Width, afineCoordY * CurrentMap->GetBounds().Height, event.mouseButton.button == sf::Mouse::Left);
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				double afineCoordX = event.mouseButton.x / static_cast<double>(window.getSize().x);
				double afineCoordY = event.mouseButton.y / static_cast<double>(window.getSize().y);
				FuncEventRelease(afineCoordX * CurrentMap->GetBounds().Width, afineCoordY * CurrentMap->GetBounds().Height, event.mouseButton.button == sf::Mouse::Left);
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				FuncKeyPressed(event.key.code);
			}
		}

		auto currentTime = high_resolution_clock::now();
		auto dT = duration_cast<duration<double>>(currentTime - time);
		time = currentTime;

		window.clear();

		//Func(dT.count());
		CurrentMap->Update(dT.count());
			
		CurrentMap->Draw(window);
			
		window.display();
	}
}

shared_ptr<Map> Game::CurrentMap = nullptr;
GameSettings Game::Settings = {};