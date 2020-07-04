#include "inch/rProcess.hpp"

#include <catch2/catch.hpp>

const Limits l;
rProcess rproc(l);

TEST_CASE("Z value is larger than start of r-process", "[rProcess]")
{
  REQUIRE(rproc.isZHighEnough() == true);

  auto isZset = rproc.limits.setZmax(20);

  REQUIRE((isZset && rproc.isZHighEnough() == false));
}

TEST_CASE("Data file is set", "[rProcess]")
{
  const std::string theFile{ "random.file" };
  rproc.setRProcessFile(theFile);

  REQUIRE(rproc.file == theFile);
}
