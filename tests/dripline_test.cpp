#include "inch/dripline.hpp"
#include "inch/limits.hpp"
#include "inch/options.hpp"

#include <catch2/catch.hpp>

const Limits limits;
const Options options;


TEST_CASE("Set the line colour", "[DripLine]")
{
  const DripLine dripline(1.0, 2.0, limits, LineType::singleproton);
  dripline.setDripLineColour("blue");
  REQUIRE_THAT(dripline.line_colour, Catch::Matches("blue"));

  dripline.setDripLineColour("green");
  REQUIRE_THAT(dripline.line_colour, Catch::Matches("green"));
}


TEST_CASE("EPS dripline setup is correct", "[DripLine]")
{
  const DripLine dripline(1.0, 2.0, limits, LineType::singleproton);
  dripline.setDripLineColour("green");
  const auto setup{ "gs\n"
                    "green rgb\n"
                    "1 u div sl\n" };

  REQUIRE_THAT(setup, Catch::Equals(dripline.EPSSetup()));
}


TEST_CASE("EPS dripline teardown is correct", "[DripLine]")
{
  const DripLine dripline(1.0, 2.0, limits, LineType::doubleneutron);

  const auto teardown{ "st\n"
                       "gr\n" };

  REQUIRE_THAT(teardown, Catch::Equals(dripline.EPSTearDown()));
}


TEST_CASE("Dripline datafile is correct formatted", "[DripLine]")
{
  const DripLine dripline(1.0, 2.0, limits, LineType::singleneutron);

  const auto dataline {"  1  23 4.3210\n"};

  REQUIRE_THAT(dataline, Catch::Equals(dripline.WriteDataLine(1, 23, 4.3210)));
}


TEST_CASE("Dripline datafiles exist", "[DripLine]")
{
  SECTION("Single Neutron Drip Line")
    {
      const DripLine dripline(1.0, 2.0, limits, LineType::singleneutron);
      dripline.setDripLineFile(options);

      const auto fileLocation = options.data_path / "neutron.drip";

      REQUIRE_THAT(options.neutron_drip, Catch::Equals(fileLocation));
    }

    SECTION("Double Neutron Drip Line")
    {
      const DripLine dripline(1.0, 2.0, limits, LineType::doubleneutron);
      dripline.setDripLineFile(options);

      const auto fileLocation = options.data_path / "2neutron.drip";

      REQUIRE_THAT(options.two_neutron_drip, Catch::Equals(fileLocation));
    }

    SECTION("Single Proton Drip Line")
    {
      const DripLine dripline(1.0, 2.0, limits, LineType::singleproton);
      dripline.setDripLineFile(options);

      const auto fileLocation = options.data_path / "proton.drip";

      REQUIRE_THAT(options.proton_drip, Catch::Equals(fileLocation));
    }

    SECTION("Double Proton Drip Line")
    {
      const DripLine dripline(1.0, 2.0, limits, LineType::doubleproton);
      dripline.setDripLineFile(options);

      const auto fileLocation = options.data_path / "2proton.drip";

      REQUIRE_THAT(options.two_proton_drip, Catch::Equals(fileLocation));
    }
}
