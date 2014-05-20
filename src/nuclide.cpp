#include "include/nuclide.h"

Nuclide::Nuclide():
  //bool
  exp(false),
  own(false),
  //int
  A(0),
  Z(0),
  N(0),
  st(0),
  pi(0),
  pi_exp(0),
  J_exp(0),
  J_tent(0),
  rich(0),
  show(0),
  //double
  NUBASE_ME(0.0),NUBASE_dME(0.0),
  AME_ME(0.0),AME_dME(0.0),
  s_n(0.0),ds_n(0.0),
  s_2n(0.0),ds_2n(0.0),
  s_p(0.0),ds_p(0.0),
  s_2p(0.0),ds_2p(0.0),
  dV_pn(0.0),ddV_pn(0.0),
  is_nrg(0.0),dis_nrg(0.0),
  hl(0.0),
  J(0.0),
  //string
  symbol(""),
  decay(""),
  colour("")
{
}

Nuclide::~Nuclide()
{
}
