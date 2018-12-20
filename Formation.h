#ifndef FORMATION_H
#define FORMATION_H

#include "GroupUnitInterface.h"
#include "GroupInterface.h"
#include "TeamInterface.h"
#include "GroupManagerExceptions.h"

#include "FormationSlot.h"
#include <memory>
#include <vector>
using namespace std;

namespace GroupManager
{
	enum class EFormationObstacleHandlingStrategy
	{
		Wrap,
		Static
	};
	enum class EFormationType
	{
		Slotted,
		Free
	};

	class Formation : public ITeam
	{
	public:
		Formation(const EFormationType Type, shared_ptr<IGroup> MyGroup);
		void SetObstacleHandlingStrategy(EFormationObstacleHandlingStrategy Strategy);
		void AcceptUnit(shared_ptr<IGroupUnit> Unit, shared_ptr<FormationSlot> Slot = nullptr);
		template<typename T, typename... Args>
		shared_ptr<FormationSlot> AddSlot(Args&&... args)
		{
			Slots.push_back(make_shared<T>(args...));
			return Slots.back();
		}
		void UpdatePosition(const Vector2d NewPosition, const Vector2d Orientation);
		inline const shared_ptr<FormationSlot> GetSlot(const size_t Index) const {
			if (Index >= Slots.size())
				throw Exceptions::GroupManagerBaseException("Index out of bounds!");
			else {
				return Slots[Index];
			}
		}
		
		const ETeam GetTeamStatus() const noexcept override
		{
			return MyGroup->GetTeamStatus();
		}
	private:
		shared_ptr<FormationSlot> FindEmptySlot() const;
		bool FindUnit(shared_ptr<IGroupUnit>) const;
		vector<shared_ptr<FormationSlot>>	Slots;
		EFormationObstacleHandlingStrategy	ObstacleStrategy;
		EFormationType						Type;
		shared_ptr<IGroup>					MyGroup;
	};

	class NoEmptySlotsException : public Exceptions::GroupManagerBaseException
	{
	public:
		NoEmptySlotsException() : Exceptions::GroupManagerBaseException("Not enough empty slots in formation to add a unit!") {};
	};
	class InvalidSlotException : public Exceptions::GroupManagerBaseException
	{
	public:
		InvalidSlotException() : Exceptions::GroupManagerBaseException("While attempting to assign a unit to a formation, you specified a slot not belonging to this formation!") {};
	};
	class InvalidUnitException : public Exceptions::GroupManagerBaseException
	{
	public:
		InvalidUnitException() : Exceptions::GroupManagerBaseException("The unit is an invalid pointer!") {};
	};
	class UnitAlreadyInFormationException : public Exceptions::GroupManagerBaseException
	{
	public:
		UnitAlreadyInFormationException() : Exceptions::GroupManagerBaseException("The unit is an invalid pointer!") {};
	};
	class FormationNotInsideAGroupException : public Exceptions::GroupManagerBaseException
	{
	public:
		FormationNotInsideAGroupException() : Exceptions::GroupManagerBaseException("Attempting to assign a unit to a formation without a group!") {};
	};
	class WrongTeamException : public Exceptions::GroupManagerBaseException
	{
	public:
		WrongTeamException() : Exceptions::GroupManagerBaseException("Attempting to assign a unit to a formation from another team!") {};
	};
}

#endif