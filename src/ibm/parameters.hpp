#ifndef _PARAMETERS_HPP_
#define _PARAMETERS_HPP_

#include <string>

enum Sex
{
    female = 0,
    male = 1
};

class Parameters
{
    public:
        unsigned n[2]{2,2};
        unsigned npatches{1250};
        unsigned long max_time{100000};

        double m[2]{0.5,0.5};

        double mortality[2][2]{{0.0,0.0},{0.0,0.0}};
        double fecundity[2]{0.0,0.0};

        double init_t[2]{0.0,0.0};

        // probability that damage arises spontaneously
        // in the egg stage
        double d_egg{0.0};
        double d_adult{0.0};

        double mu_t[2]{0.02,0.02};

        double sdmu{0.05};

        double kappa{0.1};
        double gamma[2]{0.1,0.1};
        double delta{0.5};

        std::string file_name{"sim_damage_sex"};

        unsigned skip{10};

        Parameters() {
            // female, undamaged
            mortality[female][0] = gamma[female];
            // female, damaged
            mortality[female][1] = gamma[female] + kappa;
            // male, undamaged
            mortality[male][0] = gamma[male];
            // male, damaged
            mortality[male][1] = gamma[male] + kappa;

            // female, undamaged
            fecundity[0] = 1.0;
            fecundity[1] = 1.0 - delta;
        }
};

#endif 
