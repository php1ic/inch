#ifndef NUCLIDE_H
#define NUCLIDE_H

#include <string>

class Nuclide
{
public:
  Nuclide();
  ~Nuclide();
  bool exp;
  unsigned short A, Z, N, st, pi, pi_exp, J_exp, J_tent, own, rich;
  float NUBASE_ME, NUBASE_dME, AME_ME, AME_dME, is_nrg, dis_nrg, hl, J;
  std::string symbol, decay;
};

#endif
