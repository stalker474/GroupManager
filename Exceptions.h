#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

class GameException : public std::exception
{
public:
	GameException(const std::string& Msg) : Msg(Msg) {}

	char const* what() const override
	{
		return Msg.c_str();
	}

private:
	std::string Msg;
};

#endif
