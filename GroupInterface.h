#ifndef IGROUP_H
#define IGROUP_H

#include "TeamInterface.h"
#include <memory>

using namespace std;

namespace GroupManager
{
	class FormationStateMachine;
	class IGroup : public ITeam
	{
	public:
		virtual void AssignFormationStateMachine(shared_ptr<FormationStateMachine> FSM) = 0;
	};
}

#endif
