#include "inch/chartColour.hpp"
#include "inch/key.hpp"
#include "inch/options.hpp"
#include "inch/partition.hpp"

#include <catch2/catch.hpp>

class KeyForTest : public Key
{
public:
  KeyForTest(){};

  void SetText(const Partition& part) const;
  std::string Setup() const;
  std::string MassExcessSetup() const;
  std::string HalLifeSetup() const;
  std::string PlaceKey(const Options& draw) const;
  std::string AdditionalFunctions(const ChartColour& colour) const;
  void Write(std::ofstream& outFile, const Partition& part) const;
  std::string SurroundingBox() const;
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
