#include "chart.hpp"

#include "chartColour.hpp"
#include "chartSelection.hpp"
#include "dripline.hpp"
#include "fileType.hpp"
#include "grid.hpp"
#include "key.hpp"
#include "magicNumbers.hpp"
#include "nuclide.hpp"
#include "options.hpp"
#include "prolog.hpp"
#include "rProcess.hpp"

#include <cmath>
#include <string>


void Chart::write(std::vector<Nuclide>& nuc, Options& draw, Partition& part) const
{
  std::cout << "\nCreating " << draw.outfile << "\n|--\n";

  switch (draw.filetype)
    {
      case FileType::EPS:
        writeEPS(nuc, draw, part);
        break;
      case FileType::SVG:
        writeSVG(nuc, draw);
        break;
      case FileType::TIKZ:
        writeTIKZ(nuc, draw);
        break;
    }

  std::cout << "--| done\n" << std::endl;
}


void Chart::setCanvasSize(const double key_scale, const double key_height, const Options& draw) const
{
  height = draw.Zmax - draw.Zmin + 2.0;

  if (key_height * key_scale > height)
    {
      key_relative = true;
      height       = (key_height + 1.0) * key_scale;
    }

  // HACKS
  // When all nuclei are drawn, key is in top left.
  // The below stops extra space being created on the right.
  //
  // max_key_width*scale extends the width to fit the widest key
  // This should really be set as a function of the variable
  // used to colour the isotopes. Either way, this cannot be
  // set dynamically in the file so we need to use 'magic numbers'
  width = draw.Nmax - draw.Nmin + 2;

  if (draw.chart_selection != ChartSelection::FULL_CHART && (draw.Zmax - draw.Zmin) < MAX_Z)
    {
      width += (max_key_width * key_scale);
    }
}


void Chart::drawNuclei(const std::vector<Nuclide>& in, const Options& draw, std::ostream& outFile) const
{
  for (const auto& it : in)
    {
      if (it.show == 1)
        {
          if (draw.filetype == FileType::EPS)
            {
              // Set how the shape representing the isotope is displayed
              const int isotope_display = [&]() {
                return (draw.chart_colour == ChartColour::FIRST_ISOMERENERGY && it.decay == "stable") ? 1
                                                                                                      : it.own ? 8 : 0;
              }();

              // Set the text, if it is displayed
              const std::string writing_colour = [&]() {
                if (draw.write_isotope)
                  {
                    std::string text_colour{ "black" };
                    // If the square is coloured black, change text colour to white
                    if (it.colour == "black")
                      {
                        // but if it's user defined, use red
                        text_colour = it.own ? " red" : " white";
                      }
                    else if (draw.chart_colour == ChartColour::FIRST_ISOMERENERGY && it.decay == "stable")
                      {
                        text_colour = "white";
                      }

                    return text_colour + " (" + it.symbol + ") (" + std::to_string(it.A) + ")";
                  }

                return std::string("");
              }();

              // Everything is set, draw the isotope
              // Add comment in the eps file with isotope ID
              // This is for easy navigation if manually altering the file
              outFile << "%" << it.A << it.symbol << "\n"
                      << isotope_display << " " << writing_colour << " " << it.colour << " " << it.N - draw.Nmin << " "
                      << it.Z - draw.Zmin << " curve Nucleus\n";
            }
          else if (draw.filetype == FileType::SVG)
            {
              outFile << "<!--" << it.A << it.symbol << "-->\n"
                      << R"(<g transform="translate()" << it.N - draw.Nmin << " " << draw.Zmax - it.Z << R"lit()"> )lit"
                      << R"(<use xlink:href="#)" << it.colour << R"(Nucleus"/></g>)" << std::endl;
              //<< "<text class=\"MidSymbol Black\" dx=\"0.5\" dy=\"0.80\">" << it.symbol << "</text> "
              //<< "<text class=\"MidNumber Black\" dx=\"0.5\" dy=\"0.35\">" << it.A << "</text></g>" << std::endl;
            }
          else if (draw.filetype == FileType::TIKZ)
            {
              outFile << "%" << it.A << it.symbol << "\n"
                      << R"(\nucleus{)" << it.colour << "}{" << it.N << "}{" << it.Z << "}{" << it.A << "}{"
                      << it.symbol << "}" << std::endl;
            }
        }
      else if (it.show == 2)
        {
          if (it.decay == "stable")
            {
              it.colour = "black";
            }

          if (draw.filetype == FileType::EPS)
            {
              // Set the text, if it is displayed
              const std::string writing_colour = [&]() {
                if (draw.write_isotope)
                  {
                    // If the square is coloured black change the text to white
                    // unless it a user isotope, in which case red
                    const std::string text_colour = (it.colour == "black") ? it.own ? "red" : "white" : "black";

                    return text_colour + " (" + it.symbol + ") (" + std::to_string(it.A) + ")";
                  }

                return std::string("");
              }();

              outFile << "%" << it.A << it.symbol << "\n"
                      << "0"
                      << " " << writing_colour << " " << it.colour << " " << it.N - draw.Nmin << " " << it.Z - draw.Zmin
                      << " curve Nucleus" << std::endl;
            }
          else if (draw.filetype == FileType::SVG)
            {
            }
          else if (draw.filetype == FileType::TIKZ)
            {
            }
        }
    }
}


void Chart::writeEPS(std::vector<Nuclide>& nuc, Options& draw, Partition& part) const
{
  // Open the output file we are going to use
  std::ofstream outFile(draw.outfile, std::ios::binary);

  if (!outFile)
    {
      std::cout << "\n"
                << "***ERROR***: Couldn't open " << draw.outfile << " to create the chart." << std::endl;
      return;
    }

  // Create and write the prolog, setting up definitions
  // and functions that will be used
  const Prolog setup(size);
  setup.EPSWriteProlog(outFile, draw);

  // Set the scale and an outer border of half a unit.
  outFile << "u dup scale\n"
          << "0.5 dup translate" << std::endl;

  // If key is taller than chart, shift chart to be centered in y.
  const Key theKey;
  theKey.setScale(draw, part);

  setCanvasSize(theKey.scale, theKey.height, draw);

  // For positioning and alignment,
  // draw a grid, default spacing is 5 units.
  if (draw.grid)
    {
      const Grid grid;
      grid.EPSDrawGrid(outFile, floor(width), floor(height));
    }

  if (key_relative)
    {
      outFile << "\n"
              << "%Shift coordinates so chart is vertically centered\n"
              << "gs\n"
              << "0 " << (height - (draw.Zmax - draw.Zmin + 2)) / 2 << " translate" << std::endl;
    }

  // r-process - shaded
  // Postscript doesn't support transparency so draw shaded
  // area of the r-process before nuclei and the outline after.
  //
  // We can't create the instance in the if condition below, as
  // it would then go out of scope and we would have to create it
  // again to draw the outline.
  const rProcess rProc(draw.Zmin, draw.Zmax, draw.Nmin, draw.Nmax);

  if (draw.r_process && draw.Zmax > rProc.min_Z)
    {
      rProc.setRProcessFile(draw.r_proc_path);
      rProc.readData();
      rProc.EPSWritePath(outFile, true);
    }

  //---------------
  //- Draw Nuclei -
  //---------------
  drawNuclei(nuc, draw, outFile);

  // Magic numbers
  if (draw.magic_numbers)
    {
      const MagicNumbers magic(draw.Zmin, draw.Zmax, draw.Nmin, draw.Nmax);

      magic.EPSSetup(outFile);

      for (const auto val : magic.numbers)
        {
          if (draw.Zmax >= val && draw.Zmin <= val)
            {
              magic.EPSWriteProtonNumber(outFile, val);
            }

          if (draw.Nmax >= val && draw.Nmin <= val)
            {
              magic.EPSWriteNeutronNumber(outFile, val);
            }
        }

      magic.EPSTearDown(outFile);
    }
  else
    {
      std::cout << "\nNot drawing the magic numbers" << std::endl;
    }

  // Drip lines
  // NUBASE has units of keV, we need MeV once we eventually use these values.
  // If we convert here then we can pass them as const
  if (draw.single_drip_lines > 0)
    {
      const std::string dripLineColour = { "purple" };
      if (draw.single_drip_lines != 2 && (draw.Nmax > 17 && draw.Zmax > 8))
        {
          const DripLine snDrip(nuc[0].NUBASE_ME / 1.0e3,
                                nuc[1].NUBASE_ME / 1.0e3,
                                draw.Zmin,
                                draw.Zmax,
                                draw.Nmin,
                                draw.Nmax,
                                LineType::singleneutron);

          snDrip.setDripLineFile(draw);
          snDrip.setDripLineColour(dripLineColour);
          snDrip.EPSWriteLine(outFile);
        }

      if (draw.single_drip_lines != 3 && (draw.Nmax > 8 && draw.Zmax > 11))
        {
          const DripLine spDrip(nuc[0].NUBASE_ME / 1.0e3,
                                nuc[1].NUBASE_ME / 1.0e3,
                                draw.Zmin,
                                draw.Zmax,
                                draw.Nmin,
                                draw.Nmax,
                                LineType::singleproton);

          spDrip.setDripLineFile(draw);
          spDrip.setDripLineColour(dripLineColour);
          spDrip.EPSWriteLine(outFile);
        }
    }
  else
    {
      std::cout << "Drawing neither of the single particle drip lines" << std::endl;
    }


  if (draw.double_drip_lines > 0)
    {
      const std::string dripLineColour = { "darkgreen" };
      if (draw.double_drip_lines != 2 && (draw.Nmax > 20 && draw.Zmax > 8))
        {
          const DripLine dnDrip(nuc[0].NUBASE_ME / 1.0e3,
                                nuc[1].NUBASE_ME / 1.0e3,
                                draw.Zmin,
                                draw.Zmax,
                                draw.Nmin,
                                draw.Nmax,
                                LineType::doubleneutron);

          dnDrip.setDripLineFile(draw);
          dnDrip.setDripLineColour(dripLineColour);
          dnDrip.EPSWriteLine(outFile);
        }

      if (draw.double_drip_lines != 3 && (draw.Nmax > 8 && draw.Zmax > 14))
        {
          const DripLine dpDrip(nuc[0].NUBASE_ME / 1.0e3,
                                nuc[1].NUBASE_ME / 1.0e3,
                                draw.Zmin,
                                draw.Zmax,
                                draw.Nmin,
                                draw.Nmax,
                                LineType::doubleproton);

          dpDrip.setDripLineFile(draw);
          dpDrip.setDripLineColour(dripLineColour);
          dpDrip.EPSWriteLine(outFile);
        }
    }
  else
    {
      std::cout << "Drawing neither of the double particle drip lines" << std::endl;
    }

  // r-process - outline
  if (draw.r_process && draw.Zmax > rProc.min_Z)
    {
      rProc.EPSWritePath(outFile, false);
    }
  else
    {
      std::cout << "Not drawing the r-process path" << std::endl;
    }

  if (key_relative)
    {
      outFile << "\n"
              << "%Put coordinates back now that chart is drawn\n"
              << "gr" << std::endl;
    }

  // Key
  if (draw.key)
    {
      theKey.EPSSetup(outFile);
      theKey.EPSPlaceKey(outFile, draw);
      theKey.EPSAdditionalFunctions(outFile, draw);
      theKey.EPSSetText(draw, part);
      theKey.EPSWrite(outFile, part);
      theKey.EPSSurroundingBox(outFile);
    }
  else
    {
      std::cout << "Not drawing the key" << std::endl;
    }

  // Reset the state and mark end of file
  // As we didn't know the full size during prolog, set it now
  outFile << "end grestore\n"
          << "\n"
          << "%%Trailer\n"
          << "%%BoundingBox: 0 0 " << ceil(width * size) << " " << ceil(height * size) << "\n"
          << "%%EOF" << std::endl;

  outFile.close();
}


void Chart::writeSVG(std::vector<Nuclide>& nuc, Options& draw) const
{
  std::ofstream outFile(draw.outfile, std::ios::binary);

  if (!outFile)
    {
      std::cout << "\n"
                << "***ERROR***: Couldn't open " << draw.outfile << " to create the chart." << std::endl;
      return;
    }

  const Prolog setup(size);
  setup.SVGWriteProlog(outFile, draw);

  outFile << R"(<g transform="translate()" << 0.5 * size << "," << 0.5 * size << ") scale(" << size << "," << size
          << R"lit()">)lit" << std::endl;

  drawNuclei(nuc, draw, outFile);

  outFile << "</g>\n</svg>" << std::endl;

  outFile.close();
}


void Chart::writeTIKZ(std::vector<Nuclide>& nuc, Options& draw) const
{
  std::ofstream outFile(draw.outfile, std::ios::binary);

  if (!outFile)
    {
      std::cout << "\n"
                << "***ERROR***: Couldn't open " << draw.outfile << " to create the chart." << std::endl;
      return;
    }

  const Prolog setup(size);
  setup.TIKZWriteProlog(outFile); //, draw);

  outFile << "\\begin{document}\n"
          << R"(\begin{tikzpicture})" << std::endl;

  drawNuclei(nuc, draw, outFile);

  outFile << "\\end{tikzpicture}\n"
          << R"(\end{document})" << std::endl;

  outFile.close();
}
