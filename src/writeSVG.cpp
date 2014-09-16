#include "include/functions.h"

void writeSVG(std::vector<Nuclide> &in,
	      inputs *draw
	      )
{
  std::ofstream outFile(draw->outfile.c_str());

  if ( !outFile.is_open() )
    {
      std::cout << "\n"
		<< "***ERROR***: Couldn't open " << draw->outfile
		<< " to create the chart." << std::endl;
      exit(-1);
    }

  createSVGProlog(draw,outFile);

  outFile << "<g transform=\"translate(" << 0.5*draw->size << "," << 0.5*draw->size
	  << ") scale(" << draw->size << "," << draw->size << ")\">" << std::endl;

  drawNuclei(in,draw,outFile);

  outFile << "</g>\n</svg>" << std::endl;

  outFile.close();
}
