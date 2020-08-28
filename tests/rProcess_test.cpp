#include "inch/rProcess.hpp"

#include <catch2/catch.hpp>

class rProcessForTest : public rProcess
{
public:
  rProcessForTest(Limits _limits) : rProcess(std::move(_limits)) {}
  ~rProcessForTest();

  inline void WritePath(std::ofstream& /*outFile*/, const bool /*shaded*/) const {}
  inline std::string PathSetup(const bool /*shaded*/) const { return std::string(); }
  inline std::string PathTearDown(const bool /*shaded*/) const { return std::string(); }
};

rProcessForTest::~rProcessForTest()
{
}

const Limits l;
rProcessForTest rproc(l);

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
