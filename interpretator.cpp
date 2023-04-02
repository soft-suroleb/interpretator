#include "interpretator.hpp"

Interpretator::Interpretator (const char * program):pars(program){};

void Interpretator::interpretation(){
	pars.analyze();
    std::cout << "========= Execution =========" << std::endl;
    E.execute(pars.prog);
}