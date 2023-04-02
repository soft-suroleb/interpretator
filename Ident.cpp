#include "type_of_lex.hpp"
#include <cstring>
#include "Ident.hpp"

Ident::Ident(){
    declare = false;
    assign = false;
}

Ident::~Ident(){}

std::string Ident::get_name(){
    return name;
}

void Ident::put_name(std::string name){
    this->name = name;
}

bool Ident::get_declare(){
    return declare;
}

void Ident::put_declare(){
    declare = true;
}

type_of_lex Ident::get_type(){
    return type;
}

void Ident::put_type(type_of_lex t){
    type = t;
}

bool Ident::get_assign(){
    return assign;
}

void Ident::put_assign(){
    assign = true;
}

double Ident::get_value(){
    return value;
}

void Ident::put_value(double v){
    value = v;
}

void Ident::put_info(std::string info){
    this->info = info;
}

std::string Ident::get_info(){
    return info;
}

std::vector<int>& Ident::label_info(){
    return labels_info;
}
