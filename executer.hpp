#pragma once
#include "parser.hpp"

extern tabl_ident TID;
extern tabl_ident const_str;

class Executer
{
	Lex pc_el;
public:
	void execute(Poliz &prog);
};