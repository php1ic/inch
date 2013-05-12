#include "include/functions.h"

void writeSVG(std::vector<Nuclide> &in,
	      inputs *draw,
	      partition *part
	      )
{
  std::ofstream out_file(draw->outfile.c_str());
  std::vector<Nuclide>::const_iterator nuc_it;

  if (out_file.is_open())
    {
      createSVGProlog(draw,out_file);

      out_file << "<g transform=\"translate(" << 0.5*draw->size << "," << 0.5*draw->size << ") scale(" << draw->size << "," << draw->size << ")\">" << std::endl;

      drawNuclei(in,draw,out_file);

      out_file << "</g>\n</svg>" << std::endl;

      out_file.close();
    }
  else
    {
      std::cout << "\nERROR: Couldn't open " << draw->outfile << " to create the chart." << std::endl;
      exit(-1);
    }
}
