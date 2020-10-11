#include "inch/eps_chart.hpp"

#include <catch2/catch.hpp>


TEST_CASE("EPS setup", "[EPSChart]")
{
  Options options;
  const EPSChart theChart(options);

  const std::string setup{ "u dup scale\n"
                           "0.5 dup translate\n" };

  REQUIRE(theChart.setup() == setup);
}


TEST_CASE("EPS relative key setup", "[EPSChart]")
{
  Options options;
  const EPSChart theChart(options);
  std::string setup{};

  SECTION("Do use a relative key")
  {
    theChart.key_relative = true;
    theChart.height       = 5.0;

    setup = "\n%Shift coordinates so chart is vertically centered\n"
            "gs\n"
            "0 -3.5 translate\n";

    REQUIRE(theChart.KeySetup(10) == setup);
  }

  SECTION("Do NOT use a relative key")
  {
    theChart.key_relative = false;

    setup = "";

    REQUIRE(theChart.KeySetup(10) == setup);
  }
}


TEST_CASE("EPS relative key teardown", "[EPSChart]")
{
  Options options;
  const EPSChart theChart(options);
  std::string teardown{};

  SECTION("Relative key was not used")
  {
    theChart.key_relative = true;
    teardown              = "\n%Put coordinates back now that chart is drawn\n"
               "gr\n";

    REQUIRE(theChart.KeyTearDown() == teardown);
  }

  SECTION("Relative key was used")
  {
    theChart.key_relative = false;
    teardown              = "";

    REQUIRE(theChart.KeyTearDown() == teardown);
  }
}


TEST_CASE("EPS teardown", "[EPSChart]")
{
  Options options;
  const EPSChart theChart(options);
  theChart.size   = 2;
  theChart.height = 3.0;
  theChart.width  = 4.0;

  const std::string teardown{ "end grestore\n"
                              "\n"
                              "%%Trailer\n"
                              "%%BoundingBox: 0 0 8.0 6.0\n"
                              "%%EOF\n" };

  REQUIRE(theChart.teardown() == teardown);
}


TEST_CASE("EPS header", "[EPSChart]")
{
  Options options;
  const EPSChart theChart(options);

  const std::string header =
      fmt::format("%!PS-Adobe-3.0 EPSF-3.0\n"
                  "%%Title: Nuclear Chart - Error on mass-excess - Z=[0(n),118(Ei)] - N=[0,177]\n"
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
                  Chart::getTime());

  REQUIRE(theChart.header() == header);
}


TEST_CASE("EPS info comment", "[EPSChart]")
{
  Options options;
  const EPSChart theChart(options);

  const std::string comment{ "\n%==The different ways to draw an isotope=======\n"
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
                             "%==============================================\n\n" };

  REQUIRE(theChart.infoComment() == comment);
}


TEST_CASE("EPS definitions", "[EPSChart]")
{
  Options options;
  const EPSChart theChart(options);

  const std::string definitions{ "\n%%BeginProlog\n"
                                 "gsave 45 dict begin\n\n"
                                 "/bd {bind def} bind def\n"
                                 "/ld {load def} bd\n"
                                 "/u {4} bd\n"
                                 "/curve {0.25} bd\n"
                                 "/BoxWidth {0.5} bd\n"
                                 "/ed {exch def} bd\n"
                                 "/TR {/Times-Roman exch selectfont} bd\n"
                                 "/S {/Symbol exch selectfont} bd\n\n"
                                 "/gs /gsave ld\n"
                                 "/gr /grestore ld\n"
                                 "/m /moveto ld\n"
                                 "/l /lineto ld\n"
                                 "/rl /rlineto ld\n"
                                 "/st /stroke ld\n"
                                 "/sh /show ld\n"
                                 "/rgb /setrgbcolor ld\n"
                                 "/sl /setlinewidth ld\n\n"
                                 "/CenteredText{\n"
                                 "dup stringwidth pop\n"
                                 "-2 div 0\n"
                                 "rmoveto sh\n"
                                 "} bd\n\n"
                                 "/half{\n"
                                 "rotate\n"
                                 "black rgb\n"
                                 "BoxWidth neg 0 m\n"
                                 "0 BoxWidth neg rl\n"
                                 "1 0 rl\n"
                                 "0 BoxWidth rl\n"
                                 "fill\n"
                                 "} bd\n"
                                 "\n"
                                 "/wedge{\n"
                                 "rotate\n"
                                 "black rgb\n"
                                 "BoxWidth neg BoxWidth neg m\n"
                                 "1 0 rl\n"
                                 "0 1 rl\n"
                                 "fill\n"
                                 "} bd\n"
                                 "\n"
                                 "/hbhalf{ gs 0 half gr } bd\n"
                                 "\n"
                                 "/vrhalf{ gs 90 half gr } bd\n"
                                 "\n"
                                 "/hthalf{ gs 180 half gr } bd\n"
                                 "\n"
                                 "/vlhalf{ gs 270 half gr } bd\n"
                                 "\n"
                                 "/brwedge{ gs 0 wedge gr } bd\n"
                                 "\n"
                                 "/trwedge{ gs 90 wedge gr } bd\n"
                                 "\n"
                                 "/tlwedge{ gs 180 wedge gr } bd\n"
                                 "\n"
                                 "/blwedge{ gs 270 wedge gr } bd\n"
                                 "\n"
                                 "/Background{\n"
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
                                 "} bd\n"
                                 "\n"
                                 "/Foreground{\n"
                                 "gr\n"
                                 "0.5 u div setlinewidth\n"
                                 "black rgb\n"
                                 "st\n"
                                 "gr\n"
                                 "} bd\n"
                                 "\n"
                                 "/WriteIsotope{\n"
                                 "/num ed\n"
                                 "/sym ed\n"
                                 "3 3 roll rgb\n"
                                 "0.6 TR\n"
                                 "x 0.5 add y 0.2 add m\n"
                                 "sym 4 string cvs CenteredText\n"
                                 "0.35 TR\n"
                                 "x 0.5 add y 0.65 add m\n"
                                 "num 4 string cvs CenteredText\n"
                                 "} bd\n"
                                 "\n"
                                 "/Nucleus{\n"
                                 "/CornerCurve exch 2 div def\n"
                                 "/y ed\n"
                                 "/x ed\n"
                                 "/BackgroundBlue ed\n"
                                 "/BackgroundGreen ed\n"
                                 "/BackgroundRed ed\n"
                                 "count 1 eq\n"
                                 "{\n"
                                 "/h ed\n"
                                 "h 0 eq {Background Foreground} if\n"
                                 "h 1 eq {Background hbhalf Foreground} if\n"
                                 "h 2 eq {Background hthalf Foreground} if\n"
                                 "h 3 eq {Background vlhalf Foreground} if\n"
                                 "h 4 eq {Background vrhalf Foreground} if\n"
                                 "h 5 eq {Background trwedge Foreground} if\n"
                                 "h 6 eq {Background tlwedge Foreground} if\n"
                                 "h 7 eq {Background brwedge Foreground} if\n"
                                 "h 8 eq {Background blwedge Foreground} if\n"
                                 "h 8 gt {Background Foreground} if\n"
                                 "}\n"
                                 "{\n"
                                 "count 5 eq\n"
                                 "{Background Foreground WriteIsotope}\n"
                                 "{\n"
                                 "6 5 roll\n"
                                 "/h ed\n"
                                 "h 0 eq {Background Foreground WriteIsotope} if\n"
                                 "h 1 eq {Background hthalf Foreground WriteIsotope} if\n"
                                 "h 2 eq {Background hbhalf Foreground WriteIsotope} if\n"
                                 "h 3 eq {Background vlhalf Foreground WriteIsotope} if\n"
                                 "h 4 eq {Background vrhalf Foreground WriteIsotope} if\n"
                                 "h 5 eq {Background trwedge Foreground WriteIsotope} if\n"
                                 "h 6 eq {Background tlwedge Foreground WriteIsotope} if\n"
                                 "h 7 eq {Background brwedge Foreground WriteIsotope} if\n"
                                 "h 8 eq {Background blwedge Foreground WriteIsotope} if\n"
                                 "h 8 gt {Background Foreground WriteIsotope} if\n"
                                 "} ifelse\n"
                                 "} ifelse\n"
                                 "} bd\n\n"
                                 "/black     {0 0 0} bd\n"
                                 "/white     {1 1 1} bd\n"
                                 "/red       {1 0 0} bd\n"
                                 "/green     {0 1 0} bd\n"
                                 "/blue      {0 0 1} bd\n"
                                 "/yellow    {1 1 0} bd\n"
                                 "/magenta   {1 0 1} bd\n"
                                 "/cyan      {0 1 1} bd\n"
                                 "/orange    {1 0.6471 0} bd\n"
                                 "/darkgreen {0 0.3922 0} bd\n"
                                 "/navyblue  {0 0 0.5020} bd\n"
                                 "/purple    {0.6275 0.1255 0.9412} bd\n"
                                 "%%EndProlog\n" };

  REQUIRE(theChart.definitions() == definitions);
}
