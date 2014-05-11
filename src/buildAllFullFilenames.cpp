#include "include/functions.h"

void buildAllFullFilenames(inputs *draw)
{
  constructFilePaths(draw);

  constructFullyQualifiedPaths(draw);
}
