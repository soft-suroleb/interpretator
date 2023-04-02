#pragma once
#include "type_of_lex.hpp"
#include "lex.hpp"
#include "tabl_ident.hpp"
#include <iostream>
#include <string>
#include <vector>

class Scanner{
    enum state {H, IDENT, NUMB, ALE, DELIM, LET, STR, SIN_COM, MUL_COM};
    static const std::string TW[];
    static type_of_lex words[];
    static const std::string TD[];
    static type_of_lex dlms[];
    state CS;
    std::FILE *fp;
    char c;
    bool end;
    std::string buf;
    int buf_top;
    int look(std::string buf, const std::string *list) const;
    void gc();
public:
    Lex get_lex();
    void is_end();
    Scanner(const char *program);
    ~Scanner();
};
