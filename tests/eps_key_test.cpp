#include "inch/eps_key.hpp"

#include <catch2/catch.hpp>

TEST_CASE("Setting the txt correctly", "[EPSKey]")
{
  const EPSKey tester;
  Partition part(ChartColour::MASSEXCESSERROR);
  part.setDefaultColours();
  tester.SetText(part);

  const auto output{ "1 TR (Stable : ) TotalWidth sh\n"
                     "1 S (d) TotalWidth sh\n"
                     "1 TR (m < 3.0 keV) TotalWidth sh TestWidth\n" };

  REQUIRE_THAT(output, Catch::Equals(part.values.at(0).keyText));
}
