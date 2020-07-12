#include "inch/dripline.hpp"
#include "inch/limits.hpp"
#include "inch/options.hpp"

#include <catch2/catch.hpp>

const Limits limits;
const Options options;


TEST_CASE("Set the line colour", "[DripLine]")
{
  const DripLine dripline(1.0, 2.0, limits, LineType::singleproton, "blue");
  REQUIRE_THAT(dripline.line_colour, Catch::Matches("blue"));

  dripline.setDripLineColour("green");
  REQUIRE_THAT(dripline.line_colour, Catch::Matches("green"));
}


TEST_CASE("EPS dripline setup is correct", "[DripLine]")
{
  const DripLine dripline(1.0, 2.0, limits, LineType::singleproton, "green");
  const auto setup{ "gs\n"
                    "green rgb\n"
                    "1 u div sl\n" };

  REQUIRE_THAT(setup, Catch::Equals(dripline.EPSSetup()));
}


TEST_CASE("EPS dripline teardown is correct", "[DripLine]")
{
  const DripLine dripline(1.0, 2.0, limits, LineType::doubleneutron, "black");

  const auto teardown{ "st\n"
                       "gr\n" };

  REQUIRE_THAT(teardown, Catch::Equals(dripline.EPSTearDown()));
}


TEST_CASE("Dripline datafile is correct formatted", "[DripLine]")
{
  const DripLine dripline(1.0, 2.0, limits, LineType::singleneutron, "black");

  const auto dataline{ "  1  23\n" };

  REQUIRE_THAT(dataline, Catch::Equals(dripline.WriteDataLine(1, 23)));
}


TEST_CASE("Dripline datafiles exist", "[DripLine]")
{
  const auto data_path = Options::getAbsolutePath();

  SECTION("Single Neutron Drip Line")
  {
    const DripLine dripline(1.0, 2.0, limits, LineType::singleneutron, "black");
    dripline.setDripLineFile();

    const auto fileLocation = data_path / "neutron.drip";

    REQUIRE(dripline.drip_file == fileLocation);
  }

  SECTION("Double Neutron Drip Line")
  {
    const DripLine dripline(1.0, 2.0, limits, LineType::doubleneutron, "black");
    dripline.setDripLineFile();

    const auto fileLocation = data_path / "2neutron.drip";

    REQUIRE(dripline.drip_file == fileLocation);
  }

  SECTION("Single Proton Drip Line")
  {
    const DripLine dripline(1.0, 2.0, limits, LineType::singleproton, "black");
    dripline.setDripLineFile();

    const auto fileLocation = data_path / "proton.drip";

    REQUIRE(dripline.drip_file == fileLocation);
  }

  SECTION("Double Proton Drip Line")
  {
    const DripLine dripline(1.0, 2.0, limits, LineType::doubleproton, "black");
    dripline.setDripLineFile();

    const auto fileLocation = data_path / "2proton.drip";

    REQUIRE(dripline.drip_file == fileLocation);
  }
}


TEST_CASE("N value of chart is large enough", "[DripLine]")
{
  SECTION("Single Neutron")
  {
    DripLine dripline(1.0, 2.0, limits, LineType::singleneutron, "black");
    dripline.limits.Nmax = 17;
    REQUIRE_FALSE(dripline.isNmaxValueHighEnough());
    dripline.limits.Nmax = 18;
    REQUIRE(dripline.isNmaxValueHighEnough());
  }

  SECTION("Double Neutron")
  {
    DripLine dripline(1.0, 2.0, limits, LineType::doubleneutron, "black");
    dripline.limits.Nmax = 20;
    REQUIRE_FALSE(dripline.isNmaxValueHighEnough());
    dripline.limits.Nmax = 21;
    REQUIRE(dripline.isNmaxValueHighEnough());
  }
}


TEST_CASE("Z value of chart is large enough", "[DripLine]")
{
  SECTION("Single Proton")
  {
    DripLine dripline(1.0, 2.0, limits, LineType::singleproton, "black");
    dripline.limits.Zmax = 11;
    REQUIRE_FALSE(dripline.isZmaxValueHighEnough());
    dripline.limits.Zmax = 12;
    REQUIRE(dripline.isZmaxValueHighEnough());
  }

  SECTION("Double Proton")
  {
    DripLine dripline(1.0, 2.0, limits, LineType::doubleproton, "black");
    dripline.limits.Zmax = 14;
    REQUIRE_FALSE(dripline.isZmaxValueHighEnough());
    dripline.limits.Zmax = 15;
    REQUIRE(dripline.isZmaxValueHighEnough());
  }
}


TEST_CASE("Both N and Z are large enough", "[DripLine]")
{
  DripLine dripline(1.0, 2.0, limits, LineType::doubleproton, "black");

  // Default construction
  REQUIRE(dripline.areNmaxAndZmaxValuesHighEnough());
  auto otherLine = dripline;
  REQUIRE(otherLine.areNmaxAndZmaxValuesHighEnough());

  SECTION("Neutron")
  {
    otherLine.the_line = LineType::singleneutron;
    // Both are too low
    otherLine.limits.Nmax = 17;
    otherLine.limits.Zmax = 11;
    REQUIRE_FALSE(otherLine.areNmaxAndZmaxValuesHighEnough());

    // N is too low, Z is OK
    otherLine.limits.Nmax = 17;
    otherLine.limits.Zmax = 50;
    REQUIRE_FALSE(otherLine.areNmaxAndZmaxValuesHighEnough());

    // N is OK, Z is too low
    otherLine.limits.Nmax = 18;
    otherLine.limits.Zmax = 11;
    REQUIRE_FALSE(otherLine.areNmaxAndZmaxValuesHighEnough());

    // Both are OK
    otherLine.limits.Nmax = 18;
    otherLine.limits.Zmax = 15;
    REQUIRE(otherLine.areNmaxAndZmaxValuesHighEnough());
  }

  SECTION("Proton")
  {
    otherLine.the_line = LineType::singleproton;
    // Both are too low
    otherLine.limits.Nmax = 17;
    otherLine.limits.Zmax = 11;
    REQUIRE_FALSE(otherLine.areNmaxAndZmaxValuesHighEnough());

    // N is too low, Z is OK
    otherLine.limits.Nmax = 17;
    otherLine.limits.Zmax = 50;
    REQUIRE_FALSE(otherLine.areNmaxAndZmaxValuesHighEnough());

    // N is OK, Z is too low
    otherLine.limits.Nmax = 18;
    otherLine.limits.Zmax = 11;
    REQUIRE_FALSE(otherLine.areNmaxAndZmaxValuesHighEnough());

    // Both are OK
    otherLine.limits.Nmax = 21;
    otherLine.limits.Zmax = 15;
    REQUIRE(otherLine.areNmaxAndZmaxValuesHighEnough());
  }
}


TEST_CASE("Drip line value is correct", "[DripLine]")
{
  DripLine dripline(1.0, 2.0, limits, LineType::doubleproton, "black");
  dripline.readFRDMFile();

  REQUIRE(dripline.GetProtonDripValue(76) == 59);

  dripline.the_line = LineType::doubleneutron;
  REQUIRE(dripline.GetNeutronDripValue(100) == 150);

  dripline.the_line = LineType::singleproton;
  REQUIRE(dripline.GetProtonDripValue(76) == 57);

  dripline.the_line = LineType::singleneutron;
  REQUIRE(dripline.GetNeutronDripValue(100) == 147);
}
