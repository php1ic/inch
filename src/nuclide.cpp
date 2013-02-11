#include "nuclide.h"

Nuclide::Nuclide()
{
  exp=own=false;
  A=Z=N=st=pi=pi_exp=J_exp=J_tent=rich=0;
  NUBASE_ME=AME_ME=s_n=s_2n=s_p=s_2p=dV_pn=is_nrg=dis_nrg=hl=J=0.0;
  NUBASE_dME=AME_dME=ds_n=ds_2n=ds_p=ds_2p=ddV_pn=500.0;
}

Nuclide::~Nuclide()
{
}
