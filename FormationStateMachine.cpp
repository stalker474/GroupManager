#include "FormationStateMachine.h"
#include "GroupManagerExceptions.h"

using namespace GroupManager;

FormationStateMachine::FormationStateMachine()
{
	State = EFormationState::Init;
}

void FormationStateMachine::AddFormations(const vector<shared_ptr<Formation>>& NewFormations)
{
	Formations.insert(Formations.end(), NewFormations.begin(), NewFormations.end());
}

void FormationStateMachine::Step()
{
	IsValid();
	if (State == EFormationState::Init)
		CurrentFormation = Formations[0];
	//if state changed only
	for (auto& listener : Listeners)
		listener->OnStateChanged(shared_from_this());
}

void FormationStateMachine::IsValid() const
{
	if (Formations.size() == 0)
		throw NoFormationsException();
}

void FormationStateMachine::Subscribe(shared_ptr<FormationStateMachineListener> Listener)
{
	if (Listener == nullptr)
		throw InvalidFormationListerException();
	Listeners.insert(Listener);
}

void FormationStateMachine::Unsubscribe(shared_ptr<FormationStateMachineListener> Listener)
{
	if (Listener == nullptr)
		throw InvalidFormationListerException();
	Listeners.erase(Listener);
}
