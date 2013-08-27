#ifndef NUCLIDE_H
#define NUCLIDE_H

#include <string>

class Nuclide
{
 public:
  Nuclide();
  ~Nuclide();
  bool exp, own, show;
  int A, Z, N, st, pi, pi_exp, J_exp, J_tent, rich;
  float
    NUBASE_ME, NUBASE_dME,
    AME_ME, AME_dME,
    s_n,   ds_n,
    s_2n,  ds_2n,
    s_p,   ds_p,
    s_2p,  ds_2p,
    dV_pn, ddV_pn,
    is_nrg, dis_nrg,
    hl, J;
  std::string symbol, decay, colour;
};

#endif
