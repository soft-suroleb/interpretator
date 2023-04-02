#pragma once
#include "Ident.hpp"
#include <vector>
#include <string>

class tabl_ident{
    std::vector<Ident> p;
public:
    int getTop() const;
    Ident& operator[](int k);
    int put(std::string buf);
    Ident pop();
};
