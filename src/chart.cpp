#include "inch/chart.hpp"

#include "inch/chartColour.hpp"
#include "inch/chartSelection.hpp"
#include "inch/dripline.hpp"
#include "inch/fileType.hpp"
#include "inch/grid.hpp"
#include "inch/key.hpp"
#include "inch/limits.hpp"
#include "inch/magicNumbers.hpp"
#include "inch/nuclide.hpp"
#include "inch/options.hpp"
#include "inch/prolog.hpp"
#include "inch/rProcess.hpp"

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include <cmath>
#include <string>


void Chart::write(const std::vector<Nuclide>& nuc, const Options& draw, const Partition& part) const
{
  fmt::print("\nCreating {} \n|--\n", draw.outfile);

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

  fmt::print("\n--| done\n");
}


void Chart::drawNuclei(const std::vector<Nuclide>& in, const Options& draw, std::ostream& outFile) const
{
  for (const auto& it : in)
    {
      if (it.show == 1 || it.show == 2)
        {
          if (draw.filetype == FileType::EPS)
            {
              fmt::print(outFile, "{}", it.writeAsEPS(draw));
            }
          else if (draw.filetype == FileType::SVG)
            {
              fmt::print(outFile, "{}", it.writeAsSVG(draw.limits.Nmin, draw.limits.Zmax));
            }
          else if (draw.filetype == FileType::TIKZ)
            {
              fmt::print(outFile, "{}", it.writeAsTIKZ());
            }
        }
    }
}


void Chart::writeEPS(const std::vector<Nuclide>& nuc, const Options& draw, const Partition& part) const
{
  // Open the output file we are going to use
  std::ofstream outFile(draw.outfile, std::ios::binary);

  if (!outFile.is_open())
    {
      fmt::print("\n***ERROR***: Couldn't open {} to create the chart\n", draw.outfile);
      return;
    }

  // Create and write the prolog, setting up definitions
  // and functions that will be used
  const Prolog setup(size, draw.limits);
  fmt::print(outFile, "{}", setup.EPSWriteProlog(draw.chart_colour));

  // Set the scale and an outer border of half a unit.
  fmt::print(outFile, "{}", EPSsetup());

  // If key is taller than chart, shift chart to be centered in y.
  const Key theKey;
  theKey.setScale(draw, part);

  setCanvasSize(theKey.scale, theKey.height, draw);

  // For positioning and alignment,
  // draw a grid, default spacing is 5 units.
  if (draw.grid)
    {
      fmt::print(
          outFile, "{}", Grid::EPSDrawGrid(static_cast<int>(std::floor(width)), static_cast<int>(std::floor(height))));
    }

  fmt::print(outFile, "{}", EPSRelativeKeySetup(draw.limits.getZRange()));

  // r-process - shaded
  // Postscript doesn't support transparency so draw shaded
  // area of the r-process before nuclei and the outline after.
  //
  // We can't create the instance in the if condition below, as
  // it would then go out of scope and we would have to create it
  // again to draw the outline.
  const rProcess rProc(draw.limits);

  if (draw.r_process && rProc.isZHighEnough())
    {
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
      const MagicNumbers magic(draw.limits);

      fmt::print(outFile, "{}", magic.EPSSetup());

      for (const auto val : magic.numbers)
        {
          if (draw.limits.inZRange(val))
            {
              fmt::print(outFile, "{}", magic.EPSWriteProtonNumber(val));
            }

          if (draw.limits.inNRange(val))
            {
              fmt::print(outFile, "{}", magic.EPSWriteNeutronNumber(val));
            }
        }

      fmt::print(outFile, "{}", magic.EPSTearDown());
    }
  else
    {
      fmt::print("\nNot drawing the magic numbers");
    }

  // Drip lines
  // NUBASE has units of keV, we need MeV once we eventually use these values.
  // If we convert here then we can pass them as const

  // We are going to process each of the drip lines identically
  // Store the ones that will be drawn in a vector then loop that vector and perform the actions
  // Single particla drip lines
  if (draw.single_drip_lines > 0)
    {
      const double neutronMass{ nuc[0].NUBASE_ME / 1.0e3 };
      const double protonMass{ nuc[1].NUBASE_ME / 1.0e3 };
      const std::string_view dripLineColour{ "purple" };

      if (draw.single_drip_lines != 2)
        {
          drip_lines.emplace_back(
              DripLine(neutronMass, protonMass, draw.limits, LineType::singleneutron, dripLineColour));
        }

      if (draw.single_drip_lines != 3)
        {
          drip_lines.emplace_back(
              DripLine(neutronMass, protonMass, draw.limits, LineType::singleproton, dripLineColour));
        }
    }
  else
    {
      fmt::print("Drawing neither of the single particle drip lines");
    }

  // Double particle drip lines
  if (draw.double_drip_lines > 0)
    {
      const double neutronMass{ nuc[0].NUBASE_ME / 1.0e3 };
      const double protonMass{ nuc[1].NUBASE_ME / 1.0e3 };
      const std::string_view dripLineColour{ "darkgreen" };

      if (draw.double_drip_lines != 2)
        {
          drip_lines.emplace_back(
              DripLine(neutronMass, protonMass, draw.limits, LineType::doubleneutron, dripLineColour));
        }

      if (draw.double_drip_lines != 3)
        {
          drip_lines.emplace_back(
              DripLine(neutronMass, protonMass, draw.limits, LineType::doubleproton, dripLineColour));
        }
    }
  else
    {
      fmt::print("Drawing neither of the double particle drip lines");
    }

  // Loop through the lines that should be drawn and do the necessary to draw them
  for (const auto& dl : drip_lines)
    {
      if (dl.areMaxNandZmaxValuesHighEnough())
        {
          dl.EPSWriteLine(outFile);
        }
      else
        {
          fmt::print("**WARNING**: One or both of Nmax ({}) or Zmax ({}) are lower than the initial drip line value\n",
                     dl.limits.Nmax,
                     dl.limits.Zmax);
        }
    }


  // r-process - outline
  if (draw.r_process && rProc.isZHighEnough())
    {
      rProc.EPSWritePath(outFile, false);
    }
  else
    {
      fmt::print("Not drawing the r-process path");
    }

  fmt::print(outFile, "{}", EPSRelativeKeyTearDown());

  // Key
  if (draw.key)
    {
      fmt::print("Drawing the key ");

      fmt::print(outFile, "{}{}", theKey.EPSSetup(), theKey.EPSPlaceKey(draw));

      if (draw.chart_colour == ChartColour::REL_MASSEXCESSERROR || draw.chart_colour == ChartColour::GS_HALFLIFE)
        {
          fmt::print(outFile, "{}", theKey.EPSAdditionalFunctions(draw.chart_colour));
        }

      theKey.EPSSetText(part);
      theKey.EPSWrite(outFile, part);

      fmt::print(outFile, "{}", theKey.EPSSurroundingBox());

      fmt::print("- done");
    }
  else
    {
      fmt::print("Not drawing the key");
    }

  // Reset the state and mark end of file
  // As we didn't know the full size during prolog, set it now
  fmt::print(outFile, "{}", EPSteardown());

  outFile.close();
}


void Chart::writeSVG(const std::vector<Nuclide>& nuc, const Options& draw) const
{
  std::ofstream outFile(draw.outfile, std::ios::binary);

  if (!outFile.is_open())
    {
      fmt::print("\n***ERROR***: Couldn't open {} to create the chart.\n", draw.outfile);
      return;
    }

  const Prolog setup(size, draw.limits);
  fmt::print(outFile, "{}", setup.SVGWriteProlog());

  fmt::print(outFile, "<g transform=\"translate({0},{0}) scale({1},{1})\">\n", 0.5 * size, size);

  drawNuclei(nuc, draw, outFile);

  fmt::print(outFile,
             "</g>\n"
             "</svg>\n");

  outFile.close();
}


void Chart::writeTIKZ(const std::vector<Nuclide>& nuc, const Options& draw) const
{
  std::ofstream outFile(draw.outfile, std::ios::binary);

  if (!outFile.is_open())
    {
      fmt::print("\n***ERROR***: Couldn't open {} to create the chart.\n", draw.outfile);
      return;
    }

  const Prolog setup(size, draw.limits);
  fmt::print(outFile, "{}", setup.TIKZWriteProlog());

  fmt::print(outFile,
             "\\begin{{document}}\n"
             "\\begin{{tikzpicture}}\n");

  drawNuclei(nuc, draw, outFile);

  fmt::print(outFile,
             "\\end{{tikzpicture}}\n"
             "\\end{{document}}\n");

  outFile.close();
}
