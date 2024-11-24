#include <cassert>
#include <algorithm>
#include <random>
#include "parameters.hpp"
#include "individual.hpp"

Individual::Individual(
        double const init_tf,
        double const init_tm,
        bool const damage,
        bool const is_female) :
    t{{init_tf/2,init_tf/2},{init_tm/2,init_tm/2}},
    is_damaged{damage},
    is_female{is_female}
{
}// Individual constructor

// copy constructor
Individual::Individual(Individual const &other) :
    t{{other.t[female][0],other.t[female][1]},
        {other.t[male][0],other.t[male][1]}},
    is_damaged{other.is_damaged},
    is_female{other.is_female},
    is_alive{other.is_alive},
    clutch_size{other.clutch_size}
{
}

// birth constructor
Individual::Individual(Individual const &mother
        ,Individual const &father
        ,std::mt19937 &rng_r
        ,Parameters const &par
        ,bool const is_female) :
    is_female{is_female},
    is_alive{true}
{
    std::bernoulli_distribution segregator{0.5};
    std::uniform_real_distribution uniform{0.0,1.0};

    for (unsigned sex_idx = 0; sex_idx < 2; ++sex_idx)
    {
        t[sex_idx][0] = mother.t[sex_idx][segregator(rng_r)];

        if (uniform(rng_r) < par.mu_t[sex_idx])
        {
            std::normal_distribution mutational_effect(0.0, par.sdmu);
            t[sex_idx][0] += mutational_effect(rng_r);

            t[sex_idx][0] = std::clamp(t[sex_idx][0],0.0,0.25);
        }

       t[sex_idx][1] = father.t[sex_idx][segregator(rng_r)];

        if (uniform(rng_r) < par.mu_t[sex_idx])
        {
           std::normal_distribution mutational_effect(0.0, par.sdmu);
           t[sex_idx][1] += mutational_effect(rng_r);

            t[sex_idx][1] = std::clamp(t[sex_idx][1],0.0,0.25);
        }
    }

    assert(mother.clutch_size > 0.0);

    if (!mother.is_damaged)
    {
        is_damaged = uniform(rng_r) < par.d_egg;
    } 
    else
    {
        double t = mother.t[is_female][0] + mother.t[is_female][1];

        is_damaged = uniform(rng_r) < t;
    }
    
} // birth constructor

void Individual::operator=(Individual const &other)
{
    for (unsigned sex_idx{0}; sex_idx < 2; ++sex_idx)
    {
        for (unsigned allele_idx{0}; allele_idx < 2; ++allele_idx)
        {
            t[sex_idx][allele_idx] = other.t[sex_idx][allele_idx];
        }
    }
    
    is_damaged = other.is_damaged;
    is_alive = other.is_alive;
    is_female = other.is_female;
    clutch_size = other.clutch_size;
} // end overloaded assignment operator

