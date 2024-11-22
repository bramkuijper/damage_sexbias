#ifndef _PATCH_HPP_
#define _PATCH_HPP_

#include <random>
#include <vector>

#include "parameters.hpp"
#include "individual.hpp"

class Patch
{
    private:
        std::vector <Individual> females;
        std::vector <Individual> males;

    public:
        Patch(Parameters const &par);
        Patch(Patch const &other);
        void operator=(Patch const &other);
};

#endif 
