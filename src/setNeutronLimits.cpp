#include "functions.h"

void setNeutronLimits(const std::vector<Nuclide> &in,
                      std::unique_ptr<inputs> &draw
                      )
{
  draw->Nmin = MAX_N;
  draw->Nmax = MIN_N;

  for ( const auto &it : in )
    {
      if (   it.Z >= draw->Zmin
          && it.Z <= draw->Zmax
          && it.rich % draw->np_rich == 0
          )
        {
          if ( it.N < draw->Nmin )
            {
              draw->Nmin = it.N;
            }

          if ( it.N > draw->Nmax )
            {
              draw->Nmax = it.N;
            }
        }
    }
}
