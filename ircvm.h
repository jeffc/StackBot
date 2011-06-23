#ifndef _IRCVM_H_
#define _IRCVM_H_

#include "util.h"
#include <string>
#include <map>
#include <vector>
#include "ircstack.h"
#include "IRC.h"


class IRCVM
{
	public:
		IRCVM(IRC* irc);

		bool handleCommand(std::string cmd, irc_reply_data* d);
		void printOff();
		void printOn();

		bool printing;

		void setRegister(char key, stackentry* e);
		stackentry* getRegister(char key); //this returns a copy
		stack* S;
		IRC* irc;
		
	private:
		std::map<char, stackentry*> registers;
		
};

#endif
