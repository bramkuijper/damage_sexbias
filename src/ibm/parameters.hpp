#ifndef _PARAMETERS_HPP_
#define _PARAMETERS_HPP_

enum Sex
{
    female = 0,
    male = 1
};

class Parameters
{
    public:
        unsigned n[2]{2,2};
        unsigned long max_time{100};

        double m[2]{0.5,0.5};

        double init_t[2]{0.0,0.0};

        // probability that damage arises spontaneously
        // in the egg stage
        double d_egg{0.0};

};

#endif 
