#ifndef _UTIL_H__
#define _UTIL_H__

#include <vector>
#include <string>
#include <string.h>
#include <malloc.h>

#define MYNICKNAME "stackbot"

using namespace std;


vector<string> stringsplit(string s, char* del);
vector<string> stringsplit_n(string s, char* del, int lim);

#endif
