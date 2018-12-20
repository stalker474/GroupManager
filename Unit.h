#ifndef UNIT_H
#define UNIT_H

#include <SFML/Graphics.hpp>

#include "GroupUnitInterface.h"
#include "Actor.h"
#include <vector>
#include <memory>

using namespace std;

namespace TestGame
{
	enum class ETeam
	{
		Blue,
		Red
	};
	class Map;

	class Unit : public GroupManager::IGroupUnit, public Actor
	{
	public:
		Unit(const size_t Size, const ETeam MyTeam = ETeam::Blue);
		void SetLocation(const sf::Vector2f& NewLocation) noexcept override;
		const GroupManager::ETeam GetTeamStatus() const noexcept override;
		void Update(double Elapsed, Map* MyMap) override;
		void BuildPath(Map* CurrentMap);

		inline bool IsBlocking() const noexcept override { 
			return false; 
		}
		inline sf::Vector2f GetObjectiveLocation() const noexcept {
			return ObjectiveLocation;
		}
	private:
		ETeam Team;
		double MaxSpeed;
		sf::Vector2f TempObjectiveLocation;
		sf::Vector2f ObjectiveLocation;

		// Hérité via IGroupUnit
		void OnSlotAssigned(const GroupManager::FormationSlot * Slot) override;
		void OnSlotDetached(const GroupManager::FormationSlot * Slot) override;
		void OnSlotLocationChanged(const GroupManager::FormationSlot* Slot) override;
	};
}

#endif
