#include "include/functions.h"

float errorQuadrature(const int x, ...)
{
  va_list errors;
  va_start(errors,x);

  float err=0;

  for (int i=0; i<x; ++i)
    {
      float val=va_arg(errors,double);
      err+=val*val;
    }

  va_end(errors);

  return sqrt(err);
}
