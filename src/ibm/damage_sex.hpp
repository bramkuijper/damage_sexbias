#ifndef _DAMAGE_SEX_HPP_
#define _DAMAGE_SEX_HPP_

#include <fstream>
#include <iostream>
#include <sstream>
#include <random>
#include "parameters";


class DamageSex
{
    private:
        Parameters par;
        std::ofstream data_file;
        std::uniform_real_distribution<double> uniform;

        std::random_device rd;
        unsigned seed;
        std::mt19937 rng_r;

        unsigned time_step{0};
    
        std::vector <Patch> metapopulation;

        void survival();
        void write_data_headers();
        void write_data();

    public:
        DamageSex(Parameters const &parameters);
};

#endif
