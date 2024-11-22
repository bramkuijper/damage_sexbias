#include "parameters.hpp"
#include "damage_sex.hpp"

int main(int argc, char **argv)
{
    Parameters par;
    par.d[female] = std::stod(argv[1]);
    par.d[male] = std::stod(argv[2]);

    DamageSex Simulation(par);
}
