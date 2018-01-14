#include "functions.hpp"

void writeSVG(std::vector<Nuclide> &nuc,
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

  outFile << R"(<g transform="translate()" << 0.5*draw->size << "," << 0.5*draw->size
          << ") scale(" << draw->size << "," << draw->size << R"lit()">)lit" << std::endl;

  drawNuclei(nuc,draw,outFile);

  outFile << "</g>\n</svg>" << std::endl;

  outFile.close();
}
