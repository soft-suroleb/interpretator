#pragma once
#include "type_of_lex.hpp"
#include <string>
#include <vector>


class Ident {
    std::string name = "";
    bool declare;
    type_of_lex type;
    bool assign;
    double value = 0;
    std::string info = "None";
    std::vector <int> labels_info;
public:
    Ident();
    ~Ident();
    std::string get_name();
    void put_name (std::string name);
    bool get_declare();
    void put_declare();
    type_of_lex get_type();
    void put_type(type_of_lex t);
    bool get_assign();
    void put_assign();
    double get_value();
    void put_value(double v);
    std::string get_info();
    void put_info(std::string name);
    std::vector<int>& label_info();
};