#include "inch/chartColour.hpp"
#include "inch/key.hpp"
#include "inch/options.hpp"
#include "inch/partition.hpp"

#include <catch2/catch.hpp>

class KeyForTest : public Key
{
public:
  KeyForTest()  = default;
  ~KeyForTest() = default;

  KeyForTest(const KeyForTest& KeyForTest)     = default;
  KeyForTest(KeyForTest&& KeyForTest) noexcept = default;

  KeyForTest& operator=(const KeyForTest& KeyForTest) = delete;
  KeyForTest& operator=(KeyForTest&& KeyForTest) noexcept = delete;

  inline void SetText(const Partition& /*part*/) const {}
  inline std::string Setup() const { return std::string(); }
  inline std::string MassExcessSetup() const { return std::string(); }
  inline std::string HalLifeSetup() const { return std::string(); }
  inline std::string PlaceKey(const Options& /*draw*/) const { return std::string(); }
  inline std::string AdditionalFunctions(const ChartColour& /*colour*/) const { return std::string(); }
  inline void Write(std::ofstream& /*outFile*/, const Partition& /*part*/) const {}
  inline std::string SurroundingBox() const { return std::string(); }
};


KeyForTest key;
Options options;
Partition partition(ChartColour::MASSEXCESSERROR);

TEST_CASE("The key scales as required", "[Key]")
{
  SECTION("No key sets the scale appropriately")
  {
    options.key = false;
    key.scale   = 1.0;

    key.setScale(options, partition);

    REQUIRE(key.scale == Approx(0.0));
  }
}
