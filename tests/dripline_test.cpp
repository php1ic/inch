#include "inch/dripline.hpp"
#include "inch/limits.hpp"
#include "inch/options.hpp"

#include <catch2/catch.hpp>

class DripLineForTest : public DripLine
{
public:
  DripLineForTest(
      const double nMass, const double pMass, Limits _limits, const LineType& line, std::string_view colour) :
      DripLine(nMass, pMass, std::move(_limits), line, colour)
  {
  }
  ~DripLineForTest() override = default;

  DripLineForTest(const DripLineForTest& DripLineForTest)     = default;
  DripLineForTest(DripLineForTest&& DripLineForTest) noexcept = default;

  DripLineForTest& operator=(const DripLineForTest& DripLineForTest) = delete;
  DripLineForTest& operator=(DripLineForTest&& DripLineForTest) noexcept = delete;


  [[nodiscard]] inline std::string Setup() const override { return std::string(); }
  [[nodiscard]] inline std::string TearDown() const override { return std::string(); }
  [[nodiscard]] inline std::string Header() const override { return std::string(); }
  inline int WriteLine(std::ostream& /*outFile*/) const override { return 0; }
};


const Limits limits;

TEST_CASE("Set the line colour", "[DripLine]")
{
  const DripLineForTest dripline(1.0, 2.0, limits, LineType::singleproton, "blue");
  REQUIRE_THAT(dripline.line_colour, Catch::Matches("blue"));

  dripline.setDripLineColour("green");
  REQUIRE_THAT(dripline.line_colour, Catch::Matches("green"));
}


TEST_CASE("FRDM file does not exist", "[DripLine]")
{
  const DripLineForTest dripline(1.0, 2.0, limits, LineType::singleproton, "blue");
  dripline.FRDM_file = "random.file";

  REQUIRE_FALSE(dripline.readFRDMFile(true));
}


TEST_CASE("Don't overwrite the existing stored data", "[DripLine]")
{
  const DripLineForTest dripline(1.0, 2.0, limits, LineType::singleproton, "blue");
  dripline.readFRDMFile();

  SECTION("Data already read") { REQUIRE(dripline.readFRDMFile()); }

  SECTION("Manually stop any over writing") { REQUIRE(dripline.readFRDMFile(false)); }
}


TEST_CASE("drip file is set to the default if it has been cleared", "[DripLine]")
{
  DripLineForTest dripline(1.0, 2.0, limits, LineType::singleproton, "blue");

  SECTION("No file is set to default")
  {
    dripline.drip_file = "";
    dripline.createFileIfDoesNotExist();
    REQUIRE(dripline.drip_file == (Options::getAbsolutePath() / "proton.drip"));
  }

  DripLineForTest moved_drip = std::move(dripline);
  SECTION("Create a file that is not there")
  {
    moved_drip.drip_file = "doesnotexist.dripline";
    REQUIRE_FALSE(moved_drip.createFileIfDoesNotExist());
  }
}


TEST_CASE("Create a dripline data file", "[DripLine]")
{
  const DripLineForTest dripline(1.0, 2.0, limits, LineType::singleproton, "blue");

  SECTION("create a proton file")
  {
    dripline.drip_file = "proton.file";
    REQUIRE_FALSE(dripline.createFile());
  }

  SECTION("create a neutron file")
  {
    dripline.the_line  = LineType::singleneutron;
    dripline.drip_file = "neutron.file";
    REQUIRE_FALSE(dripline.createFile());
  }
}


TEST_CASE("Dripline datafile is correct formatted", "[DripLine]")
{
  const DripLineForTest dripline(1.0, 2.0, limits, LineType::singleneutron, "black");

  const auto dataline{ "  1  23\n" };

  REQUIRE_THAT(dataline, Catch::Equals(dripline.WriteDataLine(1, 23)));
}


TEST_CASE("Dripline datafiles exist", "[DripLine]")
{
  const auto data_path = Options::getAbsolutePath();

  SECTION("Single Neutron Drip Line")
  {
    const DripLineForTest dripline(1.0, 2.0, limits, LineType::singleneutron, "black");
    dripline.setDripLineFile();

    const auto fileLocation = data_path / "neutron.drip";

    REQUIRE(dripline.drip_file == fileLocation);
  }

  SECTION("Double Neutron Drip Line")
  {
    const DripLineForTest dripline(1.0, 2.0, limits, LineType::doubleneutron, "black");
    dripline.setDripLineFile();

    const auto fileLocation = data_path / "2neutron.drip";

    REQUIRE(dripline.drip_file == fileLocation);
  }

  SECTION("Single Proton Drip Line")
  {
    const DripLineForTest dripline(1.0, 2.0, limits, LineType::singleproton, "black");
    dripline.setDripLineFile();

    const auto fileLocation = data_path / "proton.drip";

    REQUIRE(dripline.drip_file == fileLocation);
  }

  SECTION("Double Proton Drip Line")
  {
    const DripLineForTest dripline(1.0, 2.0, limits, LineType::doubleproton, "black");
    dripline.setDripLineFile();

    const auto fileLocation = data_path / "2proton.drip";

    REQUIRE(dripline.drip_file == fileLocation);
  }
}


TEST_CASE("N value of chart is large enough", "[DripLine]")
{
  SECTION("Single Neutron")
  {
    DripLineForTest dripline(1.0, 2.0, limits, LineType::singleneutron, "black");
    dripline.limits.Nmax = 17;
    REQUIRE_FALSE(dripline.isNmaxValueHighEnough());
    dripline.limits.Nmax = 18;
    REQUIRE(dripline.isNmaxValueHighEnough());
  }

  SECTION("Double Neutron")
  {
    DripLineForTest dripline(1.0, 2.0, limits, LineType::doubleneutron, "black");
    dripline.limits.Nmax = 20;
    REQUIRE_FALSE(dripline.isNmaxValueHighEnough());
    dripline.limits.Nmax = 21;
    REQUIRE(dripline.isNmaxValueHighEnough());
  }
}


TEST_CASE("Z value of chart is large enough", "[DripLine]")
{
  SECTION("Single Proton")
  {
    DripLineForTest dripline(1.0, 2.0, limits, LineType::singleproton, "black");
    dripline.limits.Zmax = 11;
    REQUIRE_FALSE(dripline.isZmaxValueHighEnough());
    dripline.limits.Zmax = 12;
    REQUIRE(dripline.isZmaxValueHighEnough());
  }

  SECTION("Double Proton")
  {
    DripLineForTest dripline(1.0, 2.0, limits, LineType::doubleproton, "black");
    dripline.limits.Zmax = 14;
    REQUIRE_FALSE(dripline.isZmaxValueHighEnough());
    dripline.limits.Zmax = 15;
    REQUIRE(dripline.isZmaxValueHighEnough());
  }
}


TEST_CASE("Both N and Z are large enough", "[DripLine]")
{
  DripLineForTest dripline(1.0, 2.0, limits, LineType::doubleproton, "black");

  // Default construction
  REQUIRE(dripline.areNmaxAndZmaxValuesHighEnough());
  auto otherLine = dripline;
  REQUIRE(otherLine.areNmaxAndZmaxValuesHighEnough());

  SECTION("Neutron")
  {
    otherLine.the_line = LineType::singleneutron;
    // Both are too low
    otherLine.limits.Nmax = 17;
    otherLine.limits.Zmax = 11;
    REQUIRE_FALSE(otherLine.areNmaxAndZmaxValuesHighEnough());

    // N is too low, Z is OK
    otherLine.limits.Nmax = 17;
    otherLine.limits.Zmax = 50;
    REQUIRE_FALSE(otherLine.areNmaxAndZmaxValuesHighEnough());

    // N is OK, Z is too low
    otherLine.limits.Nmax = 18;
    otherLine.limits.Zmax = 11;
    REQUIRE_FALSE(otherLine.areNmaxAndZmaxValuesHighEnough());

    // Both are OK
    otherLine.limits.Nmax = 18;
    otherLine.limits.Zmax = 15;
    REQUIRE(otherLine.areNmaxAndZmaxValuesHighEnough());
  }

  SECTION("Proton")
  {
    otherLine.the_line = LineType::singleproton;
    // Both are too low
    otherLine.limits.Nmax = 17;
    otherLine.limits.Zmax = 11;
    REQUIRE_FALSE(otherLine.areNmaxAndZmaxValuesHighEnough());

    // N is too low, Z is OK
    otherLine.limits.Nmax = 17;
    otherLine.limits.Zmax = 50;
    REQUIRE_FALSE(otherLine.areNmaxAndZmaxValuesHighEnough());

    // N is OK, Z is too low
    otherLine.limits.Nmax = 18;
    otherLine.limits.Zmax = 11;
    REQUIRE_FALSE(otherLine.areNmaxAndZmaxValuesHighEnough());

    // Both are OK
    otherLine.limits.Nmax = 21;
    otherLine.limits.Zmax = 15;
    REQUIRE(otherLine.areNmaxAndZmaxValuesHighEnough());
  }
}


TEST_CASE("Drip line value is correct", "[DripLine]")
{
  DripLineForTest dripline(1.0, 2.0, limits, LineType::doubleproton, "black");
  dripline.readFRDMFile();

  REQUIRE(dripline.GetProtonDripValue(76) == 59);

  dripline.the_line = LineType::doubleneutron;
  REQUIRE(dripline.GetNeutronDripValue(100) == 150);

  dripline.the_line = LineType::singleproton;
  REQUIRE(dripline.GetProtonDripValue(76) == 57);

  dripline.the_line = LineType::singleneutron;
  REQUIRE(dripline.GetNeutronDripValue(100) == 147);
}
