#include "include/functions.h"

void constructFullyQualifiedPaths(inputs *draw)
{
  draw->FRDM.insert(0,draw->path);

  draw->my_nuclei.insert(0,draw->path);

  draw->mass_table_AME.insert(0,draw->path);

  draw->mass_table_NUBASE.insert(0,draw->path);

  if (draw->AME)
    draw->mass_table = draw->mass_table_AME;
  else
    draw->mass_table = draw->mass_table_NUBASE;

  draw->options.insert(0,draw->pwd);
}
