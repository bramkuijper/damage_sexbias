#!/usr/bin/env Rscript

mf_mm <- list(c(0.9,0.1))

n <- c(1,1)

kappa <- 0.4
gamma_f <- 0.2
gamma_m <- c(0.1,0.2,0.3,0.4,0.5)

d_egg <- 0.05
d_adult <- 0.5

basename <- paste0("sim_sex_damage_", format(Sys.time(),"%Y%m%d_%H%M%S"))

exe="./damage_sex"

nrep <- 3

ctr <- 1

output <- ""

for (rep_i in 1:nrep)
{
    for (mf_mm_i in mf_mm)
    {
        for (kappa_i in kappa)
        {
            for (gamma_mi in gamma_m)
            {
                mfi <- mf_mm_i[1]
                mmi <- mf_mm_i[2]
                file_name <- paste0(basename,"_",ctr)

                output <- paste(output,"\n",
                        paste(exe,
                                mfi,
                                mmi,
                                n[1],
                                n[2],
                                kappa_i,
                                gamma_f,
                                gamma_mi,
                                d_egg,
                                d_adult,
                                file_name))

                ctr <- ctr + 1
            }
        }
    }
}

writeLines(output)
