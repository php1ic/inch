#include "functions.h"

void writeTIKZ(std::vector<Nuclide> &in,
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
          << "\\begin{tikzpicture}" << std::endl;

  drawNuclei(in,draw,outFile);

  outFile << "\\end{tikzpicture}\n"
          << "\\end{document}" << std::endl;

  outFile.close();
}
