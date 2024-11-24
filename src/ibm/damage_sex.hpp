#ifndef _DAMAGE_SEX_HPP_
#define _DAMAGE_SEX_HPP_

#include <fstream>
#include <iostream>
#include <sstream>
#include <random>
#include "patch.hpp"
#include "parameters.hpp"


class DamageSex
{
    private:
        Parameters par;
        std::ofstream data_file;
        std::uniform_real_distribution<double> uniform;
        std::discrete_distribution<unsigned> patch_level_fecundity_distribution{1,1};
        double total_fecundity{0.0};

        std::random_device rd;
        unsigned seed;
        std::mt19937 rng_r;

        unsigned time_step{0};
    
        std::vector <Patch> metapopulation;
        std::vector <double> patch_fecundities;

        // some stats
        double mean_mortality[3]{0.0,0.0};

        void reproduce();
        void survive();
        void replace();
        void write_data_headers();
        void write_data();
        void write_parameters();

    public:
        DamageSex(Parameters const &parameters);
};

#endif
