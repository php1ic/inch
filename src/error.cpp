#include "functions.h"

double error(short x, ...)
{
  va_list errors;
  va_start(errors,x);

  double err=0;

  for(int i=0; i<x; ++i)
    {
      double val=va_arg(errors,double);
      err+=val*val;
    }

  va_end(errors);

  return sqrt(err);
}
