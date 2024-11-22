#include "parameters.hpp"
#include "individual.hpp"
#include "patch.hpp"

Patch::Patch(Parameters const &par) :
    females(par.n[female], Individual(par.init_t[female], par.init_t[male], false, true)),
    males(par.n[male], Individual(par.init_t[female], par.init_t[male], false, false))
{
}

Patch::Patch(Patch const &other) :
    females{other.females},
    males{other.males}
{}

void Patch::operator=(Patch const &other)
{
    females = other.females;
    males = other.males;
}

