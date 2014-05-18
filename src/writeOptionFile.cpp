#include "include/functions.h"

void writeOptionFile(inputs *draw)
{
  std::ofstream opts(draw->options.c_str());

  if (opts.is_open())
    {
      std::cout << "Writing user choices to " << draw->options;

      opts << "section=" << draw->section << "\n";

      if (draw->section == "b")
	{
	  opts << "Zmin=" << draw->Zmin << "\n"
	       << "Zmax=" << draw->Zmax << "\n"
	       << "required=" << draw->required << "\n";

	  if (draw->required == "b")
	    opts << "Nmin=" << draw->Nmin << "\n"
		 << "Nmax=" << draw->Nmax << "\n";
	}

      opts << "type=" << draw->type << "\n"
	   << "choice=" << draw->choice << std::endl;

      opts.close();

      std::cout << " - done\n" << std::endl;
    }
  else
    std::cout << "\n"
	      << "***ERROR***: Couldn't open " << draw->options << " to write the options.\n"
	      << "             Not creating any option file." << std::endl;
}
