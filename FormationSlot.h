#ifndef FORMATIONSLOT_H
#define FORMATIONSLOT_H

#include "GroupUnitInterface.h"
#include "Math.h"
#include "GroupManagerExceptions.h"
#include <memory>

using namespace std;

namespace GroupManager
{
	class FormationSlot
	{
	public:
		FormationSlot();
		void AssignUnit(shared_ptr<IGroupUnit> Unit);
		inline bool IsEmpty() const { return CurrentUnit == nullptr; }
		virtual void UpdatePosition(const Vector2d FormationLocation, const Vector2d FormationDirection) = 0;
		inline const Vector2d GetPosition() const { return Position; }
		inline shared_ptr<IGroupUnit> GetUnit() const { return CurrentUnit; }
	protected:
		shared_ptr<IGroupUnit> CurrentUnit;
		Vector2d Position;
	};

	class FormationSlotRadial : public FormationSlot
	{
	public:
		FormationSlotRadial(const double Theta, const double Distance) 
			: FormationSlot(),Theta(Theta)
			,Distance(Distance)
		{}
		void UpdatePosition(const Vector2d FormationLocation, const Vector2d FormationDirection) override;
	private:
		double Theta;
		double Distance;	
	};

	class FormationSlotRelative : public FormationSlot
	{
	public:
		FormationSlotRelative(const double X, const double Y)
			: FormationSlot(), X(X),Y(Y)
		{}
		void UpdatePosition(const Vector2d FormationLocation, const Vector2d FormationDirection) override;
	private:
		double X;
		double Y;
	};

	class InvalidFormationInputException : public Exceptions::GroupManagerBaseException
	{
	public:
		InvalidFormationInputException(const std::string Message) : GroupManagerBaseException(Message) {}
	};
}

#endif