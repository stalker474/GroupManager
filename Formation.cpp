#include "Formation.h"

using namespace GroupManager;

Formation::Formation(const EFormationType Type, shared_ptr<IGroup> MyGroup) : 
	Type(Type)
	,MyGroup(MyGroup)
{
	if(Type == EFormationType::Free)
		throw Exceptions::GroupManagerBaseException("Free formations are not supported");
	ObstacleStrategy = EFormationObstacleHandlingStrategy::Static;
	if (MyGroup == nullptr)
		throw FormationNotInsideAGroupException();
}

void Formation::SetObstacleHandlingStrategy(EFormationObstacleHandlingStrategy Strategy)
{
	ObstacleStrategy = Strategy;
}

void Formation::AcceptUnit(shared_ptr<IGroupUnit> Unit, shared_ptr<FormationSlot> Slot)
{
	if (Unit == nullptr)
		throw InvalidUnitException();
	if (MyGroup == nullptr)
		throw FormationNotInsideAGroupException();
	if(FindUnit(Unit))
		throw UnitAlreadyInFormationException();
	if (Unit->GetTeamStatus() != GetTeamStatus())
		throw WrongTeamException();
	if (Type != EFormationType::Free && FindEmptySlot() == nullptr )
		throw NoEmptySlotsException();
	if (Slot != nullptr && std::find(Slots.begin(), Slots.end(), Slot) == Slots.end())
		throw InvalidSlotException();
	if (Slot == nullptr && Type != EFormationType::Free)
	{
		Slot = FindEmptySlot(); //we know we have an empty slot
		if(Slot == nullptr)
			throw NoEmptySlotsException(); //possibly MT problem
	}
	else
	{
		//free formation, just create a random slot
		throw Exceptions::GroupManagerBaseException("Free formations are not supported");
	}
	Slot->AssignUnit(Unit);
}

void Formation::UpdatePosition(const Vector2d NewPosition, const Vector2d Orientation)
{
	for (const auto& slot : Slots)
		slot->UpdatePosition(NewPosition, Orientation);
}

shared_ptr<FormationSlot> Formation::FindEmptySlot() const
{
	for (const auto& slot : Slots)
		if (slot->IsEmpty())
			return slot;
	return nullptr;
}

bool Formation::FindUnit(shared_ptr<IGroupUnit> Unit) const
{
	for (const auto& slot : Slots)
		if (!slot->IsEmpty())
			if (slot->GetUnit() == Unit)
				return true;

	return false;
}
