#ifndef NUCLIDE_H
#define NUCLIDE_H

#include <string>

class Nuclide
{
 public:
  Nuclide();
  ~Nuclide();

  bool own;

  int exp;
  int A;
  int Z;
  int N;
  int st;
  int pi;
  int pi_exp;
  int J_exp;
  int J_tent;
  int rich;
  int show;

  double NUBASE_ME;
  double NUBASE_dME;
  double AME_ME;
  double AME_dME;
  double s_n;
  double ds_n;
  double s_2n;
  double ds_2n;
  double s_p;
  double ds_p;
  double s_2p;
  double ds_2p;
  double dV_pn;
  double ddV_pn;
  double is_nrg;
  double dis_nrg;
  double hl;
  double J;

  std::string decay;
  std::string colour;
};

#endif
