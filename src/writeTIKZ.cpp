#include "functions.hpp"

#include "prolog.hpp"

void writeTIKZ(std::vector<Nuclide> &nuc,
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
  setup.TIKZWriteProlog(outFile, draw);

  outFile << "\\begin{document}\n"
          << R"(\begin{tikzpicture})" << std::endl;

  drawNuclei(nuc,draw,outFile);

  outFile << "\\end{tikzpicture}\n"
          << R"(\end{document})" << std::endl;

  outFile.close();
}
