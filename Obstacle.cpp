#include "Obstacle.h"

using namespace TestGame;

Obstacle::Obstacle(const float Width, const float Height)
{
	Shape = sf::RectangleShape({ Width, Height });
	Shape.setFillColor(sf::Color::White);
}