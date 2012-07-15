#include "nuclide.h"

Nuclide::Nuclide()
{
  exp=0;
  A=Z=N=st=pi=pi_exp=J_exp=J_tent=own=rich=0;
  NUBASE_ME=AME_ME=is_nrg=dis_nrg=hl=J=0.0;
  NUBASE_dME=AME_dME=500.0;
}

Nuclide::~Nuclide()
{
}
