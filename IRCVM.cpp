#include "ircvm.h"

IRCVM::IRCVM(IRC* arg_irc)
{
	printing=true;
	for(char i='A';i<='Z';i++)
		registers.insert(std::pair<char,stackentry*>(i,NULL));

	S = new stack();
	irc = arg_irc;
}

void IRCVM::printOn()
{
	printing=true;
}
void IRCVM::printOff()
{
	printing=false;
}

void IRCVM::setRegister(char key, stackentry* e)
{
	if(key < 'A' || key > 'Z')
		return;
	registers[key]=new stackentry(e); //copy constructor
}

stackentry* IRCVM::getRegister(char key)
{
	if(key < 'A' || key > 'Z')
		return NULL;
	return registers[key];
}

bool IRCVM::handleCommand(std::string params, irc_reply_data* hostd)
{
    std::vector<std::string> cmd = stringsplit(params, " ");
    
    if(cmd.size() < 2 || cmd[0].find(MYNICKNAME) == std::string::npos)
    {
        //cout << "\t IGNORING MESSAGE\n";
    }
    else
    {
        if(cmd[1] == "pushi")
        {
            if(cmd.size() >= 3)
            {
                vector<string> out3 = stringsplit_n(string(params), " \n", 2);
                S->push(new stackentry(out3[2]));
                if(printing)
                    irc->privmsg(hostd->target, "pushed %s", S->peek()->print().c_str());
		return true;
            }
            else
            {
                if(printing)
                    irc->privmsg(hostd->target, "Sorry, %s, the usage is #MYNICKNAME: pushi <value>", hostd->nick);
		return false;
            }
        }
        else if(cmd[1] == "pop")
        {
            stackentry* entry = S->pop();
            if(entry == NULL)
            { 
                if(printing)
                    irc->privmsg(hostd->target, "The stack is empty!");
		return false;
            }
            else
            {
                if(printing)
                    irc->privmsg(hostd->target, "popped %s", entry->print().c_str());
		return true;
            }
        }
        else if(cmd[1] == "peek")
        {
            stackentry* entry = S->peek();
            if(entry == NULL)
            {
                if(printing)
                    irc->privmsg(hostd->target, "The stack is empty!");
		return false;
            }
            else
            {
                if(printing)
                    irc->privmsg(hostd->target, "top of the stack is %s", entry->print().c_str());
		return true;
            }
        }
        else if(cmd[1] == "inc")
        {
            stackentry* entry = S->peek();
            if(entry == NULL)
            {
                if(printing)
                    irc->privmsg(hostd->target, "The stack is empty!");
		return false;
            }
            else if(entry->mytype != S_INT && entry->mytype != S_FLOAT)
            {
                if(printing)
                    irc->privmsg(hostd->target, "Top of the stack is not a number!");
		return false;
            }
            else
            {
                if(printing)
                    irc->privmsg(hostd->target, "Incremented");
                if(entry->mytype == S_INT)
                {
                    entry->i++;
                }
                else if(entry->mytype == S_FLOAT)
                {
                    entry->f++;
                }
		return true;
            }
        }
        else if(cmd[1] == "dec")
        {
            stackentry* entry = S->peek();
            if(entry == NULL)
            {
                if(printing)
                    irc->privmsg(hostd->target, "The stack is empty!");
		return false;
            }
            else if(entry->mytype != S_INT && entry->mytype != S_FLOAT)
            {
                if(printing)
                    irc->privmsg(hostd->target, "Top of the stack is not a number!");
		return false;
            }
            else
            {
                if(printing)
                    irc->privmsg(hostd->target, "Decremented");
                if(entry->mytype == S_INT)
                {
                    entry->i--;
                }
                else if(entry->mytype == S_FLOAT)
                {
                    entry->f--;
                }
		return true;
            }
        }
        else if(cmd[1] == "add")
        {
            if(S->getSize() < 2)
            {
                if(printing)
                    irc->privmsg(hostd->target, "Not enough entries on the stack!");
		return false;
            }
            else
            {
                stackentry* b = S->pop();
                stackentry* a = S->pop();
                if(a == NULL || b == NULL)
                {
                    if(printing)
                        irc->privmsg(hostd->target, "The stack is empty!");
		    return false;
                }
                else if(!a->isNumber() || !b->isNumber())
                {
                    if(printing)
                        irc->privmsg(hostd->target, "Top of the stack is not a number!");
		    false;
                }
                else
                {
                    if(printing)
                        irc->privmsg(hostd->target, "Added");
                    float val = a->floatVal() + b->floatVal();
                    if(fmod(val, 1.0) == 0.0)
                        S->push(new stackentry((int)val));
                    else
                        S->push(new stackentry(val));
		    return true;
                }
            }
        }
        else if(cmd[1] == "sub")
        {
            if(S->getSize() < 2)
            {
                if(printing)
                    irc->privmsg(hostd->target, "Not enough entries on the stack!");
		return false;
            }
            else
            {
                stackentry* b = S->pop();
                stackentry* a = S->pop();
                if(a == NULL || b == NULL)
                {
                    if(printing)
                        irc->privmsg(hostd->target, "The stack is empty!");
		    return false;
                }
                else if(!a->isNumber() || !b->isNumber())
                {
                    if(printing)
                        irc->privmsg(hostd->target, "Top of the stack is not a number!");
		    return false;
                }
                else
                {
                    if(printing)
                        irc->privmsg(hostd->target, "Subtracted");
                    float val = a->floatVal() - b->floatVal();
                    if(fmod(val, 1.0) == 0.0)
                        S->push(new stackentry((int)val));
                    else
                        S->push(new stackentry(val));

		    return true;
                }
            }
        }
        else if(cmd[1] == "mul")
        {
            if(S->getSize() < 2)
            {
                if(printing)
                    irc->privmsg(hostd->target, "Not enough entries on the stack!");
		return false;
            }
            else
            {
                stackentry* b = S->pop();
                stackentry* a = S->pop();
                if(a == NULL || b == NULL)
                {
                    if(printing)
                        irc->privmsg(hostd->target, "The stack is empty!");
		    return false;
                }
                else if(!a->isNumber() || !b->isNumber())
                {
                    if(printing)
                        irc->privmsg(hostd->target, "Top of the stack is not a number!");
		    return false;
                }
                else
                {
                    if(printing)
                        irc->privmsg(hostd->target, "Multiplied");
                    float val = a->floatVal() * b->floatVal();
                    if(fmod(val, 1.0) == 0.0)
                        S->push(new stackentry((int)val));
                    else
                        S->push(new stackentry(val));

		    return true;
                }
            }
        }
        else if(cmd[1] == "div")
        {
            if(S->getSize() < 2)
            {
                if(printing)
                    irc->privmsg(hostd->target, "Not enough entries on the stack!");
		return false;
            }
            else
            {
                stackentry* b = S->pop();
                stackentry* a = S->pop();
                if(a == NULL || b == NULL)
                {
                    if(printing)
                        irc->privmsg(hostd->target, "The stack is empty!");
		    return false;
                }
                else if(!a->isNumber() || !b->isNumber())
                {
                    if(printing)
                        irc->privmsg(hostd->target, "Top of the stack is not a number!");
		    return false;
                }
                else
                {
                    if(b->floatVal() == 0.0)
                    {
                        if(printing)
                            irc->privmsg(hostd->target, "Error: Universe Implosion");
			return false;
                    }
                    else
                    {
                        if(printing)
                            irc->privmsg(hostd->target, "Divided");
                        float val = a->floatVal() / b->floatVal();
                        if(fmod(val, 1.0) == 0.0)
                            S->push(new stackentry((int)val));
                        else
                            S->push(new stackentry(val));

			return true;
                    }
                }
            }
        }
        else if(cmd[1] == "shr")
        {
            if(S->getSize() < 1)
            {
                if(printing)
                    irc->privmsg(hostd->target, "Not enough entries on the stack!");
		return false;
            }
            else
            {
                stackentry* a = S->pop();
                if(a == NULL)
                {
                    if(printing)
                        irc->privmsg(hostd->target, "The stack is empty!");
		    return false;
                }
                else if(a->mytype != S_INT)
                {
                    if(printing)
                        irc->privmsg(hostd->target, "Top of the stack is not an integer!");
                    S->push(a);
		    return false;
                }
                else
                {
                    if(printing)
                        irc->privmsg(hostd->target, "Shifted");
                    S->push(new stackentry(a->i >> 1));
		    return true;
                }
            }
        }
        else if(cmd[1] == "shl")
        {
            if(S->getSize() < 1)
            {
                if(printing)
                    irc->privmsg(hostd->target, "Not enough entries on the stack!");
		return false;
            }
            else
            {
                stackentry* a = S->pop();
                if(a == NULL)
                {
                    if(printing)
                        irc->privmsg(hostd->target, "The stack is empty!");
		    return false;
                }
                else if(a->mytype != S_INT)
                {
                    if(printing)
                        irc->privmsg(hostd->target, "Top of the stack is not an integer!");
                    S->push(a);
		    return true;
                }
                else
                {
                    if(printing)
                        irc->privmsg(hostd->target, "Shifted");
                    S->push(new stackentry(a->i << 1));
		    return true;
                }
            }
        }
        else if(cmd[1] == "and")
        {
            if(S->getSize() < 2)
            {
                if(printing)
                    irc->privmsg(hostd->target, "Not enough entries on the stack!");
		return false;
            }
            else
            {
                stackentry* b = S->pop();
                stackentry* a = S->pop();
                if(a == NULL || b == NULL)
                {
                    if(printing)
                        irc->privmsg(hostd->target, "Not enough entries on the stack!");
		    return false;
                }
                else if(a->mytype != S_INT || b->mytype != S_INT)
                {
                    if(printing)
                        irc->privmsg(hostd->target, "Top of the stack is not an integer!");
                    S->push(a);
                    S->push(b);

		    return false;
                }
                else
                {
                    if(printing)
                        irc->privmsg(hostd->target, "And'd");
                    S->push(new stackentry(a->i&b->i));
		    
		    return true;
                }
            }
        }
        else if(cmd[1] == "or")
        {
            if(S->getSize() < 2)
            {
                if(printing)
                    irc->privmsg(hostd->target, "Not enough entries on the stack!");
		return true;
            }
            else
            {
                stackentry* b = S->pop();
                stackentry* a = S->pop();
                if(a == NULL || b == NULL)
                {
                    if(printing)
                        irc->privmsg(hostd->target, "Not enough entries on the stack!");
		    return false;
                }
                else if(a->mytype != S_INT || b->mytype != S_INT)
                {
                    if(printing)
                        irc->privmsg(hostd->target, "Top of the stack is not an integer!");
                    S->push(a);
                    S->push(b);

		    return false;
                }
                else
                {
                    if(printing)
                        irc->privmsg(hostd->target, "Or'd");
                    S->push(new stackentry(a->i|b->i));
		    
		    return true;
                }
            }
        }
        if(cmd[1] == "push")
        {
            if(cmd.size() >= 3)
            {
                vector<string> out3 = stringsplit_n(string(params), " \n", 2);
                
                if(registers[out3[2][0]] != NULL)
                {
                    S->push(new stackentry(registers[out3[2][0]]));
                    if(printing)
                        irc->privmsg(hostd->target, "pushed");
		    return true;
                }
                else
                {
                    if(printing)
                        irc->privmsg(hostd->target, "Register contains nothing!");

		    return false;
                }
            }
            else
            {
                if(printing)
                    irc->privmsg(hostd->target, "Sorry, %s, the usage is #MYNICKNAME: push <register> (registers are A-Z)", hostd->nick);
		return false;
            }
        }
        if(cmd[1] == "ldi")
        {
            if(cmd.size() >= 4)
            {
                vector<string> out3 = stringsplit_n(string(params), " \n", 3);
                setRegister(out3[2][0], new stackentry(out3[3]));
                if(printing)
                    irc->privmsg(hostd->target, "loaded");
		return true;
            }
            else
            {
                if(printing)
                    irc->privmsg(hostd->target, "Sorry, %s, the usage is #MYNICKNAME: ldi <register> <value> (registers are A-Z)", hostd->nick);
		return false;
            }
        }
        if(cmd[1] == "popr")
        {
            if(cmd.size() >= 3)
            {
                vector<string> out3 = stringsplit_n(string(params), " \n", 2);
                stackentry* a = S->pop();

                if(a == NULL)
                {
                    if(printing)
                        irc->privmsg(hostd->target, "Nothing on the stack!");
                    return false;
                }
                else
                {
                    setRegister(out3[2][0], a);
                    if(printing)
                        irc->privmsg(hostd->target, "Popped");
                }
            }
            else
            {
                if(printing)
                    irc->privmsg(hostd->target, "Sorry, %s, the usage is #MYNICKNAME: push <register> (registers are A-Z)", hostd->nick);
		return false;
            }
        }
        else if(cmd[1] == "prtoff")
        {
            printing=false;
	    return true;
        }
        else if(cmd[1] == "prton")
        {
            printing=true;
            irc->privmsg(hostd->target, "Printing enabled");
	    return true;
        }
    }            
}
