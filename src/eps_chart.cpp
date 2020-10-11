#include "inch/eps_chart.hpp"

#include "inch/eps_dripline.hpp"
#include "inch/eps_grid.hpp"
#include "inch/eps_key.hpp"
#include "inch/eps_magicNumbers.hpp"
#include "inch/eps_rProcess.hpp"
#include "inch/nuclide.hpp"
#include "inch/options.hpp"

#include <fmt/ostream.h>

void EPSChart::write(const std::vector<Nuclide>& nuc, const Partition& part) const
{
  fmt::print("\nCreating {} \n|--\n", options.outfile);

  // Open the output file we are going to use
  std::ofstream outFile(options.outfile, std::ios::binary);

  if (!outFile.is_open())
    {
      fmt::print("\n***ERROR***: Couldn't open {} to create the chart\n", options.outfile);
      return;
    }

  fmt::print(outFile, "{}", prolog());

  fmt::print(outFile, "{}", setup());

  // If key is taller than chart, shift chart to be centered in y.
  const EPSKey theKey;
  theKey.setScale(options, part);

  setCanvasSize(theKey.scale, theKey.height);

  if (options.grid)
    {
      const EPSGrid grid;
      fmt::print(
          outFile, "{}", grid.DrawGrid(static_cast<int>(std::floor(width)), static_cast<int>(std::floor(height)), 5));
    }

  fmt::print(outFile, "{}", KeySetup(options.limits.getZRange()));

  // r-process - shaded
  // Postscript doesn't support transparency so draw shaded
  // area of the r-process before nuclei and the outline after.
  //
  // We can't create the instance in the if condition below, as
  // it would then go out of scope and we would have to create it
  // again to draw the outline.
  const EPSrProcess rProc(options.limits);

  if (options.r_process && rProc.isZHighEnough())
    {
      rProc.readData();
      rProc.WritePath(outFile, true);
    }

  for (const auto& isotope : nuc)
    {
      if (isotope.show == 1 || isotope.show == 2)
        {
          fmt::print(outFile, "{}", isotope.writeAsEPS(options));
        }
    }

  // Magic numbers
  if (options.magic_numbers)
    {
      const EPSMagicNumbers magic(options.limits);

      fmt::print(outFile, "{}", magic.Setup());

      for (const auto val : magic.numbers)
        {
          if (options.limits.inZRange(val))
            {
              fmt::print(outFile, "{}", magic.WriteProtonNumber(val));
            }

          if (options.limits.inNRange(val))
            {
              fmt::print(outFile, "{}", magic.WriteNeutronNumber(val));
            }
        }

      fmt::print(outFile, "{}", magic.TearDown());
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
  if (options.single_drip_lines > 0)
    {
      const double neutronMass{ nuc[0].NUBASE_ME / 1.0e3 };
      const double protonMass{ nuc[1].NUBASE_ME / 1.0e3 };
      const std::string_view dripLineColour{ "purple" };

      if (options.single_drip_lines != 2)
        {
          drip_lines.emplace_back(
              EPSDripLine(neutronMass, protonMass, options.limits, LineType::singleneutron, dripLineColour));
        }

      if (options.single_drip_lines != 3)
        {
          drip_lines.emplace_back(
              EPSDripLine(neutronMass, protonMass, options.limits, LineType::singleproton, dripLineColour));
        }
    }
  else
    {
      fmt::print("Drawing neither of the single particle drip lines");
    }

  // Double particle drip lines
  if (options.double_drip_lines > 0)
    {
      const double neutronMass{ nuc[0].NUBASE_ME / 1.0e3 };
      const double protonMass{ nuc[1].NUBASE_ME / 1.0e3 };
      const std::string_view dripLineColour{ "darkgreen" };

      if (options.double_drip_lines != 2)
        {
          drip_lines.emplace_back(
              EPSDripLine(neutronMass, protonMass, options.limits, LineType::doubleneutron, dripLineColour));
        }

      if (options.double_drip_lines != 3)
        {
          drip_lines.emplace_back(
              EPSDripLine(neutronMass, protonMass, options.limits, LineType::doubleproton, dripLineColour));
        }
    }
  else
    {
      fmt::print("Drawing neither of the double particle drip lines");
    }

  // Loop through the lines that should be drawn and do the necessary to draw them
  for (const auto& dl : drip_lines)
    {
      if (dl.areNmaxAndZmaxValuesHighEnough())
        {
          dl.WriteLine(outFile);
        }
      else
        {
          fmt::print("**WARNING**: One or both of Nmax ({}) or Zmax ({}) are lower than the initial drip line value\n",
                     dl.limits.Nmax,
                     dl.limits.Zmax);
        }
    }

  // r-process - outline
  if (options.r_process && rProc.isZHighEnough())
    {
      rProc.WritePath(outFile, false);
    }
  else
    {
      fmt::print("Not drawing the r-process path");
    }

  fmt::print(outFile, "{}", KeyTearDown());

  // Key
  if (options.key)
    {
      fmt::print("Drawing the key ");

      fmt::print(outFile, "{}{}", theKey.Setup(), theKey.PlaceKey(options));

      if (options.chart_colour == ChartColour::REL_MASSEXCESSERROR || options.chart_colour == ChartColour::GS_HALFLIFE)
        {
          fmt::print(outFile, "{}", theKey.AdditionalFunctions(options.chart_colour));
        }

      theKey.Write(outFile, part);

      fmt::print(outFile, "{}", theKey.SurroundingBox());

      fmt::print("- done");
    }
  else
    {
      fmt::print("Not drawing the key");
    }

  fmt::print(outFile, "{}", teardown());

  fmt::print("\n--| done\n");
}


std::string EPSChart::header() const
{
  return fmt::format("%!PS-Adobe-3.0 EPSF-3.0\n"
                     "%%Title: Nuclear Chart - {} - Z=[{}({}),{}({})] - N=[{},{}]\n"
                     "%%BoundingBox: (atend)\n"
                     "%%Creator: The Interactive Nuclear CHart (INCH)\n"
                     "%%CreationDate: {}\n"
                     "%%DocumentFonts: Times-Roman Symbol\n"
                     "%%Page: 1\n"
                     "%%EndComments\n"
                     "\n"
                     "systemdict /setdistillerparams known {{\n"
                     "<< /AutoFilterColorImages false /ColorImageFilter /FlateEncode >>\n"
                     "setdistillerparams }} if\n",
                     getTitle(options.chart_colour),
                     options.limits.Zmin,
                     Converter::ZToSymbol(options.limits.Zmin),
                     options.limits.Zmax,
                     Converter::ZToSymbol(options.limits.Zmax),
                     options.limits.Nmin,
                     options.limits.Nmax,
                     getTime());
}


std::string EPSChart::definitions() const
{
  return fmt::format("\n%%BeginProlog\n"
                     "gsave 45 dict begin\n\n"
                     "/bd {{bind def}} bind def\n"
                     "/ld {{load def}} bd\n"
                     "/u {{{}}} bd\n"
                     "/curve {{{}}} bd\n"
                     "/BoxWidth {{0.5}} bd\n"
                     "/ed {{exch def}} bd\n"
                     "/TR {{/Times-Roman exch selectfont}} bd\n"
                     "/S {{/Symbol exch selectfont}} bd\n\n"
                     "/gs /gsave ld\n"
                     "/gr /grestore ld\n"
                     "/m /moveto ld\n"
                     "/l /lineto ld\n"
                     "/rl /rlineto ld\n"
                     "/st /stroke ld\n"
                     "/sh /show ld\n"
                     "/rgb /setrgbcolor ld\n"
                     "/sl /setlinewidth ld\n\n"
                     "/CenteredText{{\n"
                     "dup stringwidth pop\n"
                     "-2 div 0\n"
                     "rmoveto sh\n"
                     "}} bd\n\n"
                     "/half{{\n"
                     "rotate\n"
                     "black rgb\n"
                     "BoxWidth neg 0 m\n"
                     "0 BoxWidth neg rl\n"
                     "1 0 rl\n"
                     "0 BoxWidth rl\n"
                     "fill\n"
                     "}} bd\n"
                     "\n"
                     "/wedge{{\n"
                     "rotate\n"
                     "black rgb\n"
                     "BoxWidth neg BoxWidth neg m\n"
                     "1 0 rl\n"
                     "0 1 rl\n"
                     "fill\n"
                     "}} bd\n"
                     "\n"
                     "/hbhalf{{ gs 0 half gr }} bd\n"
                     "\n"
                     "/vrhalf{{ gs 90 half gr }} bd\n"
                     "\n"
                     "/hthalf{{ gs 180 half gr }} bd\n"
                     "\n"
                     "/vlhalf{{ gs 270 half gr }} bd\n"
                     "\n"
                     "/brwedge{{ gs 0 wedge gr }} bd\n"
                     "\n"
                     "/trwedge{{ gs 90 wedge gr }} bd\n"
                     "\n"
                     "/tlwedge{{ gs 180 wedge gr }} bd\n"
                     "\n"
                     "/blwedge{{ gs 270 wedge gr }} bd\n"
                     "\n"
                     "/Background{{\n"
                     "gs\n"
                     "x BoxWidth add y BoxWidth add translate\n"
                     "0 BoxWidth m\n"
                     "BoxWidth neg BoxWidth BoxWidth neg 0 CornerCurve arct\n"
                     "BoxWidth neg BoxWidth neg 0 BoxWidth neg CornerCurve arct\n"
                     "BoxWidth BoxWidth neg BoxWidth 0 CornerCurve arct\n"
                     "BoxWidth BoxWidth 0 BoxWidth CornerCurve arct\n"
                     "closepath clip\n"
                     "gs\n"
                     "BackgroundRed BackgroundGreen BackgroundBlue rgb\n"
                     "fill\n"
                     "}} bd\n"
                     "\n"
                     "/Foreground{{\n"
                     "gr\n"
                     "0.5 u div setlinewidth\n"
                     "black rgb\n"
                     "st\n"
                     "gr\n"
                     "}} bd\n"
                     "\n"
                     "/WriteIsotope{{\n"
                     "/num ed\n"
                     "/sym ed\n"
                     "3 3 roll rgb\n"
                     "0.6 TR\n"
                     "x 0.5 add y 0.2 add m\n"
                     "sym 4 string cvs CenteredText\n"
                     "0.35 TR\n"
                     "x 0.5 add y 0.65 add m\n"
                     "num 4 string cvs CenteredText\n"
                     "}} bd\n"
                     "\n"
                     "/Nucleus{{\n"
                     "/CornerCurve exch 2 div def\n"
                     "/y ed\n"
                     "/x ed\n"
                     "/BackgroundBlue ed\n"
                     "/BackgroundGreen ed\n"
                     "/BackgroundRed ed\n"
                     "count 1 eq\n"
                     "{{\n"
                     "/h ed\n"
                     "h 0 eq {{Background Foreground}} if\n"
                     "h 1 eq {{Background hbhalf Foreground}} if\n"
                     "h 2 eq {{Background hthalf Foreground}} if\n"
                     "h 3 eq {{Background vlhalf Foreground}} if\n"
                     "h 4 eq {{Background vrhalf Foreground}} if\n"
                     "h 5 eq {{Background trwedge Foreground}} if\n"
                     "h 6 eq {{Background tlwedge Foreground}} if\n"
                     "h 7 eq {{Background brwedge Foreground}} if\n"
                     "h 8 eq {{Background blwedge Foreground}} if\n"
                     "h 8 gt {{Background Foreground}} if\n"
                     "}}\n"
                     "{{\n"
                     "count 5 eq\n"
                     "{{Background Foreground WriteIsotope}}\n"
                     "{{\n"
                     "6 5 roll\n"
                     "/h ed\n"
                     "h 0 eq {{Background Foreground WriteIsotope}} if\n"
                     "h 1 eq {{Background hthalf Foreground WriteIsotope}} if\n"
                     "h 2 eq {{Background hbhalf Foreground WriteIsotope}} if\n"
                     "h 3 eq {{Background vlhalf Foreground WriteIsotope}} if\n"
                     "h 4 eq {{Background vrhalf Foreground WriteIsotope}} if\n"
                     "h 5 eq {{Background trwedge Foreground WriteIsotope}} if\n"
                     "h 6 eq {{Background tlwedge Foreground WriteIsotope}} if\n"
                     "h 7 eq {{Background brwedge Foreground WriteIsotope}} if\n"
                     "h 8 eq {{Background blwedge Foreground WriteIsotope}} if\n"
                     "h 8 gt {{Background Foreground WriteIsotope}} if\n"
                     "}} ifelse\n"
                     "}} ifelse\n"
                     "}} bd\n\n"
                     "/black     {{0 0 0}} bd\n"
                     "/white     {{1 1 1}} bd\n"
                     "/red       {{1 0 0}} bd\n"
                     "/green     {{0 1 0}} bd\n"
                     "/blue      {{0 0 1}} bd\n"
                     "/yellow    {{1 1 0}} bd\n"
                     "/magenta   {{1 0 1}} bd\n"
                     "/cyan      {{0 1 1}} bd\n"
                     "/orange    {{1 0.6471 0}} bd\n"
                     "/darkgreen {{0 0.3922 0}} bd\n"
                     "/navyblue  {{0 0 0.5020}} bd\n"
                     "/purple    {{0.6275 0.1255 0.9412}} bd\n"
                     "%%EndProlog\n",
                     size,
                     curve);
}


std::string EPSChart::infoComment() const
{
  // Watch for matching { characters
  return fmt::format("\n%==The different ways to draw an isotope=======\n"
                     "%\n"
                     "%\n"
                     "% Empty box\n"
                     "% ---------\n"
                     "% box_colour x y n\n"
                     "%\n"
                     "%\n"
                     "% Box with the nuclei written inside\n"
                     "% ----------------------------------\n"
                     "% writing_colour (El) (num) box x y n\n"
                     "%\n"
                     "%\n"
                     "% Empty box with half of it black\n"
                     "% -------------------------------\n"
                     "% x box_colour x y n\n"
                     "%\n"
                     "%\n"
                     "% One black half with the nuclei written inside\n"
                     "% ---------------------------------------------\n"
                     "% x writing_colour (El) (num)  box_colour x y n\n"
                     "%\n"
                     "% Where x can take the values below to alter the\n"
                     "% square as shown (x=0 creates a blank square).\n"
                     "%\n"
                     "% x    1      2      3      4\n"
                     "%     ___    ___    ___    ___\n"
                     "%    |###|  |   |  |#| |  | |#|\n"
                     "%    |---|  |---|  |#| |  | |#|\n"
                     "%    |___|  |###|  |#|_|  |_|#|\n"
                     "%\n"
                     "% x    5      6      7      8\n"
                     "%     ___    ___    ___    ___\n"
                     "%    |\\##|  |##/|  |  /|  |\\  |\n"
                     "%    | \\#|  |#/ |  | /#|  |#\\ |\n"
                     "%    |__\\|  |/__|  |/##|  |##\\|\n"
                     "%\n"
                     "%==============================================\n\n");
}


std::string EPSChart::prolog() const
{
  return header() + definitions() + infoComment();
}


std::string EPSChart::setup() const
{
  return fmt::format("u dup scale\n"
                     "0.5 dup translate\n");
}


std::string EPSChart::teardown() const
{
  return fmt::format("end grestore\n"
                     "\n"
                     "%%Trailer\n"
                     "%%BoundingBox: 0 0 {0:0.1f} {1:0.1f}\n"
                     "%%EOF\n",
                     std::ceil(width * size),
                     std::ceil(height * size));
}


std::string EPSChart::KeySetup(const int ZRange) const
{
  return (key_relative) ? fmt::format("\n%Shift coordinates so chart is vertically centered\n"
                                      "gs\n"
                                      "0 {} translate\n",
                                      0.5 * (height - (ZRange + 2 * BORDER)))
                        : "";
}


std::string EPSChart::KeyTearDown() const
{
  return (key_relative) ? fmt::format("\n%Put coordinates back now that chart is drawn\n"
                                      "gr\n")
                        : "";
}
