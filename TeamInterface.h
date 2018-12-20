#ifndef TEAMINTERFACE_H
#define TEAMINTERFACE_H

namespace GroupManager
{
	enum class ETeam
	{
		ALLY,
		ENEMY
	};
	class ITeam
	{
	public:
		virtual const ETeam GetTeamStatus() const noexcept = 0;
	};
}

#endif
