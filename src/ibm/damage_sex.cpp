#include <cassert>
#include "damage_sex.hpp"

DamageSex::DamageSex(Parameters const &params) :
    par{params},
    data_file{par.file_name},
    uniform{0.0,1.0},
    rd{},
    seed{rd()},
    rng_r{seed},
    metapopulation{par.npatches, Patch(par)}
{
    write_data_headers();

    for (time_step = 0; 
            time_step <= par.max_time; ++time_step)
    {
        reproduce(); // make fecundity distributions etc
        survive(); // now have males and females die
        replace(); // and then replace dead individuals by newborns
        
        if (time_step % par.skip == 0)
        {
            write_data();
        }
    }

    write_parameters();
} // end DamageSex

// write headers to the data file
void DamageSex::write_data_headers()
{
    data_file << "time;clutch;meantf;meantm;vartf;vartm;fecundity;p_survival_m;p_survival_f;freq_fu;freq_fd;freq_mu;freq_md;n" << std::endl;

}

void DamageSex::reproduce()
{

    std::vector <double> individual_level_fecundities;
    std::vector <double> patch_level_fecundities;
    double sum_fecundity, f;

    total_fecundity = 0.0;

    for (auto patch_iterator = metapopulation.begin();
            patch_iterator != metapopulation.end();
            ++patch_iterator)
    {
        sum_fecundity = 0.0;
        individual_level_fecundities.clear();

        for (auto female_iterator = patch_iterator->females.begin();
                female_iterator != patch_iterator->females.end();
                ++female_iterator)
        {
            f = female_iterator->clutch_size = par.fecundity[female_iterator->is_damaged];
            sum_fecundity += f;
            individual_level_fecundities.push_back(f);
        } // end for female_iterator

        patch_iterator->fecundity = sum_fecundity;
        total_fecundity += sum_fecundity;
        
        // for each patch add a total fecundity to the patch
        // level distribution
        patch_level_fecundities.push_back(sum_fecundity);
        
        // make the within-patch fecundity distribution
        std::discrete_distribution<unsigned>::param_type 
            fecundity_distribution_param(
                    individual_level_fecundities.begin()
                    ,individual_level_fecundities.end());

        // update within patch fecundity discrete distribution
        patch_iterator->fecundity_distribution.param(fecundity_distribution_param);
    } // end for patch_iterator

    assert(total_fecundity > 0);

    // update between-patch fecundity distribution
    std::discrete_distribution<unsigned>::param_type
        patch_level_fecundities_param(
                patch_level_fecundities.begin()
                ,patch_level_fecundities.end());

    patch_level_fecundity_distribution.param(patch_level_fecundities_param);
} // reproduce()

// survive
void DamageSex::survive()
{
    double plocal_female,plocal_male;
    unsigned patch_origin_id, pair_id;

    std::uniform_int_distribution<unsigned> male_sampler(0, par.n[male] - 1);
    
    for (unsigned patch_idx = 0;
            patch_idx < metapopulation.size();
            ++patch_idx)
    {
        metapopulation[patch_idx].juvenile_females.clear();
        metapopulation[patch_idx].juvenile_males.clear();

        plocal_female = (1.0 - par.m[female]) * 
            metapopulation[patch_idx].fecundity / 
            (
             (1.0 - par.m[female]) * metapopulation[patch_idx].fecundity 
             +
             par.m[female] * total_fecundity / par.npatches);
        
        plocal_male = (1.0 - par.m[male]) * 
            metapopulation[patch_idx].fecundity / 
            ((1.0 - par.m[male]) * metapopulation[patch_idx].fecundity 
             +
             par.m[male] * total_fecundity / par.npatches);

        for (auto female_iterator = metapopulation[patch_idx].females.begin();
                female_iterator != metapopulation[patch_idx].females.end();
                ++female_iterator)
        {
            assert(female_iterator->is_alive);

            // adult female dies, replace
            if (uniform(rng_r) < par.mortality[female][female_iterator->is_damaged])
            {
                // this female is dead
                female_iterator->is_alive = false;

                // now assess whether newborn female wll be from
                // local patch
                if (uniform(rng_r) < plocal_female)
                {
                    patch_origin_id = patch_idx;
                    assert(patch_origin_id < par.npatches);
                } 
                else
                {
                    // sample patch from between patch 
                    // fecundity distribution
                    patch_origin_id = 
                        patch_level_fecundity_distribution(rng_r);
                    
                    assert(patch_origin_id < par.npatches);
                }

                // sample mom dad from fecundity distribution
                pair_id = metapopulation[patch_origin_id].fecundity_distribution(rng_r);
                    
                assert(pair_id < par.n[female]);

                // make new child that will replace individual
                metapopulation[patch_idx].juvenile_females.push_back(
                        Individual(
                            metapopulation[patch_origin_id].females[pair_id]
                            ,metapopulation[patch_origin_id].males[male_sampler(rng_r)]
                            ,rng_r
                            ,par
                            ,true));
            } // end if mortality female
        } // end for female_iterator

        for (auto male_iterator = metapopulation[patch_idx].males.begin();
                male_iterator != metapopulation[patch_idx].males.end();
                ++male_iterator)
        {
            assert(male_iterator->is_alive);

            // adult male dies, replace
            if (uniform(rng_r) < par.mortality[male][male_iterator->is_damaged])
            {
                // this female is dead
                male_iterator->is_alive = false;

                // now assess whether newborn female wll be from
                // local patch
                if (uniform(rng_r) < plocal_male)
                {
                    patch_origin_id = patch_idx;
                    assert(patch_origin_id < par.npatches);
                } 
                else
                {
                    // sample patch from between patch 
                    // fecundity distribution
                    patch_origin_id = 
                        patch_level_fecundity_distribution(rng_r);
                    
                    assert(patch_origin_id < par.npatches);
                }

                // sample mom dad from fecundity distribution
                pair_id = metapopulation[patch_origin_id].fecundity_distribution(rng_r);
                    
                assert(pair_id < par.n[female]);

                // make new child that will replace individual
                metapopulation[patch_idx].juvenile_males.push_back(Individual(
                        metapopulation[patch_origin_id].females[pair_id]
                        ,metapopulation[patch_origin_id].males[male_sampler(rng_r)]
                        ,rng_r
                        ,par
                        ,false));
            } // end if mortality male
        } // end for male_iterator
    } // end for patch_idx
} // end DamageSex::survive()

void DamageSex::replace()
{
    unsigned n_dead[2];

    double t;

    mean_mortality[female] = 0.0;
    mean_mortality[male] = 0.0;

    // now replace dead individuals
    for (unsigned patch_idx = 0;
            patch_idx < metapopulation.size();
            ++patch_idx)
    {
        n_dead[female] = 0.0; 
        n_dead[male] = 0.0; 
        // random shuffle vector with newborn females
        std::shuffle(
                metapopulation[patch_idx].juvenile_females.begin(),
                metapopulation[patch_idx].juvenile_females.end(),
                rng_r);

        for (auto female_iterator = metapopulation[patch_idx].females.begin();
                female_iterator != metapopulation[patch_idx].females.end();
                ++female_iterator)
        {
            // alive: update damage
            if (female_iterator->is_alive)
            {
                // check whether dumping damage
                // has freed this female from damage now
                if (female_iterator->is_damaged)
                {
                    t = 0.5 * (
                            female_iterator->t[female][0] + female_iterator->t[female][1])
                            +
                        0.5 * (
                            female_iterator->t[male][0] + female_iterator->t[male][1]);

                    if (uniform(rng_r) < t)
                    {
                        female_iterator->is_damaged = false;
                    }
                }
                else // female undamaged will she obtain damage this time step
                {
                    if (uniform(rng_r) < par.d_adult)
                    {
                        female_iterator->is_damaged = true;
                    }
                }
            } // end if female_iterator is alive
            else // female dead, replace
            {
                assert(metapopulation[patch_idx].juvenile_females.size() > 0);

                ++n_dead[female];
                *female_iterator = metapopulation[patch_idx].juvenile_females.back();
                metapopulation[patch_idx].juvenile_females.pop_back();
            }
        } // end female_iterator


        for (auto male_iterator = metapopulation[patch_idx].males.begin();
                male_iterator != metapopulation[patch_idx].males.end();
                ++male_iterator)
        {
            // alive: update damage
            if (male_iterator->is_alive)
            {
                // male undamaged will he obtain damage this time step
                if (!(male_iterator->is_damaged)) 
                {
                    if (uniform(rng_r) < par.d_adult)
                    {
                        male_iterator->is_damaged = true;
                    }
                }
            } // end if male_iterator is alive
            else // male dead, replace
            {
                assert(metapopulation[patch_idx].juvenile_males.size() > 0);

                ++n_dead[male];
                *male_iterator = metapopulation[patch_idx].juvenile_males.back();
                
                metapopulation[patch_idx].juvenile_males.pop_back();
            }
        } // end male_iterator
   
        mean_mortality[female] += n_dead[female];
        mean_mortality[male] += n_dead[male];

    } // end patch_idx
} // end damage::survival()

void DamageSex::write_parameters()
{
    data_file << std::endl 
        << std::endl
        << "seed;" << seed << std::endl
        << "nf;" << par.n[female] << std::endl
        << "nm;" << par.n[male] << std::endl
        << "npatches;" << par.npatches << std::endl
        << "mf;" << par.m[female] << std::endl
        << "mm;" << par.m[male] << std::endl
        << "mortality_fu;" << par.mortality[female][0] << std::endl
        << "mortality_fd;" << par.mortality[female][1] << std::endl
        << "mortality_mu;" << par.mortality[male][0] << std::endl
        << "mortality_md;" << par.mortality[male][1] << std::endl
        << "fecundity_fu;" << par.fecundity[0] << std::endl
        << "fecundity_fd;" << par.fecundity[1] << std::endl
        << "d_egg;" << par.d_egg << std::endl
        << "d_adult;" << par.d_adult << std::endl
        << "mu_tf;" << par.mu_t[female] << std::endl
        << "mu_tm;" << par.mu_t[male] << std::endl
        << "sdmu;" << par.sdmu << std::endl
        << "kappa;" << par.kappa << std::endl
        << "gamma_f;" << par.gamma[female] << std::endl
        << "gamma_m;" << par.gamma[male] << std::endl
        << "delta;" << par.delta << std::endl;
} // end write_parameters

void DamageSex::write_data()
{
    double mean_t[2]{0.0,0.0};
    double ss_t[2]{0.0,0.0};
    double mean_clutch{0.0};
    double ntypes[2][2]{{0.0,0.0},{0.0,0.0}};

    double t;

    for (unsigned patch_idx = 0;
            patch_idx < metapopulation.size();
            ++patch_idx)
    {
        for (auto female_iterator = metapopulation[patch_idx].females.begin();
                female_iterator != metapopulation[patch_idx].females.end();
                ++female_iterator)
        {
            assert(female_iterator->is_alive);
            assert(female_iterator->t[female][0] >= 0.0);
            assert(female_iterator->t[female][0] <= 1.0);
            assert(female_iterator->t[female][1] >= 0.0);
            assert(female_iterator->t[female][1] <= 1.0);
            assert(female_iterator->t[male][0] >= 0.0);
            assert(female_iterator->t[male][0] <= 1.0);
            assert(female_iterator->t[male][1] >= 0.0);
            assert(female_iterator->t[male][1] <= 1.0);

            t = female_iterator->t[female][0] + 
                    female_iterator->t[female][1];

            mean_t[female] += t;
            ss_t[female] += t * t;
                
            t = female_iterator->t[male][0] + 
                female_iterator->t[male][1];

            mean_t[male] += t;

            ss_t[male] += t * t;

            ++ntypes[female][female_iterator->is_damaged];

            mean_clutch += female_iterator->clutch_size;
        }
        
        for (auto male_iterator = metapopulation[patch_idx].males.begin();
                male_iterator != metapopulation[patch_idx].males.end();
                ++male_iterator)
        {
            assert(male_iterator->is_alive);
            assert(male_iterator->t[female][0] >= 0.0);
            assert(male_iterator->t[female][0] <= 1.0);
            assert(male_iterator->t[female][1] >= 0.0);
            assert(male_iterator->t[female][1] <= 1.0);
            assert(male_iterator->t[male][0] >= 0.0);
            assert(male_iterator->t[male][0] <= 1.0);
            assert(male_iterator->t[male][1] >= 0.0);
            assert(male_iterator->t[male][1] <= 1.0);

            t = male_iterator->t[female][0] + 
                    male_iterator->t[female][1];

            mean_t[female] += t;
            ss_t[female] += t * t;
                
            t = male_iterator->t[male][0] + 
                male_iterator->t[male][1];

            mean_t[male] += t;

            ss_t[male] += t * t;

            ++ntypes[male][male_iterator->is_damaged];
        }
    } // end patch_idx
    
    unsigned population_size = par.npatches * (par.n[female] + par.n[male]);

    mean_clutch /= par.npatches * par.n[female];

    mean_t[female] /= population_size;
    ss_t[female] /= population_size;
    mean_t[male] /= population_size;
    ss_t[male] /= population_size;

    total_fecundity /= par.npatches * par.n[female];
    mean_mortality[male] /= par.npatches * par.n[male];
    mean_mortality[female] /= par.npatches * par.n[female];

    double var_t_f = ss_t[female] - mean_t[female] * mean_t[female];
    double var_t_m = ss_t[male] - mean_t[male] * mean_t[male];

    data_file << time_step << ";" 
        << mean_clutch << ";"
        << mean_t[female] << ";"
        << mean_t[male] << ";"
        << var_t_f << ";"
        << var_t_m << ";"
        << total_fecundity << ";"
        << (1.0 - mean_mortality[male]) << ";"
        << (1.0 - mean_mortality[female]) << ";"
        << ntypes[female][0] / population_size << ";"
        << ntypes[female][1] / population_size << ";"
        << ntypes[male][0] / population_size << ";"
        << ntypes[male][1] / population_size << ";"
        << population_size
        << std::endl;


} // end write_data();

