#ifndef SQUAD_H
#define SQUAD_H

#include "Group.h"
#include "Unit.h"
#include "Actor.h"
#include "Map.h"
#include "FormationStateMachine.h"

namespace TestGame
{
	class Squad
	{
	public:
		Squad(size_t Count, ETeam Team);
		void Spawn(shared_ptr<Map> MyMap);
		void Unspawn(shared_ptr<Map> MyMap);
		void AddUnits(size_t Count);
		void UpdatePosition(sf::Vector2f Location, sf::Vector2f Orientation, bool Teleport = false);
		void Goto(const sf::Vector2f Location, const sf::Vector2f Orientation);
		void Attack(shared_ptr<Squad> Target);

		inline sf::Vector2f GetLocation() const { return Position; }
		inline shared_ptr<GroupManager::Group> GetGroup() const { return Group; }
		ETeam GetTeam() const;
	private:
		shared_ptr<GroupManager::Group> Group;
		vector<shared_ptr<Unit>> Units;
		sf::Vector2f Position;
	};
}

#endif
