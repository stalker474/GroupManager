#include "Unit.h"
#include "FormationSlot.h"
#include "Map.h"
#include <assert.h>

using namespace TestGame;

Unit::Unit(const size_t Size, const ETeam MyTeam) : Team(MyTeam)
{
	Shape = sf::RectangleShape({ (float)Size,(float)Size });
	Shape.setFillColor(MyTeam == ETeam::Blue?sf::Color::Blue:sf::Color::Red);
	Shape.setOutlineThickness(1.0f);
	MaxSpeed = 18.0;
}

void Unit::SetLocation(const sf::Vector2f& NewLocation) noexcept
{
	Actor::SetLocation(NewLocation);
	ObjectiveLocation = NewLocation;
	TempObjectiveLocation = ObjectiveLocation;
}

bool sameNode(const sf::Vector2f& A, const sf::Vector2f& B)
{
	uint32_t X = (uint32_t)A.x;
	uint32_t Y = (uint32_t)A.x;
	uint32_t bX = (uint32_t)B.x;
	uint32_t bY = (uint32_t)B.y;

	return (X==bX) && (Y == bY);
}

void Unit::Update(double Elapsed, Map* MyMap)
{
	if (Elapsed < 0)
	{
		//instant move
		SetLocation(ObjectiveLocation);
	}
		
	auto dir = ObjectiveLocation - Location;
	Vector2d normdir = { dir.x,dir.y };
	Shape.setOutlineColor(sf::Color::Green);
	//our last goal and current position are the same
	//nothing to do
	if (normdir.length() == 0)
		return;

	dir = TempObjectiveLocation - Location;
	normdir = { dir.x,dir.y };
	//our temporary goal and location are the same, build next part of path
	if(normdir.length() == 0)
		BuildPath(MyMap);
	else
	{
		Shape.setOutlineColor(sf::Color::Yellow);
		auto len = normdir.length();
		//we havent reached the temporary goal, compute the movement
		//towards it
		normdir.normalize();
		auto displacement = normdir*MaxSpeed*Elapsed;
		if (displacement.length() > len) //too much time elapsed so we jumped too far
			displacement = displacement.normalized()*len;

		Vector2d location = { Location.x,Location.y };
		location = location + displacement;
		Location = { (float)location.x,(float)location.y };
	}
}

void Unit::BuildPath(Map* CurrentMap)
{
	assert(CurrentMap != nullptr);
	TempObjectiveLocation = CurrentMap->FindPath(Location, ObjectiveLocation);
}

void Unit::OnSlotAssigned(const GroupManager::FormationSlot * Slot)
{
}

void Unit::OnSlotDetached(const GroupManager::FormationSlot * Slot)
{
}

void Unit::OnSlotLocationChanged(const GroupManager::FormationSlot * Slot)
{
	ObjectiveLocation = {
		static_cast<float>(Slot->GetPosition().x), 
		static_cast<float>(Slot->GetPosition().y) 
	};
}

const GroupManager::ETeam Unit::GetTeamStatus() const noexcept
{
	return Team == ETeam::Blue ? GroupManager::ETeam::ALLY : GroupManager::ETeam::ENEMY;
}