#include "inch/nuclide.hpp"

#include <catch2/catch.hpp>


/// 2003 Nubase table
const std::string nubase_gs03{
  "010 0030   10Li    33051       15                              2.0   zs 0.5    (1-,2-)       99 94Yo01tj  n=100"
};
Nuclide nubase_gs03_isotope(nubase_gs03);

const std::string nubase_isomer03{
  "010 0031   10Lim   33250       40       200      40     RQ     3.7   zs 1.5    1+               97Zi04t   IT=100"
};
Nuclide nubase_isomer03_isotope(nubase_isomer03);

const std::string nubase_theoretical03{
  "014 0090   14F     32660#     400#                                             2-#                        p ?"
};
Nuclide nubase_theoretical03_isotope(nubase_theoretical03);

const std::string nubase03_12C{
  "012 0060   12C         0.0      0.0                          stbl              0+            00           IS=98.93 8"
};
Nuclide nubase03_12C_isotope(nubase03_12C);

/// 2012 Nubase table
const std::string nubase_gs12{
  "010 0030   10Li    33053       13                              2.0   zs 0.5    (1-,2-)       07 94Yo01tj 1975 n=100"
};
Nuclide nubase_gs12_isotope(nubase_gs12);

const std::string nubase_isomer12{
  "010 0031   10Lim   33250       40       200      40     RQ     3.7   zs 1.5    1+            07 97Zi04t  1994 IT=100"
};
Nuclide nubase_isomer12_isotope(nubase_isomer12);


// TEST_CASE("", "[Nuclide]")
//{
//}


TEST_CASE("Set IsomerEnergy", "[Nuclide]")
{
  double value{ 0.0 };
  nubase_isomer03_isotope.setIsomerEnergy(value);

  REQUIRE(value == Approx(200.0));
}


TEST_CASE("Set IsomerEnergyError", "[Nuclide]")
{
  double value{ 0.0 };
  nubase_isomer03_isotope.setIsomerEnergyError(value);

  REQUIRE(value == Approx(40.0));
}


TEST_CASE("Read and set measured or theoretical", "[Nuclide]")
{
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
  REQUIRE(nubase_gs03_isotope.errorQuadrature(1.0, 0.0) == Approx(1.0));
  REQUIRE(nubase_gs03_isotope.errorQuadrature(3.0, 4.0) == Approx(5.0));
  REQUIRE(nubase_gs03_isotope.errorQuadrature(2.0, 2.0) == Approx(2.828427125));

  REQUIRE(nubase_gs03_isotope.errorQuadrature(1.0, 2.0, 3.0) == Approx(3.741657));

  REQUIRE(nubase_gs03_isotope.errorQuadrature(1.0, 1.0, 2.0, 5.0) == Approx(5.567764));
  REQUIRE(nubase_gs03_isotope.errorQuadrature(5.0, 6.0, 7.0, 8.0) == Approx(sqrt(174)));
}


TEST_CASE("Read spin parity of the state", "[Nuclide]")
{
  nubase_gs03_isotope.setSpinParity();

  REQUIRE(nubase_gs03_isotope.J == Approx(1.0));
  REQUIRE(nubase_gs03_isotope.pi == 1);
  REQUIRE(nubase_gs03_isotope.J_tent == 1);

  nubase_isomer03_isotope.setSpinParity();

  REQUIRE(nubase_isomer03_isotope.J == Approx(1.0));
  REQUIRE(nubase_isomer03_isotope.pi == 0);
  REQUIRE(nubase_isomer03_isotope.J_tent == 0);
}


TEST_CASE("Set missing units string", "[Nuclide]")
{
  REQUIRE_THAT(nubase_gs03_isotope.missingUnit(), Catch::Matches("no_units"));
}


TEST_CASE("Set 'own' flag", "[Nuclide]")
{
  nubase_gs03_isotope.setOwn(true);
  REQUIRE(nubase_gs03_isotope.own == true);

  nubase_gs03_isotope.setOwn(false);
  REQUIRE(nubase_gs03_isotope.own == false);
}


TEST_CASE("Read half-life value", "[Nuclide]")
{
  nubase_gs03_isotope.setHalfLifeValue();

  REQUIRE(nubase_gs03_isotope.hl == Approx(2.0));
}


TEST_CASE("Read half-life unit", "[Nuclide]")
{
  nubase_gs03_isotope.setHalfLifeUnit();

  REQUIRE_THAT(nubase_gs03_isotope.halflife_unit, Catch::Matches("zs"));
}


TEST_CASE("Read and set half-life to double", "[Nuclide]")
{
  nubase_gs03_isotope.setHalfLife();

  REQUIRE(nubase_gs03_isotope.hl == Approx(2.0e-21));
}


TEST_CASE("Read the majority decay mode", "[Nuclide]")
{
  /// The decaymode function watches for the first stable isotope to start marking
  /// future ones as 'proton rich'. Therefore we need to pass a dummy vector<bool>
  /// that can be altered (ZMAX ~ 120)
  std::vector<bool> pnSide(false, 120);

  nubase_gs03_isotope.setDecayMode(pnSide, 3);
  REQUIRE_THAT(nubase_gs03_isotope.decay, Catch::Matches("n"));

  nubase_isomer03_isotope.setDecayMode(pnSide, 3);
  REQUIRE_THAT(nubase_isomer03_isotope.decay, Catch::Matches("IT"));

  nubase_gs12_isotope.setDecayMode(pnSide, 12);
  REQUIRE_THAT(nubase_gs12_isotope.decay, Catch::Matches("n"));

  nubase_isomer12_isotope.setDecayMode(pnSide, 12);
  REQUIRE_THAT(nubase_isomer12_isotope.decay, Catch::Matches("IT"));
}


TEST_CASE("Read the year", "[Nuclide]")
{
  nubase_gs12_isotope.setYear();
  nubase_isomer12_isotope.setYear();

  REQUIRE(nubase_gs12_isotope.year == 1975);
  REQUIRE(nubase_isomer12_isotope.year == 1994);
}


TEST_CASE("Read mass number", "[Nuclide]")
{
  nubase_gs03_isotope.setA();

  REQUIRE(nubase_gs03_isotope.A == 10);
}


TEST_CASE("Read proton number", "[Nuclide]")
{
  nubase_gs03_isotope.setZ();

  REQUIRE(nubase_gs03_isotope.Z == 3);
}


TEST_CASE("Calculate neutron number", "[Nuclide]")
{
  nubase_gs03_isotope.setA();
  nubase_gs03_isotope.setZ();

  nubase_gs03_isotope.setN();

  REQUIRE(nubase_gs03_isotope.N == 7);
}


TEST_CASE("Read mass excess", "[Nuclide]")
{
  nubase_gs03_isotope.setNubaseMassExcess();

  REQUIRE(nubase_gs03_isotope.NUBASE_ME == Approx(33051.0));
}


TEST_CASE("Read mass excess error", "[Nuclide]")
{
  nubase_gs03_isotope.setNubaseMassExcessError();

  REQUIRE(nubase_gs03_isotope.NUBASE_dME == Approx(15.0));
}


TEST_CASE("Calculate relative error on mass excess", "[Nuclide]")
{
  SECTION("Random isotope is processed correctly")
    {
      // 33051
      nubase_gs03_isotope.setNubaseMassExcess();
      // 15
      nubase_gs03_isotope.setNubaseMassExcessError();

      // 15/33051 = 0.0005
      REQUIRE(nubase_gs03_isotope.getRelativeMassExcessError(false, 1.0e-7) == Approx(15.0 / 33051.0));
      REQUIRE(nubase_gs03_isotope.getRelativeMassExcessError(false, 0.1) == Approx(0.1));
    }

  SECTION("12C and it's annoying 0.0 ME is handled correctly")
    {
      nubase03_12C_isotope.setA();
      nubase03_12C_isotope.setZ();
      nubase03_12C_isotope.setNubaseMassExcess();
      nubase03_12C_isotope.setNubaseMassExcessError();

      REQUIRE(nubase03_12C_isotope.getRelativeMassExcessError(false, 1.0e-5) == Approx(1.0e-5));
    }
}
