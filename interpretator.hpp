#pragma once
#include "executer.hpp"

class Interpretator 
{
	Parser pars;
	Executer E;
public:
	Interpretator (const char * program);
	void interpretation();
}; 
