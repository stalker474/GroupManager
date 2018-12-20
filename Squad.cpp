#include "Squad.h"
#include "Game.h"
#include "Exceptions.h"
#include "GroupManagerExceptions.h"

using namespace TestGame;

Squad::Squad(size_t Count, ETeam Team)
{
	Group = make_shared<GroupManager::Group>(Team == ETeam::Blue ? GroupManager::ETeam::ALLY : GroupManager::ETeam::ENEMY);
	auto formation = make_shared<GroupManager::Formation>(GroupManager::EFormationType::Slotted, Group);

	GroupManager::FormationStateMachineBuilder fsmbuilder;

	fsmbuilder.AddFormations({ formation });

	Group->AssignFormationStateMachine(fsmbuilder.Build());

	AddUnits(Count);
}

void Squad::Spawn(shared_ptr<Map> MyMap)
{
	for(const auto& unit : Units)
		MyMap->AddActor(unit);

	MyMap->Update(-1);
}

void Squad::Unspawn(shared_ptr<Map> MyMap)
{
	for (const auto& unit : Units)
		MyMap->RemoveActor(unit);
}

void Squad::AddUnits(size_t Count)
{
	auto formation = Group->GetFSM()->GetCurrentFormation();
	//by groups  of 5?
	double angle = 0;
	double x, y;
	int distance = 2;
	size_t prevCount = Units.size();
	for (size_t i = prevCount; i < prevCount+Count; ++i)
	{
		y = static_cast<double>((i % 5)) * distance;
		x = (floor(i / 5) - 2.5) * distance;
		//formation->AddSlot<GroupManager::FormationSlotRadial>(angle, 3);
		formation->AddSlot<GroupManager::FormationSlotRelative>(x, y);
		angle += M_PI * 2 / Count;
		size_t size = 1;
		if (auto map = Game::GetGlobalMap())
			size = Game::GetGlobalSettings().Resolution.Width / Game::GetGlobalMap()->GetBounds().Width;

		auto unit = make_shared<TestGame::Unit>(size, GetTeam());
		formation->AcceptUnit(unit);
		Units.push_back(unit);
	}
}

void Squad::UpdatePosition(sf::Vector2f Location, sf::Vector2f Orientation, bool Teleport)
{
	Group->UpdatePosition({Location.x,Location.y}, {Orientation.x,Orientation.y});
	Position = Location;
	/*if(Teleport)
		for (const auto& unit : Units)
			unit->SetLocation(Location);*/
}

void Squad::Goto(const sf::Vector2f Location,const sf::Vector2f Orientation)
{
	sf::Vector2f orientation;

	if (Orientation.x == 0 && Orientation.y == 0)
		orientation = Location - Position;
	else
		orientation = Orientation;

	UpdatePosition(Location, orientation);
}

void Squad::Attack(shared_ptr<Squad> Target)
{
	auto enemyLocation = Target->GetLocation();
	auto enemyGroup = Target->GetGroup();
	try
	{
		Group->Attack(enemyGroup);
	}
	catch (const GroupManager::Exceptions::GroupManagerBaseException& e)
	{
		throw GameException(e.what());
	}
	
}

ETeam Squad::GetTeam() const
{
	if (Group->GetTeamStatus() == GroupManager::ETeam::ALLY)
		return ETeam::Blue;
	else
		return ETeam::Red;
}
