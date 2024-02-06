#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER

#include "inch/nuclide.hpp"

#include <catch2/catch_all.hpp>


// TEST_CASE("", "[Nuclide]")
//{
//}


TEST_CASE("Set Symbol", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  nubase_gs03_isotope.setSymbol("Pb");
  REQUIRE(nubase_gs03_isotope.symbol == "Pb");
}


TEST_CASE("Set State", "[Nuclide]")
{
  // Start with non sensical value to make sure it is actually changed
  int state{ 55 };
  SECTION("Ground state")
  {
    const std::string nubase_gs03{
      "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
    };
    Nuclide nubase_gs03_isotope(nubase_gs03);

    nubase_gs03_isotope.setState(state);
    REQUIRE(state == 0);
  }

  SECTION("First Isomeric state")
  {
    const std::string nubase_isomer03{
      "010 0031   10Lim   33250       40       200      40     RQ     3.7   zs 1.5    1+               97Zi04t   IT=100"
    };
    Nuclide nubase_isomer03_isotope(nubase_isomer03);

    auto anotherIsomer = std::move(nubase_isomer03_isotope);
    anotherIsomer.setState(state);
    REQUIRE(state == 1);
  }
}


TEST_CASE("Is the isotope drawn", "[Nuclide]")
{
  Limits limits;
  Options options;

  const std::string nubase_isomer03{
    "010 0031   10Lim   33250       40       200      40     RQ     3.7   zs 1.5    1+               97Zi04t   IT=100"
  };
  Nuclide nubase_isomer03_isotope(nubase_isomer03);

  SECTION("Isotope is shown by default")
  {
    REQUIRE(nubase_isomer03_isotope.isShown(options));
  }

  options.chart_type = ChartType::EXPERIMENTAL;

  SECTION("Wrong ChartType")
  {
    REQUIRE_FALSE(nubase_isomer03_isotope.isShown(options));
  }
}


TEST_CASE("State object is created correctly", "[Nuclide]")
{
  auto level = Nuclide::State(0, 1.2345, 0.321);

  REQUIRE(level.level == 0);
  REQUIRE(level.energy == Catch::Approx(1.2345));
  REQUIRE(level.error == Catch::Approx(0.321));
}


TEST_CASE("Set IsomerEnergy", "[Nuclide]")
{
  const std::string nubase_isomer03{
    "010 0031   10Lim   33250       40       200      40     RQ     3.7   zs 1.5    1+               97Zi04t   IT=100"
  };
  Nuclide nubase_isomer03_isotope(nubase_isomer03);

  double value{ 0.0 };
  nubase_isomer03_isotope.setIsomerEnergy(value);

  REQUIRE(value == Catch::Approx(200.0));
}


TEST_CASE("Set IsomerEnergyError", "[Nuclide]")
{
  const std::string nubase_isomer03{
    "010 0031   10Lim   33250       40       200      40     RQ     3.7   zs 1.5    1+               97Zi04t   IT=100"
  };
  Nuclide nubase_isomer03_isotope(nubase_isomer03);

  double value{ 0.0 };
  nubase_isomer03_isotope.setIsomerEnergyError(value);

  REQUIRE(value == Catch::Approx(40.0));
}


TEST_CASE("Read and set measured or theoretical", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  const std::string nubase_theoretical03{
    "014 0090   14F     32660#     400#                                             2-#                        p ?"
  };
  Nuclide nubase_theoretical03_isotope(nubase_theoretical03);

  nubase_gs03_isotope.setExperimental();
  REQUIRE(nubase_gs03_isotope.exp == 1);
  nubase_gs03_isotope.setExperimental(0);
  REQUIRE(nubase_gs03_isotope.exp == 0);

  nubase_theoretical03_isotope.setExperimental();
  REQUIRE(nubase_theoretical03_isotope.exp == 0);
  nubase_theoretical03_isotope.setExperimental(1);
  REQUIRE(nubase_theoretical03_isotope.exp == 1);
}


TEST_CASE("Validate error calculations", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  REQUIRE(nubase_gs03_isotope.errorQuadrature(1.0, 0.0) == Catch::Approx(1.0));
  REQUIRE(nubase_gs03_isotope.errorQuadrature(3.0, 4.0) == Catch::Approx(5.0));
  REQUIRE(nubase_gs03_isotope.errorQuadrature(2.0, 2.0) == Catch::Approx(2.828427125));

  REQUIRE(nubase_gs03_isotope.errorQuadrature(1.0, 2.0, 3.0) == Catch::Approx(3.741657));

  REQUIRE(nubase_gs03_isotope.errorQuadrature(1.0, 1.0, 2.0, 5.0) == Catch::Approx(5.567764));
  REQUIRE(nubase_gs03_isotope.errorQuadrature(5.0, 6.0, 7.0, 8.0) == Catch::Approx(std::sqrt(174)));
}


TEST_CASE("Read spin parity of the state", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  const std::string nubase_isomer03{
    "010 0031   10Lim   33250       40       200      40     RQ     3.7   zs 1.5    1+               97Zi04t   IT=100"
  };
  Nuclide nubase_isomer03_isotope(nubase_isomer03);

  nubase_gs03_isotope.setSpinParity();

  REQUIRE(nubase_gs03_isotope.J == Catch::Approx(1.0));
  REQUIRE(nubase_gs03_isotope.pi == 1);
  REQUIRE(nubase_gs03_isotope.J_tent == 1);

  nubase_isomer03_isotope.setSpinParity();

  REQUIRE(nubase_isomer03_isotope.J == Catch::Approx(1.0));
  REQUIRE(nubase_isomer03_isotope.pi == 0);
  REQUIRE(nubase_isomer03_isotope.J_tent == 0);
}


TEST_CASE("Set 'own' flag", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  nubase_gs03_isotope.setOwn(true);
  REQUIRE(nubase_gs03_isotope.own == true);

  nubase_gs03_isotope.setOwn(false);
  REQUIRE(nubase_gs03_isotope.own == false);
}


TEST_CASE("Read half-life value", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  nubase_gs03_isotope.setHalfLifeValue();
  auto halflife = Converter::seconds{ 2.0 };

  REQUIRE(nubase_gs03_isotope.hl == halflife);
}


TEST_CASE("Read half-life unit", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  nubase_gs03_isotope.setHalfLifeUnit();

  REQUIRE(nubase_gs03_isotope.halflife_unit == "zs");
}


TEST_CASE("Read half-life error", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  nubase_gs03_isotope.setHalfLifeErrorValue();
  auto hl_error = Converter::seconds{ 0.5 };

  REQUIRE(nubase_gs03_isotope.hl_error == hl_error);
}


TEST_CASE("Read and set half-life to double", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  nubase_gs03_isotope.setHalfLife();
  auto halflife = Converter::attoseconds{ 2.0 * 1.0e-3 };
  REQUIRE(nubase_gs03_isotope.hl == halflife);
}


TEST_CASE("Read the majority decay mode", "[Nuclide]")
{
  /// The decaymode function watches for the first stable isotope to start marking
  /// future ones as 'proton rich'. Therefore we need to pass a dummy array
  std::array<bool, Limits::MAX_Z + 1> pnSide = {};
  pnSide.fill(false);

  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  const std::string nubase_isomer03{
    "010 0031   10Lim   33250       40       200      40     RQ     3.7   zs 1.5    1+               97Zi04t   IT=100"
  };
  Nuclide nubase_isomer03_isotope(nubase_isomer03);

  const std::string nubase_gs12{ "010 0030   10Li    33053       13                              2.0   zs 0.5    "
                                 "(1-,2-)       07 94Yo01tj 1975 n = 100 " };
  Nuclide nubase_gs12_isotope(nubase_gs12);

  const std::string nubase_isomer12{ "010 0031   10Lim   33250       40       200      40     RQ     3.7   zs 1.5    "
                                     "1+            07 97Zi04t  1994 IT = 100 " };
  Nuclide nubase_isomer12_isotope(nubase_isomer12);

  nubase_gs03_isotope.setDecayMode(pnSide, 3);
  REQUIRE(nubase_gs03_isotope.decay == "n");

  nubase_isomer03_isotope.setDecayMode(pnSide, 3);
  REQUIRE(nubase_isomer03_isotope.decay == "IT");

  nubase_gs12_isotope.setDecayMode(pnSide, 12);
  REQUIRE(nubase_gs12_isotope.decay == "n");

  nubase_isomer12_isotope.setDecayMode(pnSide, 12);
  REQUIRE(nubase_isomer12_isotope.decay == "IT");
}


TEST_CASE("Read the year", "[Nuclide]")
{
  const std::string nubase_gs12{ "010 0030   10Li    33053       13                              2.0   zs 0.5    "
                                 "(1-,2-)       07 94Yo01tj 1975 n = 100 " };
  Nuclide nubase_gs12_isotope(nubase_gs12);

  const std::string nubase_isomer12{ "010 0031   10Lim   33250       40       200      40     RQ     3.7   zs 1.5    "
                                     "1+            07 97Zi04t  1994 IT = 100 " };
  Nuclide nubase_isomer12_isotope(nubase_isomer12);

  nubase_gs12_isotope.setYear();
  nubase_isomer12_isotope.setYear();

  REQUIRE(nubase_gs12_isotope.year == 1975);
  REQUIRE(nubase_isomer12_isotope.year == 1994);
}


TEST_CASE("Read mass number", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  nubase_gs03_isotope.setA();

  REQUIRE(nubase_gs03_isotope.A == 10);
}


TEST_CASE("Read proton number", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  nubase_gs03_isotope.setZ();

  REQUIRE(nubase_gs03_isotope.Z == 3);
}


TEST_CASE("Calculate neutron number", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  nubase_gs03_isotope.setA();
  nubase_gs03_isotope.setZ();

  nubase_gs03_isotope.setN();

  REQUIRE(nubase_gs03_isotope.N == 7);
}


TEST_CASE("Read mass excess", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  nubase_gs03_isotope.setNubaseMassExcess();

  REQUIRE(nubase_gs03_isotope.NUBASE_ME == Catch::Approx(33051.0));
}


TEST_CASE("Read mass excess error", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  nubase_gs03_isotope.setNubaseMassExcessError();

  REQUIRE(nubase_gs03_isotope.NUBASE_dME == Catch::Approx(15.0));
}


TEST_CASE("Calculate relative error on mass excess", "[Nuclide]")
{
  SECTION("Random isotope is processed correctly")
  {
    const std::string nubase_gs03{
      "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
    };
    Nuclide nubase_gs03_isotope(nubase_gs03);

    // 33051
    nubase_gs03_isotope.setNubaseMassExcess();
    // 15
    nubase_gs03_isotope.setNubaseMassExcessError();

    // 15/33051 = 0.0005
    REQUIRE(nubase_gs03_isotope.getRelativeMassExcessError(false, 1.0e-7) == Catch::Approx(15.0 / 33051.0));
    REQUIRE(nubase_gs03_isotope.getRelativeMassExcessError(false, 0.1) == Catch::Approx(0.1));
  }

  SECTION("12C and it's annoying 0.0 ME is handled correctly")
  {
    const std::string nubase03_12C{ "012 0060   12C         0.0      0.0                          stbl              0+ "
                                    "           00           IS=98.938 " };
    Nuclide nubase03_12C_isotope(nubase03_12C);

    nubase03_12C_isotope.setA();
    nubase03_12C_isotope.setZ();
    nubase03_12C_isotope.setNubaseMassExcess();
    nubase03_12C_isotope.setNubaseMassExcessError();

    REQUIRE(nubase03_12C_isotope.getRelativeMassExcessError(false, 1.0e-5) == Catch::Approx(1.0e-5));
  }
}


TEST_CASE("SVG output", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  nubase_gs03_isotope.setA();
  nubase_gs03_isotope.setZ();
  nubase_gs03_isotope.setN();
  nubase_gs03_isotope.setSymbol("Li");
  nubase_gs03_isotope.colour = "Blue";

  std::string svg_output{ "<!--10Li-->\n"
                          "<g transform=\"translate(7 -3)\"> <use xlink:href=\"#BlueNucleus\"/></g>\n" };

  // This comparison fails and I can't see why, will revisit later
  // REQUIRE(nubase_gs03_isotope.writeAsSVG(0, 0) == svg_output);
  REQUIRE_FALSE(svg_output.compare(nubase_gs03_isotope.writeAsSVG(0, 0)));
}


TEST_CASE("TikZ output", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  nubase_gs03_isotope.setA();
  nubase_gs03_isotope.setZ();
  nubase_gs03_isotope.setN();
  nubase_gs03_isotope.setSymbol("Li");
  nubase_gs03_isotope.colour = "Blue";

  std::string tikz_output = fmt::format("%10Li\n"
                                        "\\nucleus{{Blue}}{{7}}{{3}}{{10}}{{Li}}\n");

  // REQUIRE(nubase_gs03_isotope.writeAsTIKZ() == tikz_output);
  REQUIRE_FALSE(tikz_output.compare(nubase_gs03_isotope.writeAsTIKZ()));
}


TEST_CASE("Display mode", "[Nuclide]")
{
  SECTION("1st isomer colour, stable isotope, not own")
  {
    const std::string nubase_gs03{
      "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
    };
    Nuclide nubase_gs03_isotope(nubase_gs03);

    nubase_gs03_isotope.decay = "stable";
    nubase_gs03_isotope.own   = false;
    REQUIRE(nubase_gs03_isotope.getDisplayMode(ChartColour::FIRST_ISOMERENERGY) == IsotopeDisplayMode::TopHalf);
  }

  SECTION("decay mode colour, non-stable, not own")
  {
    const std::string nubase_gs03{
      "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
    };
    Nuclide nubase_gs03_isotope(nubase_gs03);

    nubase_gs03_isotope.decay = "alpha";
    nubase_gs03_isotope.own   = false;
    REQUIRE(nubase_gs03_isotope.getDisplayMode(ChartColour::GS_DECAYMODE) == IsotopeDisplayMode::EmptySquare);
  }

  SECTION("decay mode colour, non-stable, not own")
  {
    const std::string nubase_gs03{
      "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
    };
    Nuclide nubase_gs03_isotope(nubase_gs03);

    nubase_gs03_isotope.decay = "alpha";
    nubase_gs03_isotope.own   = true;
    REQUIRE(nubase_gs03_isotope.getDisplayMode(ChartColour::GS_DECAYMODE) == IsotopeDisplayMode::BottomLeftWedge);
  }
}


TEST_CASE("Text colour", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  SECTION("Not writing the isotope")
  {
    REQUIRE(nubase_gs03_isotope.getIsotopeTextColour(ChartColour::GS_HALFLIFE, false) == "");
  }

  SECTION("Default colour is black")
  {
    REQUIRE(nubase_gs03_isotope.getIsotopeTextColour(ChartColour::MASSEXCESSERROR, true) == "black");
  }

  SECTION("Colour by isomer energy and stable isotope")
  {
    nubase_gs03_isotope.decay = "stable";
    REQUIRE(nubase_gs03_isotope.getIsotopeTextColour(ChartColour::FIRST_ISOMERENERGY, true) == "white");
  }
}


TEST_CASE("AME Mass Excess", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  const std::string ame_gs03{ "   0    8    8   16 O         -4737.00141    0.00016   7976.206    0.000 B- -15417.255 "
                              "8.321  15 994914.61956    0.00016" };

  nubase_gs03_isotope.setAMEMassExcess(ame_gs03);
  REQUIRE(nubase_gs03_isotope.AME_ME == Catch::Approx(-4737.00141));
}


TEST_CASE("AME Mass Excess error", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  const std::string ame_gs03{ "   0    8    8   16 O         -4737.00141    0.00016   7976.206    0.000 B- -15417.255 "
                              "8.321  15 994914.61956    0.00016" };

  nubase_gs03_isotope.setAMEMassExcessError(ame_gs03);
  REQUIRE(nubase_gs03_isotope.AME_dME == Catch::Approx(0.00016));
}


TEST_CASE("Write as EPS", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);
  nubase_gs03_isotope.setA();
  nubase_gs03_isotope.setZ();
  nubase_gs03_isotope.setN();
  nubase_gs03_isotope.setSymbol("Li");
  nubase_gs03_isotope.colour = "green";

  Options options;
  options.write_isotope = true;
  options.chart_colour  = ChartColour::MASSEXCESSERROR;

  std::string theline{ "%10Li\n"
                       "0 black (Li) (10) green 7 3 curve Nucleus\n" };

  // REQUIRE(nubase_gs03_isotope.writeAsEPS(options) == theline);
  REQUIRE_FALSE(theline.compare(nubase_gs03_isotope.writeAsEPS(options)));
}


TEST_CASE("CSV header line", "[Nuclide]")
{
  const std::string nubase_gs03{
    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
  };
  Nuclide nubase_gs03_isotope(nubase_gs03);

  std::string header{
    "A,Z,N,Symbol,DecayMode,ExperimentallyMeasured,Own,NUBASEMass,ErrorNUBASEMass,AMEMass,ErrorAMEMass,HalfLife,"
    "SingleNeutronSeparationEnergy,ErrorSingleNeutronSeparationEnergy,SingleProtonSeparationEnergy,"
    "ErrorSingleProtonSeparationEnergy,DoubleNeutronSeparationEnergy,ErrorDoubleNeutronSeparationEnergy,"
    "DoubleProtonSeparationEnergy,ErrorDoubleProtonSeparationEnergy,DiscoveryYear"
  };

  REQUIRE(nubase_gs03_isotope.CSVHeader() == header);
}


// TEST_CASE("Write as csv", "[Nuclide]")
//{
//  const std::string nubase_gs03{
//    "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
//  };
//  Nuclide nubase_gs03_isotope(nubase_gs03);
//
//  int state{ 0 };
//  nubase_gs03_isotope.setExperimental();
//  nubase_gs03_isotope.setA();
//  nubase_gs03_isotope.setZ();
//  nubase_gs03_isotope.setSymbol(Converter::ZToSymbol(nubase_gs03_isotope.Z));
//  nubase_gs03_isotope.setN();
//  nubase_gs03_isotope.setState(state);
//  nubase_gs03_isotope.setNubaseMassExcess();
//  nubase_gs03_isotope.setNubaseMassExcessError();
//  nubase_gs03_isotope.setSpinParity();
//  nubase_gs03_isotope.setHalfLife();
//  nubase_gs03_isotope.decay = "stable";
//  // nubase_gs03_isotope.setDecayMode(pnSide, table_year);
//  // nubase_gs03_isotope.setNeutronOrProtonRich(pnSide);
//
//  std::string line{
//    "10,3,7,Li,stable,0,true,33051.0,15.0,-4737.00141,0.00016,2.00e-21,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1900"
//  };
//
//  REQUIRE_THAT(nubase_gs03_isotope.writeAsCSV(), Catch::Matches(line));
//}
