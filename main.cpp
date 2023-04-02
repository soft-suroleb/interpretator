#include <iostream>
#include "scanner.hpp"
#include "parser.hpp"
#include <iomanip>
#include <map>
#include <string>
#include <vector>
#include "interpretator.hpp"
using namespace std;

extern tabl_ident TID;
extern map<string, vector<Ident>>map_struct;
// extern vector<Ident*>labels;
extern tabl_ident const_str;

int main(int argc, char** argv){
    Interpretator inter(argv[argc - 1]);
    inter.interpretation();
    const int N = 15;
    cout << endl << "Table of identefiers:" << endl;
    cout << setw(3) << "ID" <<
            setw(N) << "Name" <<
            setw(N) << "Type" <<
            setw(N) << "Is declare" <<
            setw(N) << "Is assign" <<
            setw(N) << "Value" <<
            setw(N) << "Info" << endl;
    for (int i = 0; i < TID.getTop(); i++){
        cout << setw(3) << i <<
                setw(N) << TID[i].get_name() <<
                setw(N) << TID[i].get_type() <<
                setw(N) << TID[i].get_declare() <<
                setw(N) << TID[i].get_assign() <<
                setw(N) << TID[i].get_value() <<
                setw(N) << TID[i].get_info();
        if (TID[i].get_info() == "label"){
            for (auto& j: TID[i].label_info()) cout << " " << j;
        }
        cout << endl;
    }
    cout << endl << "Struct dict:" << endl;
    if (map_struct.size() == 0) cout << "None" << endl;
    for (auto &i: map_struct){
        cout << i.first << ": ";
        for (auto &j: i.second){
            cout << j.get_name() << " ";
        }
        cout << endl;
    }
    cout << endl << "Const strings: " << endl;
    for (int i = 0; i < const_str.getTop(); i++){
        cout << i << ": \"" << const_str[i].get_name() << "\"" << endl;
    }
    return 0;
}
