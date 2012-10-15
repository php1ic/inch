#include "functions.h"

void setNlimits(std::vector<Nuclide> &in, inputs *draw)
{
  std::vector<Nuclide>::iterator nuc_it;

  draw->Nmin = 176;
  draw->Nmax = 0;

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
