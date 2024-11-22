#include "damage_sex.hpp"

DamageSex::DamageSex(Parameters const &par) :
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
        survival();
    }
} // end DamageSex
