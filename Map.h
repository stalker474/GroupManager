#ifndef MAP_H
#define MAP_H

#include <string>
#include <memory>
#include "Actor.h"
#include "PathFinder.h"
#include "AStar.h"
#include "Math.h"
#include <list>

using namespace std;

namespace TestGame
{
	class MapNode : public AStarNode
	{
	public:
		MapNode(Vector2i Coords);
		// Hérité via AStarNode
		virtual float distanceTo(AStarNode * node) const override;
		// A diagonal move has a sqrt(2) cost, 1 otherwise
		float localDistanceTo(AStarNode* node) const
		{
			if (node->getX() != m_x && node->getY() != m_y)
				return 1.41421356237f;
			else
				return 1.0f;
		}
		inline void SetIsObstacle(bool Obstacle) noexcept { IsObstacle = Obstacle; }
		inline bool GetIsObstacle() const noexcept { return IsObstacle; }
	private:
		bool IsObstacle;
	};
		
	class Map
	{
	public:
		struct MapBounds
		{
			size_t Width, Height;
		};
		sf::Vector2f FindPath(sf::Vector2f Start, sf::Vector2f End);
		Map(const string& Name, const size_t Width, const size_t Height);
		void AddActor(shared_ptr<Actor> Actor) noexcept;
		void RemoveActor(shared_ptr<Actor> Actor) noexcept;
		void Update(double Elapsed);
		void Draw(sf::RenderWindow& Context) noexcept;
		shared_ptr<Actor> GetActorOnLocation(const sf::Vector2f Location) const;

		inline const string GetName() const noexcept { return Name; }
		inline const MapBounds GetBounds() const noexcept { return Bounds; }
	private:
		string Name;
		list<shared_ptr<Actor>> Actors;
		MapBounds Bounds;
		vector<shared_ptr<MapNode>> Nodes;
	};
}
#endif

