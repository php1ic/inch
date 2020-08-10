#include "inch/eps_rProcess.hpp"

#include <catch2/catch.hpp>

Limits limits;

TEST_CASE("EPS path is correctly setup", "[EPSrProcess]")
{
  EPSrProcess rproc(limits);

  auto setup{ "\n%r-process -- shaded path\n"
              "gs\n"
              "0.9 setgray\n" };

  SECTION("Do the shaded setup of the path") { REQUIRE_THAT(setup, Catch::Equals(rproc.PathSetup(true))); }

  SECTION("Do the outline setup of the path")
  {
    setup = "\n%r-process -- path outline\n"
            "black rgb\n"
            "0.1 u div sl\n";

    REQUIRE_THAT(setup, Catch::Equals(rproc.PathSetup(false)));
  }
}


TEST_CASE("EPS path is correctly torn down", "[EPSrProcess]")
{
  EPSrProcess rproc(limits);

  auto teardown{ "\fill\n"
                 "gr\n" };

  SECTION("Do the shaded teardown of the path") { REQUIRE_THAT(teardown, Catch::Equals(rproc.PathSetup(true))); }

  SECTION("Do the outline teardown of the path")
  {
    teardown = "st\n";

    REQUIRE_THAT(teardown, Catch::Equals(rproc.PathSetup(false)));
  }
}
