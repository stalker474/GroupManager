#include "FormationSlot.h"

using namespace GroupManager;

FormationSlot::FormationSlot()
{
}

void FormationSlot::AssignUnit(shared_ptr<IGroupUnit> Unit)
{
	if (Unit == nullptr)
		throw InvalidFormationInputException("Unit is null");
	if (CurrentUnit)
		CurrentUnit->OnSlotDetached(this);
	CurrentUnit = Unit;
	CurrentUnit->OnSlotAssigned(this);
	CurrentUnit->OnSlotLocationChanged(this);
}

void FormationSlotRadial::UpdatePosition(const Vector2d FormationLocation, const Vector2d FormationDirection)
{
	Vector2d v{ 1,0 };
	v.rotate(v.angle(FormationDirection));
	v.rotate(Theta);
	v.normalize();
	Position = FormationLocation + v * Distance;

	if (CurrentUnit)
		CurrentUnit->OnSlotLocationChanged(this);
}

void FormationSlotRelative::UpdatePosition(const Vector2d FormationLocation, const Vector2d FormationDirection)
{
	Vector2d dir{ 1,0 };
	Vector2d v{ X,Y };
	double angle = dir.angle(FormationDirection);
	double deg = angle /3.14 * 180.0;
	if (v.length() > 0)
		v.rotate(dir.angle(FormationDirection));
		
	Position = FormationLocation + v;

	if (CurrentUnit)
		CurrentUnit->OnSlotLocationChanged(this);
}
