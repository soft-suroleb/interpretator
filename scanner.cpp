#include "scanner.hpp"

tabl_ident TID;
tabl_ident const_str;

int Scanner::look(std::string buf, const std::string *list) const{
    int i = 0;
    while (list[i] != ""){
        if (list[i] == buf)
            return i;
        ++i;
    }
    return -1;
}

void Scanner::gc(){
    c = fgetc(fp);
}

Scanner::Scanner(const char *program){
    fp = fopen(program, "r");
    CS = H;
    buf.clear();
    gc();
}

Scanner::~Scanner(){
    fclose(fp);
}

const std::string Scanner::TW[] = {
    "if",
    "else",
    "for",
    "while",
    "break",
    "goto",
    "read",
    "write",
    "int",
    "string",
    "real",
    "bool",
    "true",
    "false",
    "not",
    "or",
    "and",
    "program",
    "struct",
    "continue",
    ""
};

const std::string Scanner:: TD[] = {
    "{",
    "}",
    "(",
    ")",
    "*",
    "/",
    "+",
    "-",
    "<",
    ">",
    "<=",
    ">=",
    "==",
    "!=",
    "=",
    ";",
    "%",
    ".",
    ",",
    ":",
    ""
};

type_of_lex Scanner::words[] = {
    LEX_IF,
    LEX_ELSE,
    LEX_FOR,
    LEX_WHILE,
    LEX_BREAK,
    LEX_GOTO,
    LEX_READ,
    LEX_WRITE,
    LEX_INT,
    LEX_STRING,
    LEX_REAL,
    LEX_BOOL,
    LEX_TRUE,
    LEX_FALSE,
    LEX_NOT,
    LEX_OR,
    LEX_AND,
    LEX_PROGRAM,
    LEX_STRUCT,
    LEX_CONTINUE,
    LEX_NULL
};

type_of_lex Scanner::dlms[] = {
    LEX_BEGIN,
    LEX_END,
    LEX_LPAREN,
    LEX_RPAREN,
    LEX_MUL,
    LEX_DIV,
    LEX_PLUS,
    LEX_MINUS,
    LEX_LESS,
    LEX_GR,
    LEX_LE,
    LEX_GE,
    LEX_EQUAL,
    LEX_NE,
    LEX_ASSIGN,
    LEX_SEMICOLON,
    LEX_MOD,
    LEX_DOT,
    LEX_COMMA,
    LEX_DDOT,
    LEX_NULL
};

Lex Scanner::get_lex(){
    int d, j;
    CS = H;
    do {
        switch(CS){
            case H:
                if(c == ' ' || c == '\n' || c== '\r' || c == '\t')
                    gc();
                else if(isalpha(c)){
                    buf.clear();
                    buf += c;
                    gc();
                    CS = IDENT;
                } else if(isdigit(c)){
                    d = c - '0';
                    gc();
                    CS = NUMB;
                } else if(c == '!' || c == '<' || c == '>' || c == '=' || c == '/'){
                    buf.clear();
                    buf += c;
                    gc();
                    CS = ALE;
                } else if (c == '\'') {
                    buf.clear();
                    gc();
                    CS = LET;
                } else if (c == '"'){
                    buf.clear();
                    gc();
                    CS = STR;
                } else if (c == EOF)
                    return Lex(); 
                else
                    CS = DELIM;
                break;
            case IDENT:
                if (isalpha(c) || isdigit(c)){
                    buf += c;
                    gc();
                } else {
                    if ((j = look(buf, TW)) != -1)
                        return Lex(words[j], j);
                    else{
                        j = TID.put(buf);
                        return Lex(LEX_ID, j);
                    }
                }
                break;
            case NUMB:
                if (isdigit(c)){
                    d = d * 10 + (c - '0');
                    gc();
                } else
                    return Lex(LEX_NUM, d);
                break;
            case ALE:
                if (c == '='){
                    buf += c;
                    gc();
                    j = look(buf, TD);
                    return Lex (dlms[j], j);
                } else { 
                    if (c == '/' && buf[0] == '/') {
                        buf.clear();
                        gc();
                        CS = SIN_COM;
                    } else if (c == '*' && buf[0] == '/'){
                        buf.clear();
                        gc();
                        CS = MUL_COM;
                    } else {
                        j = look(buf, TD);
                        return Lex (dlms[j], j);
                    }
                }
                break;
            case SIN_COM:
                if (c == '\n') {
                    gc();
                    CS = H;
                } else if (c == EOF)
                    CS = H;
                else
                    gc();
                break;
            case MUL_COM:
                if (c == '*') {
                    gc();
                    if (c == '/'){
                        gc();
                        CS = H;
                    } else {
                        gc();
                    }
                } else if (c == EOF)
                    throw std::invalid_argument("No closing '*/'");
                else
                    gc();
                break;
            case DELIM:
                buf.clear();
                buf += c;
                if ((j = look(buf, TD)) != -1){
                    gc();
                    return Lex(dlms[j], j);
                } else throw std::invalid_argument("No such delimeter");
                break;
            case LET:
                buf += c;
                gc();
                if (c != '\'') {
                    throw std::invalid_argument("The letter must be in '_'");
                } else {
                    gc();
                    return Lex(LEX_LET, (int)buf[0]);
                }
                break;
            case STR:
                if (c == '"') {
                    j = const_str.put(buf);
                    gc();
                    return Lex(LEX_STR, j);
                } else if (c == EOF){
                    throw std::invalid_argument("There's no closing \"");
                }
                buf += c;
                gc();
                break;
        }
    } while(true);
}
