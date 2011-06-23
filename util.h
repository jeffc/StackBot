#ifndef _UTIL_H__
#define _UTIL_H__

#include <vector>
#include <string>
#include <string.h>
#include <malloc.h>

#define MYNICKNAME "stackbot"

using namespace std;


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

#endif
