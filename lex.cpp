#include "type_of_lex.hpp"
#include "lex.hpp"
#include <iostream>

Lex::Lex(){
    t_lex = LEX_NULL;
    v_lex = 0;

}

Lex::Lex(type_of_lex t, int v){
    t_lex = t; v_lex = v;
    supp = 0;
}

Lex::Lex(type_of_lex t, int v, double supp){
    t_lex = t;
    v_lex = v;
    this->supp = supp;
}

type_of_lex Lex::get_type() {return t_lex;}

int Lex::get_value() {return v_lex;}

double Lex::get_supp(){return supp;}

std::ostream& operator<<(std::ostream &stream,Lex l){
    stream << '(';
    switch (l.get_type()) {
        case 0:
            stream << "LEX_NULL";
            break;
        case 1:
            stream << "LEX_BEGIN";
            break;
        case 2:
            stream << "LEX_END";
            break;
        case 3:
            stream << "LEX_IF";
            break;
        case 4:
            stream << "LEX_ELSE";
            break;
        case 5:
            stream << "LEX_FOR";
            break;
        case 6:
            stream << "LEX_WHILE";
            break;
        case 7:
            stream << "LEX_BREAK";
            break;
        case 8:
            stream << "LEX_GOTO";
            break;
        case 9:
            stream << "LEX_READ";
            break;
        case 10:
            stream << "LEX_WRITE";
            break;
        case 11:
            stream << "LEX_INT";
            break;
        case 12:
            stream << "LEX_STRING";
            break;
        case 13:
            stream << "LEX_REAL";
            break;
        case 14:
            stream << "LEX_BOOL";
            break;
        case 15:
            stream << "LEX_TRUE";
            break;
        case 16:
            stream << "LEX_FALSE";
            break;
        case 17:
            stream << "LEX_NOT";
            break;
        case 18:
            stream << "LEX_OR";
            break;
        case 19:
            stream << "LEX_AND";
            break;
        case 20:
            stream << "LEX_LPAREN";
            break;
        case 21:
            stream << "LEX_RPAREN";
            break;
        case 22:
            stream << "LEX_MUL";
            break;
        case 23:
            stream << "LEX_DIV";
            break;
        case 24:
            stream << "LEX_PLUS";
            break;
        case 25:
            stream << "LEX_MINUS";
            break;
        case 26:
            stream << "LEX_LESS";
            break;
        case 27:
            stream << "LEX_GR";
            break;
        case 28:
            stream << "LEX_LE";
            break;
        case 29:
            stream << "LEX_GE";
            break;
        case 30:
            stream << "LEX_EQUAL";
            break;
        case 31:
            stream << "LEX_NE";
            break;
        case 32:
            stream << "LEX_ASSIGN";
            break;
        case 33:
            stream << "LEX_SEMICOLON";
            break;
        case 34:
            stream << "LEX_MOD";
            break;
        case 35:
            stream << "LEX_DOT";
            break;
        case 36:
            stream << "LEX_NUM";
            break;
        case 37:
            stream << "LEX_ID";
            break;
        case 38:
            stream << "POLIZ_LABEL";
            break;
        case 39:
            stream << "POLIZ_ADDRESS";
            break;
        case 40:
            stream << "POLIZ_GO";
            break;
        case 41:
            stream << "POLIZ_FGO";
            break;
        case 42:
            stream << "LEX_PROGRAM";
            break;
        case 43:
            stream << "LEX_COMMA";
            break;
        case 44:
            stream << "LEX_LET";
            break;
        case 45:
            stream << "LEX_STR";
            break;
        case 46:
            stream << "LEX_STRUCT";
            break;
        case 47:
            stream << "LEX_DDOT";
            break;
        case 48:
            stream << "LEX_TYPE_STRUCT";
            break;
        case 49:
            stream << "LEX_LABEL";
            break;
        case 50:
            stream << "LEX_CONTINUE";
            break;
        case 51:
            stream << "LEX_WRITELN";
            break;
        default:
            stream << "NAN";
            break;
        }
    stream  << ", " << l.v_lex;
    if (l.supp != 0){
        stream << ", " << l.supp << ")";
    } else {
        stream << ")";
    }
    return stream;
}