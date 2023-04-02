#include "parser.hpp"
#include <map>
#include <vector>

extern tabl_ident TID;
std::map<std::string, std::vector<Ident>> map_struct;
// std::vector<Ident*> labels;
std::stack<int> break_cont;


void Parser::gl(){
    curr_lex = scan.get_lex();
    c_type = curr_lex.get_type();
    c_val = curr_lex.get_value();
}

void Parser::analyze(){
    gl();
    P();
    for (int i = 0; i < TID.getTop(); i++){
        if (TID[i].get_type() == LEX_LABEL && !TID[i].get_declare())
            throw std::invalid_argument("No '" + TID[i].get_name() + "' for goto");
    }
    std::cout << "=========== POLIZ ===========" << std::endl;
    prog.print();
}

void Parser::eq_type(){
    type_of_lex type2 = st_lex.top();
    st_lex.pop();
    type_of_lex type1 = st_lex.top();
    st_lex.pop();
    if (type1 == LEX_INT && type2 == LEX_REAL) return;
    if (type1 == LEX_REAL && type2 == LEX_INT) return;
    if (type1 != type2) throw std::invalid_argument("wrong types are in =");
}

void Parser::check_op(){
    type_of_lex type2 = st_lex.top();
    st_lex.pop();
    type_of_lex op = st_lex.top();
    st_lex.pop();
    type_of_lex type1 = st_lex.top();
    st_lex.pop();
    type_of_lex result;
    if (op == LEX_AND || op == LEX_OR){
        if (type1 == type2 && type1 == LEX_BOOL){
            result = LEX_BOOL;
        } else throw std::invalid_argument("Not bool operand in bool operation");
    } else if (op == LEX_MOD) {
        if (type1 == type2 && type1 == LEX_INT){
            result = LEX_INT;
        } else throw std::invalid_argument("Not int opearand in '%' opearation");
    } else if (op == LEX_PLUS || op == LEX_MINUS || op == LEX_MUL || op == LEX_DIV || op == LEX_ASSIGN){
        if (type1 == LEX_STRING || type2 == LEX_STRING) {
            if (type1 != LEX_STRING || type2 != LEX_STRING || op != LEX_PLUS)
                throw std::invalid_argument("Wrong str operands");
            else result = LEX_STRING;
        } else if ((type1 == LEX_INT || type1 == LEX_REAL) && (type2 == LEX_INT || type2 == LEX_REAL)){
            if (type1 == type2 && type1 == LEX_INT)
                result = LEX_INT;
            else
                result = LEX_REAL;
        } else{
            throw std::invalid_argument("Wrong type of operands");
        }
    } else if (op == LEX_LESS || op == LEX_GR || op == LEX_LE ||
               op == LEX_GE || op == LEX_EQUAL || op == LEX_NE){
        if (type1 == LEX_STRING || type2 == LEX_STRING) {
            if (type1 != LEX_STRING || type2 != LEX_STRING)
                throw std::invalid_argument("Wrong str operands");
            if (op != LEX_LESS && op != LEX_GR && op != LEX_EQUAL && op != LEX_NE){
                throw std::invalid_argument("Wrong op between string operands");
            } else result = LEX_BOOL;
        } else if ((type1 == LEX_INT || type1 == LEX_REAL) && (type2 == LEX_INT || type2 == LEX_REAL)){
            result = LEX_BOOL;
        } else
            throw std::invalid_argument("Wrong type of operands");
    }
    st_lex.push(result);
    prog.put_lex(Lex(op, op));
    if (op == LEX_ASSIGN){
        int i = prog.get_free() - 3;
        prog.put_lex(Lex(POLIZ_ADRESS, prog[i].get_value(), prog[i].get_supp()), i);
    }
}

void Parser::check_single(){
    type_of_lex type = st_lex.top();
    st_lex.pop();
    type_of_lex op = st_lex.top();
    st_lex.pop();
    if (op == LEX_PLUS || op == LEX_MINUS){
        if (type == LEX_INT || type == LEX_BOOL)
            st_lex.push(type);
        else throw std::invalid_argument("Wrong type of operand");
    } else if (op == LEX_NOT){
        if (type == LEX_BOOL) st_lex.push(type);
        else throw std::invalid_argument("Wrong type of operand");
    } else throw std::runtime_error("Wrong operation in stack");
    prog.put_lex(Lex(LEX_NOT, LEX_NOT));
}

void Parser::check_id_in_read(){
    if (!TID[c_val].get_declare())
        throw std::invalid_argument("The variable is not declared");
}

void Parser::eq_bool(){
    type_of_lex type = st_lex.top();
    st_lex.pop();
    if (type != LEX_BOOL)
        throw std::invalid_argument("The expression is not boolean");
}

void Parser::P(){
    if (c_type == LEX_PROGRAM)
        gl();
    else
        throw std::invalid_argument("No program");
    if(c_type == LEX_BEGIN){
        gl();
        STRUCT();
        D1();
        B();
    } else
        throw std::invalid_argument("No '{'");
    if(c_type != LEX_END)
        throw std::invalid_argument("No '}'");
    else {
        gl();
        if(c_type != LEX_NULL) {
            throw std::invalid_argument("Some errors");
        }
    }
}

void Parser::STRUCT(){
    while (c_type == LEX_STRUCT){
        gl();
        if (c_type != LEX_ID) throw std::invalid_argument("Not an ID");
        if (TID[c_val].get_declare()) throw std::invalid_argument("Redaclaration of struct");
        TID[c_val].put_type(LEX_STRUCT);
        TID[c_val].put_declare();
        std::string struct_tmp = TID[c_val].get_name();
        gl();
        if (c_type != LEX_BEGIN) throw std::invalid_argument("No '{'");
        gl();
        D1();
        if (c_type != LEX_END) throw std::invalid_argument("No '}");
        gl();
        if(c_type != LEX_SEMICOLON)
            throw std::invalid_argument("No ';'");
        gl();
        map_struct[struct_tmp] = std::vector<Ident>({});
        while (TID[TID.getTop() - 1].get_name() != struct_tmp){
            Ident tmp = TID.pop();
            map_struct[struct_tmp].push_back(tmp);
        }
    }
}

void Parser::D1(){
    while ((c_type == LEX_INT) ||
           (c_type == LEX_BOOL) ||
           (c_type == LEX_STRING) ||
           (c_type == LEX_REAL) ||
           (c_type == LEX_ID)){
        if (c_type == LEX_ID) {
            if (TID[c_val].get_type() == LEX_STRUCT)
                IDENT_STRUCT();
            else break;
        } else D(); 
        if(c_type != LEX_SEMICOLON)
            throw std::invalid_argument("No ';'");
        gl();
    }
}

void Parser::IDENT_STRUCT(){
    bool check = false;
    for (auto &i: map_struct){
        if (i.first == TID[c_val].get_name()) {
            check = true;
            break;
        }
    }
    if (!check) throw std::invalid_argument("No such struct");
    std::string struct_tmp = TID[c_val].get_name();
    gl();
    ID_STRUCT(struct_tmp);
    while (c_type == LEX_COMMA){
        gl();
        ID_STRUCT(struct_tmp);
    }
}

void Parser::ID_STRUCT(std::string struct_name){
    if (c_type != LEX_ID) throw std::invalid_argument("Not an ID");
    std::string name = TID[TID.getTop() - 1].get_name();
    TID[TID.getTop() - 1].put_type(LEX_TYPE_STRUCT);
    TID[TID.getTop() - 1].put_declare();
    TID[TID.getTop() - 1].put_info(struct_name);
    for (auto &i: map_struct[struct_name]){
        TID.put(name + '_' + i.get_name());
        TID[TID.getTop() - 1].put_declare();
        TID[TID.getTop() - 1].put_type(i.get_type());
    }
    gl();
}

void Parser::D()
{
    type_of_lex tmp = c_type;
    st_lex.push(tmp);
    gl();
    if (c_type != LEX_ID) throw std::invalid_argument("Not ID");
    ID();
    while (c_type == LEX_COMMA){
        gl();
        ID();
    }
}

void Parser::ID(){
    if(c_type == LEX_ID){
        if (TID[c_val].get_declare()) throw std::invalid_argument("Redeclaration");
        TID[c_val].put_declare();
        TID[c_val].put_type(st_lex.top());
    } else if (c_type == LEX_COMMA || c_type == LEX_SEMICOLON) return;
    else throw std::invalid_argument("Not ID");
    int tmp_c_val = c_val;
    st_int.push(c_val);
    gl();
    if (c_type == LEX_ASSIGN){
        st_lex.push(TID[tmp_c_val].get_type());
        prog.put_lex(Lex(POLIZ_ADRESS, tmp_c_val));
        gl();
        NUM();
        prog.put_lex(Lex(LEX_ASSIGN, LEX_ASSIGN));
        prog.put_lex(Lex(LEX_SEMICOLON, LEX_SEMICOLON));
    }
}

void Parser::NUM(){
    if(c_type == LEX_STR){
        st_lex.push(LEX_STRING);
        eq_type();
        prog.put_lex(curr_lex);
        gl();
    } else if (c_type == LEX_TRUE){
        st_lex.push(LEX_BOOL);
        eq_type();
        prog.put_lex(Lex(LEX_TRUE, LEX_TRUE));
        gl();
    } else if (c_type == LEX_FALSE){
        st_lex.push(LEX_BOOL);
        eq_type();
        prog.put_lex(Lex(LEX_FALSE, LEX_FALSE));
        gl();
    } else if (c_type == LEX_NUM){
        double tmp1 = c_val;
        gl();
        if (c_type != LEX_DOT){
            st_lex.push(LEX_INT);
        } else {
            gl();
            st_lex.push(LEX_REAL);
            eq_type();
            if (c_type == LEX_NUM) {
                double tmp2 = c_val;
                while (tmp2 > 1)
                    tmp2 /= 10;
                prog.put_lex(Lex(LEX_NUM, 0, tmp1 + tmp2));
                gl();
            } else {
                prog.put_lex(Lex(LEX_NUM, 0, tmp1));
            } 
            return;
        }
        eq_type();
        prog.put_lex(Lex(LEX_NUM, tmp1));
    } else throw curr_lex;
}

void Parser::B() {
    while(c_type != LEX_END && c_type != LEX_NULL){
        S();
        //prog.put_lex(Lex(LEX_SEMICOLON, LEX_SEMICOLON));
    }
}

void Parser::S(){
    int pl0, pl1, pl2, pl3;
    if (c_type == LEX_ID){
        int tmp_val = c_val;
        gl();
        if (c_type == LEX_ASSIGN){
            if(!TID[tmp_val].get_declare()) throw std::invalid_argument("Not declared");
            if (TID[tmp_val].get_type() == LEX_TYPE_STRUCT) {
                gl();
                if (TID[c_val].get_info() != TID[tmp_val].get_info())
                    throw std::invalid_argument("Different struct types");
                std::string name1 = TID[tmp_val].get_name();
                std::string name2 = TID[c_val].get_name();
                for (auto& i: map_struct[TID[c_val].get_info()]){
                    int field1 = TID.put(name1 + "_" + i.get_name());
                    int field2 = TID.put(name2 + "_" + i.get_name());
                    prog.put_lex(Lex(POLIZ_ADRESS, field1));
                    prog.put_lex(Lex(LEX_ID, field2));
                    prog.put_lex(Lex(LEX_ASSIGN, LEX_ASSIGN));
                    prog.put_lex(Lex(LEX_SEMICOLON, LEX_SEMICOLON));
                }
                gl();
            } else {
                prog.put_lex(Lex(POLIZ_ADRESS, tmp_val));
                st_lex.push(TID[tmp_val].get_type());
                gl();
                E();
                eq_type();
                prog.put_lex(Lex(LEX_ASSIGN, LEX_ASSIGN));
                prog.put_lex(Lex(LEX_SEMICOLON, LEX_SEMICOLON));
            }
        } else if (c_type == LEX_DOT){
            if(!TID[tmp_val].get_declare()) throw std::invalid_argument("Not declared");
            gl();
            if (c_type != LEX_ID) throw std::invalid_argument("No field of a struct");
            c_val = TID.put(TID[tmp_val].get_name() + '_' + TID.pop().get_name());
            c_type = LEX_ID;
            S();
            return;
        } else if (c_type == LEX_DDOT){
            if (TID[tmp_val].get_declare()) throw std::invalid_argument("Redaclaration of label");
            TID[tmp_val].put_declare();
            TID[tmp_val].put_type(LEX_LABEL);
            TID[tmp_val].put_info("label");
            TID[tmp_val].put_value(prog.get_free());
            for (auto i: TID[tmp_val].label_info()){
                prog.put_lex(Lex(POLIZ_LABEL, TID[tmp_val].get_value()), i);
            }
            gl();
            return;
        } else throw std::invalid_argument("Syntax Error");
        if (c_type != LEX_SEMICOLON) throw std::invalid_argument("No ;");
            gl();
    } else if (c_type == LEX_WRITE){
        gl();
        if(c_type == LEX_LPAREN){
            gl();
            E();
            while(c_type == LEX_COMMA){
                prog.put_lex(Lex(LEX_WRITE, LEX_WRITE));
                gl();
                E();
            }
            if (c_type == LEX_RPAREN){
                prog.put_lex(Lex(LEX_WRITE, LEX_WRITE));
                prog.put_lex(Lex(LEX_WRITELN, LEX_WRITELN));
                gl();
            } else throw std::invalid_argument("No closing ')");
        } else throw std::invalid_argument("No opening '('");
        if (c_type != LEX_SEMICOLON) throw std::invalid_argument("No ;");
            gl();
    } else if (c_type == LEX_READ){
        gl();
        if(c_type == LEX_LPAREN){
            gl();
            if(c_type == LEX_ID){
                check_id_in_read();
                prog.put_lex(Lex(POLIZ_ADRESS, c_val));
                gl();
            } else throw std::invalid_argument("Not an ID in read");
            while(c_type == LEX_COMMA){
                prog.put_lex(Lex(LEX_READ, LEX_READ));
                gl();
                if(c_type == LEX_ID){
                    check_id_in_read();
                    prog.put_lex(Lex(POLIZ_ADRESS, c_val));
                    gl();
                } else throw std::invalid_argument("Not an ID in read");
            }
            if(c_type == LEX_RPAREN){
                gl();
                prog.put_lex(Lex(LEX_READ, LEX_READ));
            } else throw std::invalid_argument("No closing ')'");
        } else throw std::invalid_argument("No opening '('");
        if (c_type != LEX_SEMICOLON) throw std::invalid_argument("No ;");
            gl();
    } else if (c_type == LEX_WHILE){
        break_cont.push(-1);
        break_cont.push(-1);
        pl0 = prog.get_free();
        gl();
        E();
        eq_bool();
        pl1 = prog.get_free();
        prog.blank();
        prog.put_lex (Lex(POLIZ_FGO, POLIZ_FGO));
        if (c_type == LEX_BEGIN){
            gl();
            B();
            prog.put_lex (Lex(POLIZ_LABEL, pl0));
            prog.put_lex (Lex(POLIZ_GO, POLIZ_GO));
            prog.put_lex (Lex(POLIZ_LABEL, prog.get_free()),pl1);
            if (break_cont.top() != -1){
                prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), break_cont.top());
            }
            break_cont.pop();
            if (break_cont.top() != -1){
                prog.put_lex(Lex(POLIZ_LABEL, pl0), break_cont.top());
            }
            break_cont.pop();
        } else throw std::invalid_argument("No opening '{'");
        if (c_type != LEX_END) throw std::invalid_argument("No closing '}'");
        gl();
    } else if (c_type == LEX_FOR){
        break_cont.push(-1);
        break_cont.push(-1);
        gl();
        if (c_type != LEX_LPAREN) throw std::invalid_argument("No opening '('");
        gl();
        if (c_type != LEX_SEMICOLON){
            S();
        } else {
            gl();
        }
        pl0 = prog.get_free();
        E();
        eq_bool();
        pl1 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_FGO, POLIZ_FGO));
        pl2 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_GO, POLIZ_GO));
        pl3 = prog.get_free();
        if (c_type != LEX_SEMICOLON) throw std::invalid_argument("No ';'");
        gl();
        if (c_type != LEX_RPAREN){
            S();
            if (c_type != LEX_RPAREN) throw std::invalid_argument("No closing ')");
        }
        prog.put_lex(Lex(POLIZ_LABEL, pl0));
        prog.put_lex(Lex(POLIZ_GO, POLIZ_GO));
        prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl2);
        gl();
        if (c_type != LEX_BEGIN) throw std::invalid_argument("No opening '{'");
        gl();
        B();
        prog.put_lex(Lex(POLIZ_LABEL, pl3));
        prog.put_lex(Lex(POLIZ_GO, POLIZ_GO));
        prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl1);
        if (break_cont.top() != -1){
            prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), break_cont.top());
        }
        break_cont.pop();
        if (break_cont.top() != -1){
            prog.put_lex(Lex(POLIZ_LABEL, pl3), break_cont.top());
        }
        break_cont.pop();
        if (c_type != LEX_END) throw std::invalid_argument("No closing '}'");
        gl();
    } else if (c_type == LEX_IF){
        gl();
        E();
        eq_bool();
        pl2 = prog.get_free();
        prog.blank();
        prog.put_lex((Lex(POLIZ_FGO, POLIZ_FGO)));
        if(c_type == LEX_BEGIN){
            gl();
            B();
            pl3 = prog.get_free();
            prog.blank();
            prog.put_lex(Lex(POLIZ_GO, POLIZ_GO));
            prog.put_lex (Lex(POLIZ_LABEL, prog.get_free()), pl2);
        } else throw std::invalid_argument("No opening '{'");
        if (c_type != LEX_END)
            throw std::invalid_argument("No closing '}'");
        gl();
        if(c_type == LEX_ELSE)
        {
            gl();
            if(c_type == LEX_BEGIN){
                gl();
                B();
                prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl3);
            } else throw std::invalid_argument("No opening '{");
            if (c_type != LEX_END)
                throw std::invalid_argument("No closing '}'");
            gl();
        }
        else {
            prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl3);
        }
    } else if (c_type == LEX_BREAK){
        break_cont.pop();
        break_cont.push(prog.get_free());
        prog.blank();
        prog.put_lex(Lex(POLIZ_GO, POLIZ_GO));
        gl();
        if (c_type != LEX_SEMICOLON) throw std::invalid_argument("No ;");
            gl();
    } else if (c_type == LEX_CONTINUE){
        int tmp = break_cont.top();
        break_cont.pop();
        break_cont.pop();
        break_cont.push(prog.get_free());
        break_cont.push(tmp);
        prog.blank();
        prog.put_lex(Lex(POLIZ_GO, POLIZ_GO));
        gl();
        if (c_type != LEX_SEMICOLON) throw std::invalid_argument("No ;");
            gl();
    } else if (c_type == LEX_GOTO){
        gl();
        TID[c_val].label_info().push_back(prog.get_free());
        if (!TID[c_val].get_declare()){
            TID[c_val].put_type(LEX_LABEL);
            prog.blank();
        } else
            prog.put_lex(Lex(POLIZ_LABEL, TID[c_val].get_value()));
        prog.put_lex(Lex(POLIZ_GO, POLIZ_GO));
        gl();
        if (c_type != LEX_SEMICOLON) throw std::invalid_argument("No ;");
            gl();
    } else throw std::invalid_argument("Syntax error!");
}

void Parser::E(){
    E1();
    if(c_type == LEX_EQUAL || c_type == LEX_LESS || c_type == LEX_GR || 
        c_type == LEX_LE || c_type == LEX_GE || c_type == LEX_NE){
        st_lex.push(c_type);
        gl();
        E1();
        check_op();
    }
}

void Parser::E1(){
    //type_of_lex tmp;
    T();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR || c_type == LEX_ASSIGN){
        st_lex.push(c_type);
        gl();
        T();
        check_op();
    }
}

void Parser::T(){
    F();
    while (c_type == LEX_MUL || c_type == LEX_DIV || c_type == LEX_AND || c_type == LEX_MOD){
        st_lex.push(c_type);
        gl();
        F();
        check_op();
    }
}


void Parser::F(){
    if (c_type == LEX_ID){
        Lex lex_tmp = curr_lex;
        gl();
        if (c_type != LEX_DOT){
            if(!TID[lex_tmp.get_value()].get_declare()) throw std::invalid_argument("Not declared");
            st_lex.push(TID[lex_tmp.get_value()].get_type());
            prog.put_lex(lex_tmp);
        } else {
            gl();
            if (c_type != LEX_ID) throw std::invalid_argument("no filed of a struct");
            c_val = TID.put(TID[lex_tmp.get_value()].get_name() + '_' + TID.pop().get_name());
            c_type = LEX_ID;
            st_lex.push(TID[c_val].get_type());
            prog.put_lex(Lex(LEX_ID, c_val));
            gl();
        }
    } else if (c_type == LEX_STR){
        st_lex.push(LEX_STRING);
        prog.put_lex(curr_lex);
        gl();
    } else if (c_type == LEX_NUM){
        Lex lex_tmp = curr_lex;
        gl();
        if (c_type != LEX_DOT) {
            st_lex.push(LEX_INT);
            prog.put_lex(lex_tmp);
        } else {
            double tmp = lex_tmp.get_value();
            gl();
            if (c_type == LEX_NUM){
                double tmp2 = c_val;
                while (tmp2 >= 1) tmp2 /= 10;
                st_lex.push(LEX_REAL);
                prog.put_lex(Lex(LEX_NUM, 0, tmp + tmp2));
                gl();
            } else {
                st_lex.push(LEX_REAL);
                prog.put_lex(Lex(LEX_NUM, (int)tmp));
            }
        }
    } else if (c_type == LEX_TRUE){
        st_lex.push(LEX_BOOL);
        prog.put_lex(Lex(LEX_TRUE, LEX_TRUE));
        gl();
    } else if (c_type == LEX_FALSE){
        st_lex.push(LEX_BOOL);
        prog.put_lex(Lex(LEX_FALSE, LEX_FALSE));
        gl(); 
    } else if (c_type == LEX_NOT){
        st_lex.push(LEX_NOT);
        gl();
        F();
        check_single();
    } else if (c_type == LEX_PLUS){
        st_lex.push(LEX_PLUS);
        gl();
        F();
        check_single();
    } else if (c_type == LEX_MINUS){
        st_lex.push(LEX_MINUS);
        gl();
        F();
        check_single();
    } else if (c_type == LEX_LPAREN){
        gl();
        E();
        if (c_type == LEX_RPAREN)
            gl();
        else throw std::invalid_argument("No closing ')'");
    } else throw std::invalid_argument("Wrong type of operand!");
}
