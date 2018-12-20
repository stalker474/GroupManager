#include "Actor.h"

using namespace TestGame;

void Actor::SetLocation(const sf::Vector2f & NewLocation) noexcept
{
	Location = NewLocation;
}

const sf::Vector2f Actor::GetLocation() const noexcept
{
	return Location;
}

sf::Shape* Actor::GetShape() noexcept
{
	return &Shape;
}
