#include <cassert>
#include <random>
#include "individual.hpp"

Individual::Individual(
        double const init_tf,
        double const init_tm,
        double const damage,
        bool is_female) :
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
    clutch_size{other.clutch_size}
{
}

// birth constructor
Individual::Individual(Individual const &mother
        ,Individual const &father
        ,std::mt19937 &rng_r
        ,Parameters const &par
        ,bool const is_female) :
    is_female{is_female}
{
    std::bernoulli_distribution segregator{0.5};
    std::uniform_real_distribution uniform{0.0,1.0};

    for (unsigned sex_idx = 0; sex_idx < 2; ++sex_idx)
    {
        t[sex_idx][0] = mother.t[sex_idx][segregator];

        if (uniform(rng_r) < par.mu_t[sex_idx])
        {
            std::normal_distribution mutational_effect(0.0, par.sdmu);
            t[sex_idx][0] += mutational_effect(rng_r);
        }

       t[sex_idx][1] = father.t[sex_idx][segregator];

        if (uniform(rng_r) < par.mu_t[sex_idx])
        {
           std::normal_distribution mutational_effect(0.0, par.sdmu);
           t[sex_idx][1] += mutational_effect(rng_r);
        }
    }

    assert(clutch_size > 0.0);

    if (!mother.is_damaged)
    {
        is_damaged = uniform(rng_r) < d_egg;
    } 
    else
    {
        is_damage = uniform(rng) < mother.t[is_female] / mother.clutch_size;
    }

    // TODO update maternal damage
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
    is_female = other.is_female;
    clutch_size = other.clutch_size;
} // end overloaded assignment operator

