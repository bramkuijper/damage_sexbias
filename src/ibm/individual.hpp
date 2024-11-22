#ifndef _INDIVIDUAL_HPP_
#define _INDIVIDUAL_HPP_

class Individual
{
    public:
        double t[2][2]{{0.0,0.0},{0.0,0.0}};

        bool is_damaged{false};



        // initialization constructor
        Individual(double const init_tf, 
                double const init_tm, 
                bool const damage);

        // birth constructor
        Individual(Individual const &mother,
                Individual const &father,
                std::mt19937 &rng_r,
                Parameters const &par);

};

#endif
