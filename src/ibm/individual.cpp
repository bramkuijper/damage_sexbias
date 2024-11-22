#include "individual.hpp"

Individual::Individual(double const init_tf,
        double const init_tm,
        double const damage) :
    t{{init_tf/2,init_tf/2},{init_tm/2,init_tm/2}},
    is_damaged{damage}
{
}// Individual constructor

Individual::Individual(Individual const &mother
        ,Individual const &father
        ,std::mt19937 &rng_r
        ,Parameters const &par)
{
    std::bernoulli_distribution segregator{0.5};
    std::uniform_real_distribution uniform{0.0,1.0};

    for (unsigned sex_idx = 0; sex_idx < 2; ++sex_idx)
    {
        t[sex_idx][0] = mother.t[sex_idx][segregator];
        if (uniform(rng_r) < par.mu_tf)
        {
            std::normal_distribution mutational_effect(0.0, par.sdmu);
            t[sex_idx][0] += mutational_effect(rng_r);

       t[sex_idx][1] = father.t[sex_idx][segregator];
       mutate01(t[sex_idx][1], mu_t[sex_idx], sdmu);

    }
}

