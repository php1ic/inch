#include "include/functions.h"

void setNeutronLimits(const std::vector<Nuclide> &in,
		      inputs *draw
		      )
{
  std::vector<Nuclide>::const_iterator nuc_it;

  draw->Nmin = MAX_N;
  draw->Nmax = MIN_N;

  for (nuc_it=in.begin(); nuc_it!=in.end(); ++nuc_it)
    {
      if (   nuc_it->Z >= draw->Zmin
	  && nuc_it->Z <= draw->Zmax
	  && nuc_it->rich % draw->np_rich == 0
	  )
	{
	  if (nuc_it->N < draw->Nmin)
	    draw->Nmin = nuc_it->N;

	  if (nuc_it->N > draw->Nmax)
	    draw->Nmax = nuc_it->N;
	}
    }
}
