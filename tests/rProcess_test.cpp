#include "inch/rProcess.hpp"

#include <catch2/catch.hpp>

class rProcessForTest : public rProcess
{
public:
  explicit rProcessForTest(Limits _limits) : rProcess(std::move(_limits)) {}
  ~rProcessForTest() = default;

  rProcessForTest(const rProcessForTest& rProcessForTest)     = default;
  rProcessForTest(rProcessForTest&& rProcessForTest) noexcept = default;

  rProcessForTest& operator=(const rProcessForTest& rProcessForTest) = delete;
  rProcessForTest& operator=(rProcessForTest&& rProcessForTest) noexcept = delete;


  inline void WritePath(std::ofstream& /*outFile*/, const bool /*shaded*/) const {}
  inline std::string PathSetup(const bool /*shaded*/) const { return std::string(); }
  inline std::string PathTearDown(const bool /*shaded*/) const { return std::string(); }
};


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
