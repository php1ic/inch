#include "include/functions.h"

void writeEPS(std::vector<Nuclide> &in,
	      inputs *draw,
	      partition *part
	      )
{
  //-Open the output file to write to
  std::ofstream out_file(draw->outfile.c_str());

  if (out_file.is_open())
    {
      //-Set up eps header and definitions
      createEPSProlog(draw,out_file);

      //-Set the scale and a border of half a unit.
      out_file << "u dup scale\n"
	       << "0.5 dup translate" << std::endl;

      //-For positioning and alignment, draw a grid with spacings of 5 units.
      if (draw->grid)
	drawEPSGrid(draw,out_file);

      //-If key is taller than chart, shift chart to be centered in y.
      if (draw->key_relative)
	out_file << "gs\n"
		 << "0 " << (draw->chart_height-(draw->Zmax-draw->Zmin+2))/2 << " translate" << std::endl;

      //-Postscript doesn't support transparency, thus draw shaded
      //-area of the r-process before nuclei and the outline after.
      //-----------------------------
      //- r-process -- shaded path --
      //-----------------------------
      if (draw->r_process)
	drawEPSRprocess(draw,out_file,1);

      //---------------
      //- Draw Nuclei -
      //---------------
      drawNuclei(in,draw,out_file);

      //-----------------
      //- Magic numbers -
      //-----------------
      if (draw->magic_numbers)
	drawEPSMagicNumbers(draw,out_file);
      else
	std::cout << "\nNot drawing the magic numbers" << std::endl;

      //--------------
      //- Drip lines -
      //--------------
      if (draw->single_drip_lines > 0)
	drawEPSSingleDriplines(in,draw,out_file);
      else
	std::cout << "Drawing neither of the single particle drip lines" << std::endl;

      if (draw->double_drip_lines > 0)
	drawEPSDoubleDriplines(in,draw,out_file);
      else
	std::cout << "Drawing neither of the double particle drip lines" << std::endl;

      //-----------------------------
      //- r-process -- path outline -
      //-----------------------------
      if (draw->r_process)
	drawEPSRprocess(draw,out_file,0);
      else
	std::cout << "Not drawing the r-process path" << std::endl;

      if (draw->key_relative)
	out_file << "gr" << std::endl;

      //-------
      //- Key -
      //-------
      if (draw->key)
	drawEPSKey(draw,out_file,part);
      else
	std::cout << "Not drawing the key" << std::endl;

      out_file << "end grestore\n\n"
	       << "%%EOF" << std::endl;

      out_file.close();
    }
  else
    {
      std::cout << "\nERROR: Couldn't open " << draw->outfile << " to create the chart." << std::endl;
      exit(-1);
    }
}
