#include "functions.h"

void writeSVG(const std::vector<Nuclide> &in,
	      inputs *draw
	      )
{
  std::ofstream out_file(draw->outfile.c_str());
  std::vector<Nuclide>::const_iterator nuc_it;

  if (out_file.is_open())
    {
      std::vector<float> partition_value;
      std::vector<std::string> partition_colour;
      std::vector<bool> draw_partition(12,false);

      createSVGProlog(draw,out_file);

      out_file << "<g transform=\"translate(" << 0.5*draw->size << "," << 0.5*draw->size << ") scale(" << draw->size << "," << draw->size << ")\">" << std::endl;

      //-Define what colours and values will be used to differentiate the nuclei.
      setColours(partition_colour,partition_value,draw);

      drawNuclei(in,partition_colour,partition_value,draw_partition,draw,out_file);

      out_file << "</g>\n</svg>" << std::endl;

      out_file.close();
    }
  else
    {
      std::cout << "\nERROR: Couldn't open " << draw->outfile << " to create the chart." << std::endl;
      exit(-1);
    }
}
