#ifndef GAME_H
#define GAME_H

#include "Map.h"

#include <memory>
#include <atomic>
#include <functional>

using namespace std;

namespace TestGame
{

	struct GameSettings
	{
		struct
		{
			unsigned Width, Height;
		} Resolution;
	};

	class Game
	{
	public:
		Game(const GameSettings& Settings);
		~Game();
		void LoadMap(const shared_ptr<Map> NewMap);
		void Run(/*const function<void(double)>& Func, */
			const function<void(double, double, bool)>& FuncEventPress, 
			const function<void(double, double, bool)>& FuncEventRelease, 
			const function<void(shared_ptr<Actor>,bool)>& FuncEventActorClick,
			const function<void(sf::Keyboard::Key Key)>& FuncKeyPressed);
		static inline const GameSettings GetGlobalSettings() { return Settings; }
		static inline const shared_ptr<Map> GetGlobalMap() { return CurrentMap; }
	private:
		static shared_ptr<Map> CurrentMap;
		static GameSettings Settings;
	};
}

#endif

