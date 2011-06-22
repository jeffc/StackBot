#include "ircstack.h"

stack::stack()
{
	size=0;
}

int stack::getSize()
{
	return size;
}

void stack::push(stackentry* s)
{
	s->next=top;
	top=s;
	size++;
}

stackentry* stack::pop()
{
	if(top == NULL)
		return NULL;
	
	stackentry* ret = top;
	top = top->next;
	size--;
	return ret;
}


stackentry* stack::peek()
{
	return top;
}
