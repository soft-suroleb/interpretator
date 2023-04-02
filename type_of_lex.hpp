#pragma once

enum type_of_lex{
    LEX_NULL, //0
    LEX_BEGIN, //1
    LEX_END, //2
    LEX_IF, //3
    LEX_ELSE, //4
    LEX_FOR, //5
    LEX_WHILE, //6
    LEX_BREAK, //7
    LEX_GOTO, //8
    LEX_READ, //9
    LEX_WRITE, //10
    LEX_INT, //11
    LEX_STRING, //12
    LEX_REAL, //13
    LEX_BOOL, //14
    LEX_TRUE, //15
    LEX_FALSE, //16
    LEX_NOT, //17
    LEX_OR, //18
    LEX_AND, //19
    LEX_LPAREN, //20
    LEX_RPAREN, //21
    LEX_MUL, //22
    LEX_DIV, //23
    LEX_PLUS, //24
    LEX_MINUS, //25
    LEX_LESS, //26
    LEX_GR, //27
    LEX_LE, //28
    LEX_GE, //29
    LEX_EQUAL, //30
    LEX_NE, //31
    LEX_ASSIGN, //32
    LEX_SEMICOLON, //33
    LEX_MOD, //34
    LEX_DOT, //35
    LEX_NUM, //36
    LEX_ID, //37
    POLIZ_LABEL, //38
    POLIZ_ADRESS, //39
    POLIZ_GO, //40
    POLIZ_FGO, //41
    LEX_PROGRAM, //42
    LEX_COMMA, //43
    LEX_LET, //44
    LEX_STR, //45
    LEX_STRUCT, //46
    LEX_DDOT, // 47
    LEX_TYPE_STRUCT, // 48
    LEX_LABEL, // 49
    LEX_CONTINUE, //50
    LEX_WRITELN
};