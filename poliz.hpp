#pragma once
#include <iostream>
#include <vector>
#include "lex.hpp"

class Poliz{
    Lex *p;
    int size;
    int free;
public:
    Poliz (int max_size);
    ~Poliz();
    void put_lex(Lex l);
    void put_lex(Lex l, int place);
    void blank();
    int get_free();
    Lex& operator[] (int index);
    void print();
};