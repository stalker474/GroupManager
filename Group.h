#ifndef GROUP_H
#define GROUP_H

#include "GroupInterface.h"
#include "GroupManagerExceptions.h"
#include "FormationStateMachine.h"
#include "Math.h"

namespace GroupManager
{
	class Group : public GroupManager::IGroup
	{
	public:
		Group(const ETeam Team);
		void AssignFormationStateMachine(shared_ptr<GroupManager::FormationStateMachine> NewFSM) override;
		void UpdatePosition(const Vector2d NewPosition, const Vector2d NewOrientation);
		void Attack(shared_ptr<Group> Target);
		void Bind(shared_ptr<Group> Target);
		inline shared_ptr<GroupManager::FormationStateMachine> GetFSM() const noexcept { return FSM; }

		const ETeam GetTeamStatus() const noexcept override { return Team; }
		inline const Vector2d GetPosition() const { return Position; }
		inline const Vector2d GetOrientation() const { return Orientation; }
	private:
		shared_ptr<GroupManager::FormationStateMachine> FSM;
		ETeam Team;
		Vector2d Position;
		Vector2d Orientation;
	};

	class InvalidFormationException : public Exceptions::GroupManagerBaseException
	{
	public:
		InvalidFormationException(const std::string Message) : GroupManagerBaseException(Message) {}
	};

	class GroupAttackException : public Exceptions::GroupManagerBaseException
	{
	public:
		GroupAttackException(const std::string Message) : GroupManagerBaseException(Message) {}
	};
}

#endif

