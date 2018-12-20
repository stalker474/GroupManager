#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SFML/Graphics.hpp>
#include "Actor.h"

namespace TestGame
{
	class Obstacle : public Actor
	{
	public:
		Obstacle(const float Width,const float Height);
		inline void Update(double Elapsed, Map* MyMap) override {}
		inline bool IsBlocking() const noexcept override { return true; }
	private:
	};
}

#endif