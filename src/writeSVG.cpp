#include "functions.hpp"

#include "prolog.hpp"

void writeSVG(std::vector<Nuclide> &nuc,
              std::unique_ptr<inputs> &draw
              )
{
  std::ofstream outFile(draw->outfile, std::ios::binary);

  if ( !outFile )
    {
      std::cout << "\n"
                << "***ERROR***: Couldn't open " << draw->outfile
                << " to create the chart." << std::endl;
      return;
    }

  const Prolog setup;
  setup.SVGWriteProlog(outFile, draw);

  outFile << R"(<g transform="translate()" << 0.5*draw->size << "," << 0.5*draw->size
          << ") scale(" << draw->size << "," << draw->size << R"lit()">)lit" << std::endl;

  drawNuclei(nuc,draw,outFile);

  outFile << "</g>\n</svg>" << std::endl;

  outFile.close();
}
