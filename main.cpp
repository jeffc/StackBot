#include "util.h"
#include "IRC.h"
#include <pthread.h>
#include <iostream>
#include <string>
#include <string.h>
#include <malloc.h>
#include <vector>
#include "ircstack.h"
#include "ircvm.h"

using namespace std;

IRCVM* vm;

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
    vm->handleCommand(string(params), hostd);
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
	vm = new IRCVM(&con);
	con.start("irc.freenode.net", 6667, MYNICKNAME, "jeffcooper_bot", "jeffcooper_bot", "");
        con.hook_irc_command("PRIVMSG", &handlemsg);

	prog_info *r = (prog_info*)malloc(sizeof(struct prog_info));
	r->irc=&con;
	r->s=vm->S;


        pthread_t p;
	pthread_create(&p, NULL, runloop,&con);


	REPL((void*)r);
	pthread_cancel(p);
	return 0;
}
