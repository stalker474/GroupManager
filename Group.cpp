#include "Group.h"

using namespace GroupManager;

Group::Group(const ETeam Team) : Team(Team)
{
}

void Group::AssignFormationStateMachine(shared_ptr<GroupManager::FormationStateMachine> NewFSM)
{
	FSM = NewFSM;

	try
	{
		FSM->IsValid();
	}
	catch (const Exceptions::GroupManagerBaseException& e)
	{
		throw InvalidFormationException(e.what());
	}

	if (FSM->GetCurrentState() == GroupManager::EFormationState::Init)
		FSM->Step();
}

void Group::UpdatePosition(const Vector2d NewPosition, const Vector2d NewOrientation)
{
	Position = NewPosition;
	Orientation = NewOrientation;
	FSM->GetCurrentFormation()->UpdatePosition(NewPosition,Orientation);
}

void Group::Attack(shared_ptr<Group> Target)
{
	if (Target == nullptr)
		throw GroupAttackException("Target is null");
	if (Target.get() == this)
		throw GroupAttackException("Cannot attack self");
	if (Target->GetTeamStatus() == GetTeamStatus())
		throw GroupAttackException("Cannot attack ally");

	Bind(Target);
}

void Group::Bind(shared_ptr<Group> Target)
{
	if (Target == nullptr)
		throw GroupAttackException("Target is null");
	if (Target.get() == this)
		throw GroupAttackException("Cannot bind to self");

	//Target->GetFSM()->Subscribe();
}
