#include "executer.hpp"
#include <stack>
#include <unistd.h>

template <class T>
void print(std::stack<T> st){
    while (!st.empty()){
        T supp = st.top();
        st.pop();
        std::cout << supp << " ";
    }
    std::cout << std::endl;
}

void Executer::execute (Poliz& prog){
    std::stack <double> args;
    std::stack <std::string> args_s;
    int index = 0, size = prog.get_free();
    double i, j;
    std::string str1, str2;
    while (index < size){
        pc_el = prog[index];
        switch (pc_el.get_type()){
        case LEX_TRUE:
        case LEX_FALSE:
        case LEX_NUM:
        case POLIZ_ADRESS:
        case POLIZ_LABEL:
            if (pc_el.get_value() == 0 && pc_el.get_supp() != 0){
                args.push(pc_el.get_supp());
            } else {
                args.push(pc_el.get_value());
                if (pc_el.get_value() < TID.getTop() && TID[pc_el.get_value()].get_type() == LEX_STRING && pc_el.get_type() != LEX_NUM){
                    args_s.push(TID[pc_el.get_value()].get_info());
                }
            }
            break;
        case LEX_ID:
            i = pc_el.get_value();
            if (TID[i].get_assign()){
                if (TID[i].get_type() == LEX_STRING){
                    args_s.push(TID[i].get_info());
                    args.push(i);
                } else
                    args.push (TID[i].get_value());
                break;
            } else
                throw std::invalid_argument("Indefinite identifier");
        case LEX_NOT:
            i = args.top();
            args.pop();
            args.push(!i);
            break;
        case LEX_OR:
            i = args.top();
            args.pop();
            j = args.top();
            args.pop();
            args.push(j || i);
            break;
        case LEX_AND:
            i = args.top();
            args.pop();
            j = args.top();
            args.pop();
            args.push(j && i);
            break;
        case POLIZ_GO:
            i = args.top();
            args.pop();
            index = i - 1;
            break;
        case POLIZ_FGO:
            i = args.top();
            args.pop();
            j = args.top();
            args.pop();
            if (!j) index = i - 1;
            break;
        case LEX_WRITE:
            if (!args_s.empty() &&
                    ((args.top() < const_str.getTop() && const_str[args.top()].get_name() == args_s.top()) ||
                    (args.top() < TID.getTop() && TID[args.top()].get_info() == args_s.top()))){
                str1 = args_s.top();
                args_s.pop();
                args.pop();
                std::cout << str1;
            } else {
                i = args.top();
                args.pop();
                std::cout << i;
            }
            break;
        case LEX_WRITELN:
            std::cout << std::endl;
            break;
        case LEX_READ:
            int k;
            i = args.top();
            args.pop();
            if (TID[i].get_type() == LEX_INT){
                std::cout << "Input int value for ";
                std::cout << TID[i].get_name () << " ";
                std::cin >> k;
                TID[i].put_value(k);
                TID[i].put_assign();
            } else if (TID[i].get_type() == LEX_BOOL){
                std::cout << "Input boolean value";
                std::cout << "(true or false) for ";
                std::cout << TID[i].get_name() << " ";
                std::cin >> str1;
                if (str1 == "true")
                    k = 1;
                else if (str1 == "false")
                    k = 0;
                else{
                    std::cout << "Error in input:true/false";
                    std::cout << std::endl;
                }
                TID[i].put_value(k);
                TID[i].put_assign();
            } else if (TID[i].get_type() == LEX_REAL){
                double val;
                std::cout << "Input double value for ";
                std::cout << TID[i].get_name () << " ";
                std::cin >> val;
                TID[i].put_value(val);
                TID[i].put_assign();
            } else if (TID[i].get_type() == LEX_STRING){
                std::cout << "Input string value for ";
                std::cout << TID[i].get_name () << " ";
                std::cin >> str1;
                args_s.pop();
                TID[i].put_info(str1);
                TID[i].put_assign();
            }
            break;
        case LEX_PLUS:
            i = args.top();
            args.pop();
            j = args.top();
            args.pop();
            if(!args_s.empty()){
                str1 = args_s.top();
                args_s.pop();
                str2 = args_s.top();
                args_s.pop();
                args_s.push(str2 + str1);
                args.push(const_str.put(str2+str1));
            } else {
                args.push (i + j);
            }
            break;
        case LEX_MUL:
            i = args.top();
            args.pop();
            j = args.top();
            args.pop();
            args.push(i * j);
            break;
        case LEX_MINUS:
            i = args.top();
            args.pop();
            j = args.top();
            args.pop();
            args.push(j - i);
            break;
        case LEX_DIV:
            i = args.top();
            args.pop();
            j = args.top();
            args.pop();
            if (i){
                args.push(j / i);
                break;
            } else
                throw std::invalid_argument("Divide by zero");

        case LEX_MOD:
            i = args.top();
            args.pop();
            j = args.top();
            args.pop();
            if (i) {
                args.push((int)j % (int)i);
                break;
            } else
                throw std::invalid_argument("Mod by zero");
            break;
        case LEX_EQUAL:
            i = args.top();
            args.pop();
            j = args.top();
            args.pop();
            if(!args_s.empty()){
                str1 = args_s.top();
                args_s.pop();
                str2 = args_s.top();
                args_s.pop();
                args.push(str1 == str2);
            } else {
                args.push (i == j);
            }
            break;
        case LEX_LESS:
            i = args.top();
            args.pop();
            j = args.top();
            args.pop();
            if(!args_s.empty()){
                str1 = args_s.top();
                args_s.pop();
                str2 = args_s.top();
                args_s.pop();
                args.push(str2 < str1);
            } else {
                args.push (j < i);
            }
            break;
        case LEX_GR:
            i = args.top();
            args.pop();
            j = args.top();
            args.pop();
            if(!args_s.empty())
            {
                str1 = args_s.top();
                args_s.pop();
                str2 = args_s.top();
                args_s.pop();
                args.push(str2 > str1);
            } else {
                args.push ( j > i);
            }
            break;
        case LEX_LE:
            i = args.top();
            args.pop();
            j = args.top();
            args.pop();
            args.push (j <= i);
            break;
        case LEX_GE:
            i = args.top();
            args.pop();
            j = args.top();
            args.pop();
            args.push (j >= i);
            break;
        case LEX_NE:
            i = args.top();
            args.pop();
            j = args.top();
            args.pop();
            if(!args_s.empty()){
                str1 = args_s.top();
                args_s.pop();
                str2 = args_s.top();
                args_s.pop();
                args.push(str1 != str2);
            } else {
                args.push ( i != j);
            }
            break;
        case LEX_STR:
            i = pc_el.get_value();
            str1 = const_str[i].get_name();
            args_s.push(str1);
            args.push(i);
            break;
        case LEX_ASSIGN:
            i = args.top();
            args.pop();
            j = args.top();
            args.pop();
            if(!args_s.empty()) {
                str1 = args_s.top();
                args_s.pop();
                TID[j].put_assign();
                TID[j].put_info(str1);
                args.push(j);
                args_s.pop();
                args_s.push(str1);
            } else {
                // std::cout << i << std::endl;
                TID[j].put_value(i);
                TID[j].put_assign();
                args.push(i);
            }
            break;
        case LEX_SEMICOLON:
            if (!args_s.empty())
                args_s.pop();
            args.pop();
            break;
        default:
        throw std::invalid_argument("POLIZ: unexpected elem");
        } // end of switch
        ++index;
        // std::cout << "POLIZ: " << pc_el.get_type();
        // std::cout << " Stack: ";
        // print(args);
        // std::cout << " StrStuck: ";
        // print(args_s);
    }; //end of while
    std::cout << "=============================" << std::endl;
}