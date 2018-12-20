#include "Map.h"
#include <SFML/Graphics.hpp>
#include "Unit.h"
#include <array>

template<typename T>
constexpr float tofloat(const T value) noexcept
{
	return static_cast<float>(value);
}

using namespace TestGame;

shared_ptr<MapNode> GetNode(uint32_t x, uint32_t y,vector<shared_ptr<MapNode>>& Nodes, const Map::MapBounds& Bounds)
{
	return Nodes[x*Bounds.Height + y];
}

sf::Vector2f Map::FindPath(sf::Vector2f Start, sf::Vector2f End)
{
	uint32_t X, Y, W, Z;
	X = (uint32_t)Start.x;
	Y = (uint32_t)Start.y;
	W = (uint32_t)End.x;
	Z = (uint32_t)End.y;

	if (X >= Bounds.Width || Y >= Bounds.Height)
		return Start;
	if (W >= Bounds.Width || Z >= Bounds.Height)
		return Start;

	auto startNode = GetNode(X, Y, Nodes, Bounds);
	auto endNode = GetNode(W, Z, Nodes, Bounds);

	PathFinder<MapNode> pathfinder;
	pathfinder.setStart(*startNode.get());
	pathfinder.setGoal(*endNode.get());

	std::vector<MapNode*> solution;
	pathfinder.findPath<AStar>(solution);
	
	if (solution.size() <= 1)
		return Start;
	else
		return { (float)solution[1]->getX(), (float)solution[1]->getY() };
}

Map::Map(const string& Name, const size_t Width, const size_t Height) : Name(Name)
{
	Bounds.Width = Width;
	Bounds.Height = Height;

	Nodes.reserve(Bounds.Width*Bounds.Height);

	for (int32_t i = 0; i < Bounds.Width; ++i)
	{
		for (int32_t j = 0; j < Bounds.Height; ++j)
		{
			Nodes.push_back(make_shared<MapNode>(Vector2i{ i,j }));
		}
	}
	int32_t newX, newY;
	for (int32_t x = 0; x < Bounds.Width; ++x)
	{
		for (int32_t y = 0; y < Bounds.Height; ++y)
		{
			for (int ci = -1; ci < 2; ++ci)
			{
				newX = GetNode(x,y,Nodes,Bounds)->getX() + ci;
				for (int cj = -1; cj < 2; ++cj) // for all squares in this 3*3 square
				{
					newY = GetNode(x, y, Nodes, Bounds)->getY() + cj;
					if (newX > -1 && newX < Bounds.Width && newY > -1 && newY < Bounds.Height) // be sure not to go outside the limits
					{
						auto aChild = GetNode(newX, newY, Nodes, Bounds);
						if ((newX != x || newY != y)) // only take free squares and not the one we are examining
							GetNode(x, y, Nodes, Bounds)->addChild(aChild.get(), GetNode(x, y, Nodes, Bounds)->localDistanceTo(aChild.get()));
					}
				}
			}
			
		}
	}
}

void Map::AddActor(shared_ptr<Actor> Actor) noexcept
{
	Actors.push_back(Actor);
}

void Map::RemoveActor(shared_ptr<Actor> Actor) noexcept
{
	Actors.remove(Actor);
}

void Map::Update(double Elapsed)
{
	vector<shared_ptr<MapNode>> obstacles;

	for (auto& actor : Actors)
		if (actor->IsBlocking())
		{
			sf::Vector2f pos = actor->GetLocation();
			obstacles.push_back(GetNode((uint32_t)pos.x, (uint32_t)pos.y,Nodes,Bounds));
		}

	bool changed = false;
	for (auto& node : Nodes)
	{
		auto res = std::find(obstacles.begin(), obstacles.end(), node);
		if (res != obstacles.end())
		{
			if (!node->GetIsObstacle())
			{
				changed = true;
				node->SetIsObstacle(true);
			}
				
		}
		else
			if (node->GetIsObstacle())
			{
				changed = true;
				node->SetIsObstacle(false);
			}
	}

	for (auto& actor : Actors)
		actor->Update(Elapsed,this);
}

void Map::Draw(sf::RenderWindow & Context) noexcept
{	
	size_t nbLinesVert, nbLinesHor;
	nbLinesVert = Context.getSize().x / Bounds.Width;
	nbLinesHor = Context.getSize().y / Bounds.Height;
	//build grid
	vector<sf::Vertex> grid;
	grid.reserve(nbLinesVert*nbLinesHor);
	sf::Color color(50, 50, 50, 255);
	for (size_t i = 0; i < Context.getSize().x; i+= nbLinesVert)
	{
		float coord = tofloat(i);
		grid.push_back({ sf::Vector2f(coord, 0), color });
		grid.push_back({ sf::Vector2f(coord, tofloat(Context.getSize().y)), color });
	}

	for (size_t i = 0; i < Context.getSize().y; i+= nbLinesHor)
	{
		float coord = tofloat(i);
		grid.push_back({ sf::Vector2f(0, coord), color });
		grid.push_back({ sf::Vector2f(tofloat(Context.getSize().x), coord), color });
	}

	Context.draw(grid.data(), grid.size(), sf::Lines);

	float sizeX, sizeY;
	sizeX = Context.getSize().x / tofloat(Bounds.Width);
	sizeY = Context.getSize().y / tofloat(Bounds.Height);

	for (const auto& actor : Actors)
	{
		auto shape = actor->GetShape();
		float posX = actor->GetLocation().x * sizeX;
		float posY = actor->GetLocation().y * sizeY;
		shape->setPosition({posX,posY});
		Context.draw(*shape);
	}
}

shared_ptr<Actor> Map::GetActorOnLocation(const sf::Vector2f Location) const
{
	for (const auto& actor : Actors)
	{
		if (actor->GetShape()->getGlobalBounds().contains(Location))
			return actor;
	}
	return nullptr;
}

MapNode::MapNode(Vector2i Coords)
{
	setPosition(Coords.x, Coords.y);
	IsObstacle = false;
}

float MapNode::distanceTo(AStarNode * node) const
{
	auto mapnode = dynamic_cast<MapNode*>(node);
	if (mapnode->GetIsObstacle())
		return numeric_limits<float>::max();
	int newX = m_x - node->getX(), newY = m_y - node->getY();
	return sqrtf(static_cast<float>(newX*newX + newY*newY));
}
