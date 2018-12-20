#ifndef ACTOR_H
#define ACTOR_H

#include <SFML/Graphics.hpp>

namespace TestGame
{
	class Map;
	class Actor
	{
	public:
		virtual void SetLocation(const sf::Vector2f& NewLocation) noexcept;
		virtual void Update(double Elapse, Map* MyMap) = 0;
		const sf::Vector2f	GetLocation() const noexcept;
		sf::Shape*			GetShape() noexcept;
		virtual bool IsBlocking() const noexcept = 0;
	protected:
		sf::RectangleShape Shape;
		sf::Vector2f Location;
	};
}

#endif