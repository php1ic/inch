#include "functions.h"

void populateInternalMassTable(inputs *draw,
                               std::vector<Nuclide> & nuc)
{
  //-Read mass table
  if ( !readNUBASE(draw->mass_table_NUBASE,draw->year,nuc) )
    {
      std::cout << "Nuclear data has not been read, exiting..." << std::endl;
      exit(-1);
    }

  if ( draw->AME )
    {
      if ( !readAME(draw->mass_table_AME,nuc) )
        {
          std::cout << "Updated values from AME were not read." << std::endl;
        }
    }

  //-Read user defined nuclei
  if ( draw->own_nuclei )
    {
      if ( !readOWN(draw->my_nuclei,nuc) )
        {
          std::cout << "User defined nuclei have not been read." << std::endl;
        }
    }
  else
    {
      std::cout << "Not drawing any user selected nuclei" << std::endl;
    }
}
