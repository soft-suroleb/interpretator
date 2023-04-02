#pragma once
#include "type_of_lex.hpp"
#include <iostream>

class Lex{
    type_of_lex t_lex;
    int v_lex;
    double supp;
public:
    Lex();
    Lex(type_of_lex t, int v);
    Lex(type_of_lex t, int v, double supp);
    type_of_lex get_type();
    double get_supp();
    int get_value();
    friend std::ostream& operator<<(std::ostream &stream, Lex l);
};
