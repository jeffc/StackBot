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
                irc->privmsg(hostd->target, "pushed %s", S->peek()->print().c_str());
            }
            else
            {
                irc->privmsg(hostd->target, "Sorry, %s, the usage is #MYNICKNAME: pushi <value>", hostd->nick);
            }
        }
        else if(cmd[1] == "pop")
        {
            stackentry* entry = S->pop();
            if(entry == NULL)
            {
                irc->privmsg(hostd->target, "The stack is empty!");
            }
            else
            {
                irc->privmsg(hostd->target, "popped %s", entry->print().c_str());
            }
        }
        else if(cmd[1] == "peek")
        {
            stackentry* entry = S->peek();
            if(entry == NULL)
            {
                irc->privmsg(hostd->target, "The stack is empty!");
            }
            else
            {
                irc->privmsg(hostd->target, "top of the stack is %s", entry->print().c_str());
            }
        }
        else if(cmd[1] == "inc")
        {
            stackentry* entry = S->peek();
            if(entry == NULL)
            {
                irc->privmsg(hostd->target, "The stack is empty!");
            }
            else if(entry->mytype != S_INT && entry->mytype != S_FLOAT)
            {
                irc->privmsg(hostd->target, "Top of the stack is not a number!");
            }
            else
            {
                irc->privmsg(hostd->target, "Incremented");
                if(entry->mytype == S_INT)
                {
                    entry->i++;
                }
                else if(entry->mytype == S_FLOAT)
                {
                    entry->f++;
                }
            }
        }
        else if(cmd[1] == "dec")
        {
            stackentry* entry = S->peek();
            if(entry == NULL)
            {
                irc->privmsg(hostd->target, "The stack is empty!");
            }
            else if(entry->mytype != S_INT && entry->mytype != S_FLOAT)
            {
                irc->privmsg(hostd->target, "Top of the stack is not a number!");
            }
            else
            {
                irc->privmsg(hostd->target, "Decremented");
                if(entry->mytype == S_INT)
                {
                    entry->i--;
                }
                else if(entry->mytype == S_FLOAT)
                {
                    entry->f--;
                }
            }
        }
        else if(cmd[1] == "add")
        {
            if(S->getSize() < 2)
            {
                irc->privmsg(hostd->target, "Not enough entries on the stack!");
            }
            else
            {
                stackentry* b = S->pop();
                stackentry* a = S->pop();
                if(a == NULL || b == NULL)
                {
                    irc->privmsg(hostd->target, "The stack is empty!");
                }
                else if(!a->isNumber() || !b->isNumber())
                {
                    irc->privmsg(hostd->target, "Top of the stack is not a number!");
                }
                else
                {
                    irc->privmsg(hostd->target, "Added");
                    float val = a->floatVal() + b->floatVal();
                    if(fmod(val, 1.0) == 0.0)
                        S->push(new stackentry((int)val));
                    else
                        S->push(new stackentry(val));
                }
            }
        }
        else if(cmd[1] == "sub")
        {
            if(S->getSize() < 2)
            {
                irc->privmsg(hostd->target, "Not enough entries on the stack!");
            }
            else
            {
                stackentry* b = S->pop();
                stackentry* a = S->pop();
                if(a == NULL || b == NULL)
                {
                    irc->privmsg(hostd->target, "The stack is empty!");
                }
                else if(!a->isNumber() || !b->isNumber())
                {
                    irc->privmsg(hostd->target, "Top of the stack is not a number!");
                }
                else
                {
                    irc->privmsg(hostd->target, "Subtracted");
                    float val = a->floatVal() - b->floatVal();
                    if(fmod(val, 1.0) == 0.0)
                        S->push(new stackentry((int)val));
                    else
                        S->push(new stackentry(val));
                }
            }
        }
        else if(cmd[1] == "mul")
        {
            if(S->getSize() < 2)
            {
                irc->privmsg(hostd->target, "Not enough entries on the stack!");
            }
            else
            {
                stackentry* b = S->pop();
                stackentry* a = S->pop();
                if(a == NULL || b == NULL)
                {
                    irc->privmsg(hostd->target, "The stack is empty!");
                }
                else if(!a->isNumber() || !b->isNumber())
                {
                    irc->privmsg(hostd->target, "Top of the stack is not a number!");
                }
                else
                {
                    irc->privmsg(hostd->target, "Multiplied");
                    float val = a->floatVal() * b->floatVal();
                    if(fmod(val, 1.0) == 0.0)
                        S->push(new stackentry((int)val));
                    else
                        S->push(new stackentry(val));
                }
            }
        }
        else if(cmd[1] == "div")
        {
            if(S->getSize() < 2)
            {
                irc->privmsg(hostd->target, "Not enough entries on the stack!");
            }
            else
            {
                stackentry* b = S->pop();
                stackentry* a = S->pop();
                if(a == NULL || b == NULL)
                {
                    irc->privmsg(hostd->target, "The stack is empty!");
                }
                else if(!a->isNumber() || !b->isNumber())
                {
                    irc->privmsg(hostd->target, "Top of the stack is not a number!");
                }
                else
                {
                    if(b->floatVal() == 0.0)
                    {
                        irc->privmsg(hostd->target, "Error: Universe Implosion");
                    }
                    else
                    {
                        irc->privmsg(hostd->target, "Divided");
                        float val = a->floatVal() / b->floatVal();
                        if(fmod(val, 1.0) == 0.0)
                            S->push(new stackentry((int)val));
                        else
                            S->push(new stackentry(val));
                    }
                }
            }
        }
        else if(cmd[1] == "shr")
        {
            if(S->getSize() < 1)
            {
                irc->privmsg(hostd->target, "Not enough entries on the stack!");
            }
            else
            {
                stackentry* a = S->pop();
                if(a == NULL)
                {
                    irc->privmsg(hostd->target, "The stack is empty!");
                }
                else if(a->mytype != S_INT)
                {
                    irc->privmsg(hostd->target, "Top of the stack is not an integer!");
                    S->push(a);
                }
                else
                {
                    irc->privmsg(hostd->target, "Shifted");
                    S->push(new stackentry(a->i >> 1));
                }
            }
        }
        else if(cmd[1] == "shl")
        {
            if(S->getSize() < 1)
            {
                irc->privmsg(hostd->target, "Not enough entries on the stack!");
            }
            else
            {
                stackentry* a = S->pop();
                if(a == NULL)
                {
                    irc->privmsg(hostd->target, "The stack is empty!");
                }
                else if(a->mytype != S_INT)
                {
                    irc->privmsg(hostd->target, "Top of the stack is not an integer!");
                    S->push(a);
                }
                else
                {
                    irc->privmsg(hostd->target, "Shifted");
                    S->push(new stackentry(a->i << 1));
                }
            }
        }
        else if(cmd[1] == "and")
        {
            if(S->getSize() < 2)
            {
                irc->privmsg(hostd->target, "Not enough entries on the stack!");
            }
            else
            {
                stackentry* b = S->pop();
                stackentry* a = S->pop();
                if(a == NULL || b == NULL)
                {
                    irc->privmsg(hostd->target, "Not enough entries on the stack!");
                }
                else if(a->mytype != S_INT || b->mytype != S_INT)
                {
                    irc->privmsg(hostd->target, "Top of the stack is not an integer!");
                    S->push(a);
                    S->push(b);
                }
                else
                {
                    irc->privmsg(hostd->target, "And'd");
                    S->push(new stackentry(a->i&b->i));
                }
            }
        }
        else if(cmd[1] == "or")
        {
            if(S->getSize() < 2)
            {
                irc->privmsg(hostd->target, "Not enough entries on the stack!");
            }
            else
            {
                stackentry* b = S->pop();
                stackentry* a = S->pop();
                if(a == NULL || b == NULL)
                {
                    irc->privmsg(hostd->target, "Not enough entries on the stack!");
                }
                else if(a->mytype != S_INT || b->mytype != S_INT)
                {
                    irc->privmsg(hostd->target, "Top of the stack is not an integer!");
                    S->push(a);
                    S->push(b);
                }
                else
                {
                    irc->privmsg(hostd->target, "Or'd");
                    S->push(new stackentry(a->i|b->i));
                }
            }
        }
    }            
}