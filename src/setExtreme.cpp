#include "include/functions.h"

void setExtreme(const std::string &limit,
		inputs *draw
		)
{
  if (limit != "Zmin" && limit != "Zmax" && limit != "Nmin" && limit != "Nmax")
    {
      std::cout << "WARNING - " << limit << " is not a valid input, choose either Zmin, Zmax, Nmin or Nmax"
		<< "Setting limits to max values." << std::endl;

      draw->Zmin=0;
      draw->Zmax=MAX_Z;
      draw->Nmin=0;
      draw->Nmax=MAX_N;

      return;
    }

  int temp=0;
  std::string in;

  do
    {
      std::cout << limit << ": ";
      std::cin >> in;

      if (in == "0")
	temp = 0;
      else
	{
	  char value[4];
	  in.copy(value,in.size(),0);
	  value[in.size()] = '\0';

	  if (!atoi(value))
	    temp = convertSymbolToZ(in);
	  else
	    temp = atoi(value);
	}

      if (limit == "Zmin")
	{
	  draw->Zmin = temp;
	  temp=1;
	  if (draw->Zmin < 0 || draw->Zmin > MAX_Z)
	    {
	      std::cout << "\n"
			<< "Zmin must be in the range 0<Z<" << MAX_Z << ".\n"
			<< std::endl;
	      temp=0;
	    }
	}
      else if (limit == "Zmax")
	{
	  draw->Zmax = temp;
	  temp=1;
	  if (draw->Zmax < draw->Zmin || draw->Zmax > MAX_Z)
	    {
	      std::cout << "\n"
			<<"Zmax must be in the range " << draw->Zmin << "<Z<" << MAX_Z
			<< ".\n" << std::endl;
	      temp=0;
	    }
	}
      else if (limit == "Nmin")
	{
	  draw->Nmin = temp;
	  temp=1;
	  if (draw->Nmin < 0 || draw->Nmin > MAX_N)
	    {
	      std::cout << "\n"
			<< "Nmin must be in the range 0<N<" << MAX_N << "\n"
			<< std::endl;
	      temp=0;
	    }
	}
      else if (limit == "Nmax")
	{
	  draw->Nmax = temp;
	  temp=1;
	  if (draw->Nmax < draw->Nmin || draw->Nmax > MAX_N)
	    {
	      std::cout << "\n"
			<< "Nmax must be in the range " << draw->Nmin << "<N<" << MAX_N
			<< ".\n" << std::endl;
	      temp=0;
	    }
	}
    }
  while (!temp);
}
