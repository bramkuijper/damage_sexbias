#include "patch.hpp"

Patch::Patch(Parameters &const par) :
    females{par.n[female], par.init_tf, par_init_m, false},
    males{par.n[male], par.init_tf, par_init_m, false}
{
}

Patch::Patch(Patch const &other) :
    females{other.females},
    males{other.males}
{}

void operator=(Patch const &other)
{
    females = other.females;
    males = other.males;
}

