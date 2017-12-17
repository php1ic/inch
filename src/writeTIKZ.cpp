#include "functions.h"

void writeTIKZ(std::vector<Nuclide> &nuc,
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

  createTIKZProlog(draw,outFile);

  outFile << "\\begin{document}\n"
          << R"(\begin{tikzpicture})" << std::endl;

  drawNuclei(nuc,draw,outFile);

  outFile << "\\end{tikzpicture}\n"
          << R"(\end{document})" << std::endl;

  outFile.close();
}
