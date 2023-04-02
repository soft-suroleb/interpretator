#pragma once
#include "lex.hpp"
#include "type_of_lex.hpp"
#include "scanner.hpp"
#include "poliz.hpp"
#include <stack>

class Parser{
    Lex curr_lex; // текущая лексема
    type_of_lex c_type;
    int c_val;
    Scanner scan;
    std::stack <int> st_int;
    std::stack <type_of_lex> st_lex;
    void P(); // процедуры РС-метода
    void STRUCT();
    void D1();
    void IDENT_STRUCT();
    void ID_STRUCT(std::string name);
    void D();
    void ID();
    void NUM();
    void B();
    void S();
    void E();
    void E1();
    void T();
    void F();
    void eq_type();
    void check_op();
    void check_single();
    void check_id_in_read();
    void eq_bool();
    void gl(); // получить очередную лексему
public:
    Poliz prog; // внутреннее представление программы
    Parser(const char *program) : scan (program), prog(1000) {}
    void analyze(); // анализатор с действиями
};