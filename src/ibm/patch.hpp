#ifndef _PATCH_HPP_
#define _PATCH_HPP_

#include <random>
#include <vector>

#include "parameters.hpp"
#include "individual.hpp"

class Patch
{
    public:
        std::vector <Individual> females;
        std::vector <Individual> males;
        std::vector <Individual> juvenile_females;
        std::vector <Individual> juvenile_males;

        std::discrete_distribution <unsigned> fecundity_distribution{1,1};

        double fecundity{0.0};

        Patch(Parameters const &par);
        Patch(Patch const &other);
        void operator=(Patch const &other);
};

#endif 
