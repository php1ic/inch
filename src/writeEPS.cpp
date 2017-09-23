#include "functions.h"

void writeEPS(std::vector<Nuclide> &nuc,
              std::unique_ptr<inputs> &draw,
              std::unique_ptr<partition> &part
              )
{
  //-Open the output file to write to
  std::ofstream outFile(draw->outfile.c_str());

  if ( !outFile )
    {
      std::cout << "\n"
                << "***ERROR***: Couldn't open " << draw->outfile
                << " to create the chart." << std::endl;
      return;
    }

  //-Set up eps header and definitions
  createEPSProlog(draw,outFile);

  //-Set the scale and a border of half a unit.
  outFile << "u dup scale\n"
          << "0.5 dup translate" << std::endl;

  //-For positioning and alignment, draw a grid with spacings of 5 units.
  if ( draw->grid )
    {
      drawEPSGrid(draw,outFile);
    }

  //-If key is taller than chart, shift chart to be centered in y.
  if ( draw->key_relative )
    {
      outFile << "\n"
              << "%Shift coordinates so chart is vertically centered\n"
              <<"gs\n"
              << "0 " << (draw->chart_height-(draw->Zmax-draw->Zmin+2))/2 << " translate" << std::endl;
    }

  //-Postscript doesn't support transparency, thus draw shaded
  //-area of the r-process before nuclei and the outline after.
  //-----------------------------
  //- r-process -- shaded path --
  //-----------------------------
  if ( draw->r_process )
    {
      drawEPSRprocess(draw,outFile,true);
    }

  //---------------
  //- Draw Nuclei -
  //---------------
  drawNuclei(nuc,draw,outFile);

  //-----------------
  //- Magic numbers -
  //-----------------
  if ( draw->magic_numbers )
    {
      drawEPSMagicNumbers(draw,outFile);
    }
  else
    {
      std::cout << "\nNot drawing the magic numbers" << std::endl;
    }

  //--------------
  //- Drip lines -
  //--------------
  //NUBASE has units of keV, we need MeV once we eventually use these values.
  //If we convert here then we can pass them as const
  if ( draw->single_drip_lines > 0 )
    {
      drawEPSSingleDriplines(nuc[0].NUBASE_ME/1.0e3,nuc[1].NUBASE_ME/1.0e3,draw,outFile);
    }
  else
    {
      std::cout << "Drawing neither of the single particle drip lines" << std::endl;
    }

  if ( draw->double_drip_lines > 0 )
    {
      drawEPSDoubleDriplines(nuc[0].NUBASE_ME/1.0e3,nuc[1].NUBASE_ME/1.0e3,draw,outFile);
    }
  else
    {
      std::cout << "Drawing neither of the double particle drip lines" << std::endl;
    }

  //-----------------------------
  //- r-process -- path outline -
  //-----------------------------
  if ( draw->r_process )
    {
      drawEPSRprocess(draw,outFile,false);
    }
  else
    {
      std::cout << "Not drawing the r-process path" << std::endl;
    }

  if ( draw->key_relative )
    {
      outFile << "\n"
              << "%Put coordinates back now that chart is drawn\n"
              << "gr" << std::endl;
    }

  //-------
  //- Key -
  //-------
  if ( draw->key )
    {
      drawEPSKey(draw,outFile,part);
    }
  else
    {
      std::cout << "Not drawing the key" << std::endl;
    }

  outFile << "end grestore\n\n"
          << "%%EOF" << std::endl;

  outFile.close();
}
