#include "inch/rProcess.hpp"

#include <catch2/catch.hpp>

class rProcessForTest : public rProcess
{
public:
  explicit rProcessForTest(Limits _limits) : rProcess(std::move(_limits)) {}
  ~rProcessForTest() override = default;

  rProcessForTest(const rProcessForTest& rProcessForTest)     = default;
  rProcessForTest(rProcessForTest&& rProcessForTest) noexcept = default;

  rProcessForTest& operator=(const rProcessForTest& rProcessForTest) = delete;
  rProcessForTest& operator=(rProcessForTest&& rProcessForTest) noexcept = delete;


  inline void WritePath(std::ofstream& /*outFile*/, const bool /*shaded*/) const override {}
  inline std::string PathSetup(const bool /*shaded*/) const override { return std::string(); }
  inline std::string PathTearDown(const bool /*shaded*/) const override { return std::string(); }
};


TEST_CASE("Z value is larger than start of r-process", "[rProcess]")
{
  const Limits l;
  rProcessForTest rproc(l);

  REQUIRE(rproc.isZHighEnough() == true);

  auto isZset = rproc.limits.setZmax(20);

  REQUIRE((isZset && rproc.isZHighEnough() == false));
}


TEST_CASE("Data file operations", "[rProcess]")
{
  const Limits l;
  rProcessForTest rproc(l);

  const std::string theFile{ "random.file" };
  rproc.setRProcessFile(theFile);

  SECTION("The data file can be set after the object is created") { REQUIRE(rproc.file == theFile); }

  SECTION("We don't try and read a non-existant file") { REQUIRE_FALSE(rproc.readData()); }
}


TEST_CASE("Default file is read correctly", "[rProcess]")
{
  const Limits l;
  rProcessForTest rproc(l);

  auto read = rproc.readData();
  SECTION("File is read without error") { REQUIRE(read); }

  SECTION("Values are stored correctly")
  {
    // First values
    REQUIRE(rproc.data.front().first == 187);
    REQUIRE(rproc.data.front().second == 91);

    // Random values
    REQUIRE(rproc.data.at(23).first == 129);
    REQUIRE(rproc.data.at(23).second == 68);

    // Last values
    REQUIRE(rproc.data.back().first == 179);
    REQUIRE(rproc.data.back().second == 98);
  }
}
