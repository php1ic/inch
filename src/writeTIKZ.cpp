#include "include/functions.h"

void writeTIKZ(std::vector<Nuclide> &in,
	       inputs *draw,
	       partition *part
	       )
{
  std::ofstream out_file(draw->outfile.c_str());

  if (out_file.is_open())
    {
      createTIKZProlog(draw,out_file);

      out_file << "\\begin{document}\n"
	       << "\\begin{tikzpicture}" << std::endl;

      drawNuclei(in,draw,out_file);

      out_file << "\\end{tikzpicture}\n"
	       << "\\end{document}" << std::endl;

      out_file.close();
    }
  else
    {
      std::cout << "\nERROR: Couldn't open " << draw->outfile << " to create the chart." << std::endl;
      exit(-1);
    }
}
