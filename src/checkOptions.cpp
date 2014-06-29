#include "include/functions.h"

bool checkOptions(std::map<std::string, std::string> &values,
		  inputs *draw
		  )
{
  std::map<std::string, std::string>::iterator map_it;
  int lines_read=0;

  for (map_it = values.begin(); map_it != values.end(); ++map_it)
    {
      if (map_it->first == "section")
	{
	  draw->section=map_it->second;
	  lines_read++;
	}
      else if (map_it->first == "type")
	{
	  draw->type=map_it->second;

	  if (draw->type == "a")
	    {
	      draw->experimental=1;
	      lines_read++;
	    }
	  else if (draw->type == "b")
	    {
	      draw->experimental=0;
	      lines_read++;
	    }
	  else if (draw->type == "c")
	    {
	      draw->experimental=2;
	      lines_read++;
	    }
	  else
	    {
	      std::cout << "***ERROR***: " << draw->type
			<< " is not a valid choice for 'type'" << std::endl;
	    }
	}
      else if (map_it->first == "choice")
	{
	  draw->choice=map_it->second;
	  lines_read++;
	}
      else if (map_it->first == "required")
	{
	  draw->required=map_it->second;
	}
      else if (map_it->first == "Zmin")
	{
	  draw->Zmin=atoi(map_it->second.c_str());
	  draw->ZminSymbol=convertZToSymbol(atoi(map_it->second.c_str()));

	  if (   (!atoi(map_it->second.c_str()) && map_it->second!="0")
	      || (draw->Zmin<MIN_Z || draw->Zmin>MAX_Z)
	      )
	    {
	      std::cout << "***ERROR***: " << map_it->second
			<< " is not a valid choice for 'Zmin'" << std::endl;
	      return false;
	    }
	  else
	    lines_read++;
	}
      else if (map_it->first == "Zmax")
	{
	  draw->Zmax=atoi(map_it->second.c_str());
	  draw->ZmaxSymbol=convertZToSymbol(atoi(map_it->second.c_str()));

	  if (   (!atoi(map_it->second.c_str()) && map_it->second!="0")
	      || (draw->Zmax<MIN_Z || draw->Zmax>MAX_Z)
	      )
	    {
	      std::cout << "***ERROR***: " << map_it->second
			<< " is not a valid choice for 'Zmax'" << std::endl;
	      return false;
	    }
	  else
	    lines_read++;
	}
      else if (map_it->first == "Nmin")
	{
	  draw->Nmin=atoi(map_it->second.c_str());

	  if (   (!atoi(map_it->second.c_str()) && map_it->second!="0")
	      || (draw->Nmin<MIN_N || draw->Nmin>MAX_N)
	      )
	    {
	      std::cout << "***ERROR***: " << map_it->second
			<< " is not a valid choice for 'Nmin'" << std::endl;
	      return false;
	    }
	  else
	    lines_read++;
	}
      else if (map_it->first == "Nmax")
	{
	  draw->Nmax=atoi(map_it->second.c_str());

	  if (   (!atoi(map_it->second.c_str()) && map_it->second!="0")
	      || (draw->Nmax<MIN_N || draw->Nmax>MAX_N)
	      )
	    {
	      std::cout << "***ERROR***: " << map_it->second
			<< " is not a valid choice for 'Nmax'" << std::endl;
	      return false;
	    }
	  else
	    lines_read++;
	}
      else
	{
	  std::cout << "**WARNING**: " << map_it->first
		    <<" is not a valid option. Ignoring." << std::endl;
	}
    }

  if (lines_read < 3)
    {
      std::cout << "Not enough inputs have been read from the file." << std::endl;
      return false;
    }

  return true;
}
