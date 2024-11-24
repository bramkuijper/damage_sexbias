#include "parameters.hpp"
#include "damage_sex.hpp"

int main(int argc, char **argv)
{
    Parameters par{};
    par.m[female] = std::stod(argv[1]);
    par.m[male] = std::stod(argv[2]);
    par.n[female] = std::stod(argv[3]);
    par.n[male] = std::stod(argv[4]);
    par.kappa = std::stod(argv[5]);
    par.gamma[female] = std::stod(argv[6]);
    par.gamma[male] = std::stod(argv[7]);
    par.d_egg = std::stod(argv[8]);
    par.d_adult = std::stod(argv[9]);
    par.file_name = argv[10];

    DamageSex Simulation(par);
}
