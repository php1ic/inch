#include "inch/eps_rProcess.hpp"

#include <catch2/catch_all.hpp>


TEST_CASE("EPS path is correctly setup", "[EPSrProcess]")
{
  const Limits limits;
  EPSrProcess rproc(limits);

  auto setup{ "\n%r-process -- shaded path\n"
              "gs\n"
              "0.9 setgray\n" };

  SECTION("Do the shaded setup of the path")
  {
    REQUIRE(setup == rproc.PathSetup(true));
  }

  SECTION("Do the outline setup of the path")
  {
    setup = "\n%r-process -- path outline\n"
            "black rgb\n"
            "0.1 u div sl\n";

    REQUIRE(setup == rproc.PathSetup(false));
  }
}


TEST_CASE("EPS path is correctly torn down", "[EPSrProcess]")
{
  const Limits limits;
  EPSrProcess rproc(limits);

  auto teardown{ "fill\n"
                 "gr\n" };

  SECTION("Do the shaded teardown of the path")
  {
    REQUIRE(teardown == rproc.PathTearDown(true));
  }

  SECTION("Do the outline teardown of the path")
  {
    teardown = "st\n";

    REQUIRE(teardown == rproc.PathTearDown(false));
  }
}
