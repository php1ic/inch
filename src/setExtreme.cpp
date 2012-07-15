#include "functions.h"

void setExtreme(std::string limit, inputs *draw)
{
  if (limit != "Zmin" && limit != "Zmax" && limit != "Nmin" && limit != "Nmax")
    {
      std::cout << "WARNING - " << limit << " is not a valid input, choose either Zmin or Zmax."
		<< "Setting Z limits to 0 and 118" << std::endl;

      draw->Zmin=0;
      draw->Zmax=118;
      draw->Nmin=0;
      draw->Nmax=175;

      return;
    }
  
  short temp=0;
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
	  short length=in.size();
	  in.copy(value,length,0);
	  value[length] = '\0';

	  if (!atoi(value))
	    temp = el_z(in);
	  else
	    temp = atoi(value);
	}

      if (limit == "Zmin")
	{
	  draw->Zmin = temp;
	  temp=1;
	  if (draw->Zmin < 0 || draw->Zmin > 118)
	    {
	      std::cout << "\nZmin must be in the range 0<Z<118.\n" << std::endl;
	      temp=0;
	    }
	}
      else if (limit == "Zmax")
	{
	  draw->Zmax = temp;
	  temp=1;
	  if (draw->Zmax < draw->Zmin || draw->Zmax > 118)
	    {
	      std::cout << "\nZmax must be in the range " << draw->Zmin << "<Z<118.\n" << std::endl;
	      temp=0;
	    }
	}
      else if (limit == "Nmin")
	{
	  draw->Nmin = temp;
	  temp=1;
	  if (draw->Nmin < 0 || draw->Nmin > 175)
	    {
	      std::cout << "\nNmin must be in the range 0<N<175\n" << std::endl;
	      temp=0;
	    }
	}
      else if (limit == "Nmax")
	{
	  draw->Nmax = temp;
	  temp=1;
	  if (draw->Nmax < draw->Nmin || draw->Nmax > 175)
	    {
	      std::cout << "\nNmax must be in the range " << draw->Nmin << "<N<175\n\n";
	      temp=0;
	    }
	}
    }
  while (!temp);
}
