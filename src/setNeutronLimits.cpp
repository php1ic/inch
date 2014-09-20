#include "include/functions.h"

void setNeutronLimits(const std::vector<Nuclide> &in,
		      inputs *draw
		      )
{
  std::vector<Nuclide>::const_iterator it;

  draw->Nmin = MAX_N;
  draw->Nmax = MIN_N;

  for (it=in.begin(); it!=in.end(); ++it)
    {
      if (   it->Z >= draw->Zmin
	  && it->Z <= draw->Zmax
	  && it->rich % draw->np_rich == 0
	  )
	{
	  if (it->N < draw->Nmin)
	    draw->Nmin = it->N;

	  if (it->N > draw->Nmax)
	    draw->Nmax = it->N;
	}
    }
}
