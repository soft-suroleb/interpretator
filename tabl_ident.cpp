#include "Ident.hpp"
#include "tabl_ident.hpp"

Ident& tabl_ident::operator[](int k){
    return p[k];
}

int tabl_ident::put(std::string buf){
    for (size_t j = 0; j < p.size(); ++j){
        if (buf == p[j].get_name()) return j;
    }
    p.push_back(Ident());
    p[p.size() - 1].put_name(buf);
    return p.size() - 1;
}

int tabl_ident::getTop() const {
    return p.size();
}

Ident tabl_ident::pop() {
    Ident tmp;
    tmp = *(p.end() - 1);
    p.pop_back();
    return tmp;
}