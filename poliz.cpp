#include "poliz.hpp"

Poliz::Poliz(int max_size){
    p = new Lex [size = max_size];
    free = 0;
};

Poliz::~Poliz() {delete []p;};

void Poliz::put_lex(Lex l) {p[free]=l; ++free;};

void Poliz::put_lex(Lex l, int place) {p[place]=l;};

void Poliz::blank() {++free;};

int Poliz::get_free() {return free;};

Lex& Poliz::operator[] (int index){
if (index > size)
    throw "POLIZ:out of array";
else if (index > free)
    throw "POLIZ:indefinite element of array";
else
    return p[index];
};

void Poliz::print(){
    for (int i = 0; i < free; ++i)
        std::cout << i << ": " << p[i] << std::endl;
};
