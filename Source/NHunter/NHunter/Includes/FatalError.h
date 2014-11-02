#ifndef FATALERROR_H
#define FATALERROR_H

#include <string>

class FatalError
{
private:
	std::string m_Msg;

public:
	FatalError():m_Msg("Unknown fatal error"){}
	FatalError(std::string msg):m_Msg(msg){}

	void 		setMsg(std::string msg){m_Msg=msg;}
	std::string getMsg(){return m_Msg;}
};

#endif