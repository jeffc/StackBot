#include "ircstack.h"

stackentry::stackentry(std::string st)
{
	size_t found_int0;
	size_t found_double0;
	found_int0 = st.find_first_not_of("0");
	found_double0 = st.find_first_not_of("0.");
	if(found_int0 == std::string::npos)
	{
		mytype=S_INT;
		i=0;
	}
	else if(found_double0 == std::string::npos)
	{
		mytype=S_FLOAT;
		f=0.0;
	}
	else if((i=atoi(st.c_str())) != 0)
	{
		mytype=S_INT;
	}
	else if(f=atof(st.c_str()))
	{
		mytype=S_FLOAT;
        }
	else
	{
		mytype=S_STRING;
		s=std::string(st);
	}
}

stackentry::stackentry(stackentry* old) //copy constructor
{
	mytype = old->mytype;
	f = old->f;
	i = old->i;
	s = old->s;
	o = old->o;
}

stackentry::stackentry(int x)
{
    mytype=S_INT;
    i=x;
}

stackentry::stackentry(float y)
{
    mytype=S_FLOAT;
    f=y;
}
std::string stackentry::print()
{
        std::stringstream ss;
        std::cout << mytype << std::endl;
	switch(mytype)
	{
		case S_STRING:
                        ss << s;
			return ss.str();
			break;
		case S_INT:
                        ss << i;
			return ss.str();
			break;
		case S_FLOAT:
                        ss << f;
			return ss.str();
			break;
	}
}

bool stackentry::isNumber()
{
    return (mytype==S_INT || mytype==S_FLOAT);
}

float stackentry::floatVal()
{
    if(mytype == S_FLOAT)
        return f;
    if(mytype == S_INT)
        return (float)i;
    else
        return 0.0f;
}
