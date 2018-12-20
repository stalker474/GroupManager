#ifndef GROUPMANAGEREXCEPTIONS_H
#define GROUPMANAGEREXCEPTIONS_H

#include <string>

namespace GroupManager
{
	namespace Exceptions
	{
		class GroupManagerBaseException : public std::exception
		{
		public:
			GroupManagerBaseException(const std::string Msg)
			{
				Message = Msg;
			}

			char const* what() const override
			{
				return Message.c_str();
			}
		private:
			std::string Message;
		};
	}
}

#endif
