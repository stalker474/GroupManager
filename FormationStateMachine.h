#ifndef FORMATIONSTATEMACHINE_H
#define FORMATIONSTATEMACHINE_H

#include "Formation.h"
#include "GroupManagerExceptions.h"
#include <vector>
#include <unordered_set>
#include <memory>

using namespace std;

namespace GroupManager
{
	enum class EFormationState
	{
		Init,
		Idle,
		Moving,
		Charging,
		Transitionning
	};

	class FormationStateMachineListener;

	class FormationStateMachine : public enable_shared_from_this<FormationStateMachine>
	{
	public:
		FormationStateMachine();
		inline EFormationState GetCurrentState() const { return State; }
		inline shared_ptr<Formation> GetCurrentFormation() const { return CurrentFormation; }
		void AddFormations(const vector<shared_ptr<Formation>>& NewFormation);
		void Step();
		void IsValid() const;
		void Subscribe(shared_ptr<FormationStateMachineListener> Listener);
		void Unsubscribe(shared_ptr<FormationStateMachineListener> Listener);
	private:
		EFormationState State;
		vector<shared_ptr<Formation>> Formations;
		shared_ptr<Formation> CurrentFormation;
		unordered_set<shared_ptr<FormationStateMachineListener>> Listeners;
	};

	class FormationStateMachineBuilder
	{
	public:
		inline void AddFormations(vector<shared_ptr<Formation>> NewFormations)
		{
			Formations.insert(Formations.end(), NewFormations.begin(),NewFormations.end());
		}
		shared_ptr<FormationStateMachine> Build()
		{
			auto form = make_shared<FormationStateMachine>();
			form->AddFormations(Formations);
			form->IsValid();
			return form;
		}
	private:
		vector<shared_ptr<Formation>> Formations;
	};

	class FormationStateMachineListener
	{
	public:
		virtual void OnStateChanged(shared_ptr<FormationStateMachine> FSM) = 0;
	};

	class NoFormationsException : public Exceptions::GroupManagerBaseException
	{
	public:
		NoFormationsException() : GroupManagerBaseException("No formations specified") {}
	};

	class InvalidFormationListerException : public Exceptions::GroupManagerBaseException
	{
	public:
		InvalidFormationListerException() : GroupManagerBaseException("Listener is null") {}
	};
}

#endif