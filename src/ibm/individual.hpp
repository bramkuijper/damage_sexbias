#ifndef _INDIVIDUAL_HPP_
#define _INDIVIDUAL_HPP_

#include <random>
#include "parameters.hpp"

class Individual
{
    public:
        double t[2][2]{{0.0,0.0},{0.0,0.0}};

        bool is_damaged{false};

        bool is_female{true};

        bool is_alive{true};


        double clutch_size{0.0};

        // initialization constructor
        Individual(double const init_tf, 
                double const init_tm, 
                bool const damage,
                bool const is_female);

        // copy constructor
        Individual(Individual const &other);

        // birth constructor
        Individual(Individual const &mother,
                Individual const &father,
                std::mt19937 &rng_r,
                Parameters const &par,
                bool const is_female);

        void operator=(Individual const &other);
};

#endif
