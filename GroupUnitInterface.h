#ifndef IGROUPUNIT_H
#define IGROUPUNIT_H

#include "TeamInterface.h"

namespace GroupManager
{
	class FormationSlot;
	class IGroupUnit : public ITeam
	{
	public:
		//events
		virtual void OnSlotAssigned(const FormationSlot* Slot) = 0;
		virtual void OnSlotDetached(const FormationSlot* Slot) = 0;
		virtual void OnSlotLocationChanged(const FormationSlot* Slot) = 0;
	};
}

#endif

