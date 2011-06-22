#ifndef _IRCSTACK_
#define _IRCSTACK_

#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<string>
#include<sstream>

enum POSSIBLESTACKENTRIES {S_STRING, S_INT, S_FLOAT};

class stackentry
{
	public:
		stackentry(std::string s);
                stackentry(int x);
                stackentry(float y);
		enum POSSIBLESTACKENTRIES mytype;
		std::string print();
                bool isNumber();
                float floatVal();
                std::string s;
                int i;
                float f;
                void* o;
		stackentry* next;
	private:

};

class stack
{
	public:
		stack();
		int getSize();
		void push(stackentry *s);
		stackentry* pop();
                stackentry* peek();
	private:
		int size;
		stackentry* top;
};

class stackopts
{
	public:
		static bool inc(stack s);
		static bool dec(stack s);
		static bool push(stack s);
		static bool pop(stack s);
};

#endif
