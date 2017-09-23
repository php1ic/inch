#include "functions.h"

void writeSVG(std::vector<Nuclide> &in,
              std::unique_ptr<inputs> &draw
              )
{
  std::ofstream outFile(draw->outfile.c_str());

  if ( !outFile )
    {
      std::cout << "\n"
                << "***ERROR***: Couldn't open " << draw->outfile
                << " to create the chart." << std::endl;
      return;
    }

  createSVGProlog(draw,outFile);

  outFile << "<g transform=\"translate(" << 0.5*draw->size << "," << 0.5*draw->size
          << ") scale(" << draw->size << "," << draw->size << ")\">" << std::endl;

  drawNuclei(in,draw,outFile);

  outFile << "</g>\n</svg>" << std::endl;

  outFile.close();
}
