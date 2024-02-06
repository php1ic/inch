#include "inch/chartColour.hpp"
#include "inch/key.hpp"
#include "inch/limits.hpp"
#include "inch/options.hpp"
#include "inch/partition.hpp"

#include <catch2/catch_all.hpp>

class KeyForTest : public Key
{
public:
  KeyForTest()           = default;
  ~KeyForTest() override = default;

  KeyForTest(const KeyForTest& KeyForTest)     = default;
  KeyForTest(KeyForTest&& KeyForTest) noexcept = default;

  KeyForTest& operator=(const KeyForTest& KeyForTest)     = delete;
  KeyForTest& operator=(KeyForTest&& KeyForTest) noexcept = delete;

  inline void SetText(const Partition& /*part*/) const override {}
  inline std::string Setup() const override { return {}; }
  inline std::string MassExcessSetup() const override { return {}; }
  inline std::string HalLifeSetup() const override { return {}; }
  inline std::string PlaceKey(const Options& /*draw*/) const override { return {}; }
  inline std::string AdditionalFunctions(const ChartColour& /*colour*/) const override { return {}; }
  inline void Write(std::ofstream& /*outFile*/, const Partition& /*part*/) const override {}
  inline std::string SurroundingBox() const override { return {}; }
};


TEST_CASE("The key scales as required", "[Key]")
{
  KeyForTest key;
  Options options;
  Partition partition(ChartColour::MASSEXCESSERROR);


  SECTION("No key sets the scale appropriately")
  {
    options.key = false;
    key.scale   = 1.0;

    key.setScale(options, partition);

    REQUIRE(key.scale == Catch::Approx(0.0));
  }


  SECTION("Doesn't shrink too small")
  {
    options.key         = true;
    options.limits.Zmin = 20;
    options.limits.Zmax = 21;

    key.setScale(options, partition);

    REQUIRE(key.scale == Catch::Approx(key.MAX_SCALE));
  }

  SECTION("Doesn't grow to big")
  {
    options.key         = true;
    options.limits.Zmin = 0;
    options.limits.Zmax = Limits::MAX_Z;

    key.setScale(options, partition);

    REQUIRE(key.scale == Catch::Approx(key.MAX_SCALE));
  }
}
