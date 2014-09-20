#include "include/functions.h"

double errorQuadrature(const int x, ...)
{
  va_list individualErrors;
  va_start(individualErrors,x);

  double fullError=0.0;

  for (int i=0; i<x; ++i)
    {
      double value=va_arg(individualErrors,double);
      fullError+=value*value;
    }

  va_end(individualErrors);

  return sqrt(fullError);
}
