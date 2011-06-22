#include "IRC.h"
#include <pthread.h>
#include <iostream>
#include <string>
#include <string.h>
#include <malloc.h>
#include <vector>
#include "ircstack.h"

#define MYNICKNAME "jeffcooper_bot"

using namespace std;

stack* thestack;


typedef struct prog_info
{
	IRC* irc;
	stack* s;
} prog_info_info;


vector<string> stringsplit(string s, char* del)
{
        vector<string> output;
        char* tmp = strdup(s.c_str());
        char* tok = strtok(tmp,del);
        if(tok == NULL)
            return output;
        else
            output.push_back(string(tok));

        while(tok = strtok(NULL, del))
        {
            if(tok != "")
                output.push_back(string(tok));
        }

        free(tmp);
        free(tok);

        return output;
}
vector<string> stringsplit_n(string s, char* del, int lim)
{
        vector<string> output;
        char* tmp = strdup(s.c_str());
        char* tok = strtok(tmp,del);
        if(tok == NULL)
            return output;
        else
            output.push_back(string(tok));

        int i=1;
        while(tok = strtok(NULL, (i >= lim) ? "" : del))
        {
            if(tok != "")
            {
                output.push_back(string(tok));
                i++;
            }
        }

        free(tmp);
        free(tok);

        return output;
}


void* REPL(void* a)
{
	prog_info* r = (prog_info*)a;

        string s;
	getline(cin, s);
        vector<string> cmd;


	while( s != "quit" )
	{
		getline(cin, s);
                cmd = stringsplit(s, " ");
	}
        

	return NULL;
}
		
int handlemsg(char* params, irc_reply_data* hostd, void* conn)
{
    //cout << "\t"+string(params) << endl << "\t"+string(hostd->nick) << endl << "\t"+string(hostd->target) << endl;

    vector<string> cmd = stringsplit(string(params), " ");
    IRC* irc = (IRC*)conn;
    
    if(cmd.size() < 2 || cmd[0].find(MYNICKNAME) == string::npos)
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
                thestack->push(new stackentry(out3[2]));
                irc->privmsg(hostd->target, "pushed %s", thestack->peek()->print().c_str());
            }
            else
            {
                irc->privmsg(hostd->target, "Sorry, %s, the usage is #MYNICKNAME: pushi <value>", hostd->nick);
            }
        }
        else if(cmd[1] == "pop")
        {
            stackentry* entry = thestack->pop();
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
            stackentry* entry = thestack->peek();
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
            stackentry* entry = thestack->peek();
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
            stackentry* entry = thestack->peek();
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
            if(thestack->getSize() < 2)
            {
                irc->privmsg(hostd->target, "Not enough entries on the stack!");
            }
            else
            {
                stackentry* b = thestack->pop();
                stackentry* a = thestack->pop();
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
                        thestack->push(new stackentry((int)val));
                    else
                        thestack->push(new stackentry(val));
                }
            }
        }
        else if(cmd[1] == "sub")
        {
            if(thestack->getSize() < 2)
            {
                irc->privmsg(hostd->target, "Not enough entries on the stack!");
            }
            else
            {
                stackentry* b = thestack->pop();
                stackentry* a = thestack->pop();
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
                        thestack->push(new stackentry((int)val));
                    else
                        thestack->push(new stackentry(val));
                }
            }
        }
        else if(cmd[1] == "mul")
        {
            if(thestack->getSize() < 2)
            {
                irc->privmsg(hostd->target, "Not enough entries on the stack!");
            }
            else
            {
                stackentry* b = thestack->pop();
                stackentry* a = thestack->pop();
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
                        thestack->push(new stackentry((int)val));
                    else
                        thestack->push(new stackentry(val));
                }
            }
        }
        else if(cmd[1] == "div")
        {
            if(thestack->getSize() < 2)
            {
                irc->privmsg(hostd->target, "Not enough entries on the stack!");
            }
            else
            {
                stackentry* b = thestack->pop();
                stackentry* a = thestack->pop();
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
                            thestack->push(new stackentry((int)val));
                        else
                            thestack->push(new stackentry(val));
                    }
                }
            }
        }
        else if(cmd[1] == "shr")
        {
            if(thestack->getSize() < 1)
            {
                irc->privmsg(hostd->target, "Not enough entries on the stack!");
            }
            else
            {
                stackentry* a = thestack->pop();
                if(a == NULL)
                {
                    irc->privmsg(hostd->target, "The stack is empty!");
                }
                else if(a->mytype != S_INT)
                {
                    irc->privmsg(hostd->target, "Top of the stack is not an integer!");
                    thestack->push(a);
                }
                else
                {
                    irc->privmsg(hostd->target, "Shifted");
                    thestack->push(new stackentry(a->i >> 1));
                }
            }
        }
        else if(cmd[1] == "shl")
        {
            if(thestack->getSize() < 1)
            {
                irc->privmsg(hostd->target, "Not enough entries on the stack!");
            }
            else
            {
                stackentry* a = thestack->pop();
                if(a == NULL)
                {
                    irc->privmsg(hostd->target, "The stack is empty!");
                }
                else if(a->mytype != S_INT)
                {
                    irc->privmsg(hostd->target, "Top of the stack is not an integer!");
                    thestack->push(a);
                }
                else
                {
                    irc->privmsg(hostd->target, "Shifted");
                    thestack->push(new stackentry(a->i << 1));
                }
            }
        }
        else if(cmd[1] == "and")
        {
            if(thestack->getSize() < 2)
            {
                irc->privmsg(hostd->target, "Not enough entries on the stack!");
            }
            else
            {
                stackentry* b = thestack->pop();
                stackentry* a = thestack->pop();
                if(a == NULL || b == NULL)
                {
                    irc->privmsg(hostd->target, "Not enough entries on the stack!");
                }
                else if(a->mytype != S_INT || b->mytype != S_INT)
                {
                    irc->privmsg(hostd->target, "Top of the stack is not an integer!");
                    thestack->push(a);
                    thestack->push(b);
                }
                else
                {
                    irc->privmsg(hostd->target, "Or'd");
                    thestack->push(new stackentry(a->i|b->i));
                }
            }
        }
        else if(cmd[1] == "or")
        {
            if(thestack->getSize() < 2)
            {
                irc->privmsg(hostd->target, "Not enough entries on the stack!");
            }
            else
            {
                stackentry* b = thestack->pop();
                stackentry* a = thestack->pop();
                if(a == NULL || b == NULL)
                {
                    irc->privmsg(hostd->target, "Not enough entries on the stack!");
                }
                else if(a->mytype != S_INT || b->mytype != S_INT)
                {
                    irc->privmsg(hostd->target, "Top of the stack is not an integer!");
                    thestack->push(a);
                    thestack->push(b);
                }
                else
                {
                    irc->privmsg(hostd->target, "Or'd");
                    thestack->push(new stackentry(a->i|b->i));
                }
            }
        }
    }            
}

void* runloop(void* c)
{
	((IRC*)c)->join("#bossko-hii");
	((IRC*)c)->message_loop();
	return NULL;
}
int main()
{
	IRC con;
	thestack = new stack();
	con.start("irc.freenode.net", 6667, MYNICKNAME, "jeffcooper_bot", "jeffcooper_bot", "");
        con.hook_irc_command("PRIVMSG", &handlemsg);

	prog_info *r = (prog_info*)malloc(sizeof(struct prog_info));
	r->irc=&con;
	r->s=thestack;


        pthread_t p;
	pthread_create(&p, NULL, runloop,&con);


	REPL((void*)r);
	pthread_cancel(p);
	return 0;
}
