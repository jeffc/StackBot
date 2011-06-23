#include "util.h"
#include "IRC.h"
#include <pthread.h>
#include <iostream>
#include <string>
#include <string.h>
#include <malloc.h>
#include <vector>
#include "ircstack.h"


using namespace std;

stack* thestack;


typedef struct prog_info
{
	IRC* irc;
	stack* s;
} prog_info_info;




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
