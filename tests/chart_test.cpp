#include "inch/chart.hpp"

#include <catch2/catch_all.hpp>

// Create a concrete class so we can test the non-virtual functions
// This seems to be better than picking a 'random' derived class
class ChartForTest : public Chart
{
public:
  explicit ChartForTest(Options& _options) : Chart(_options){};
  ~ChartForTest() override = default;

  ChartForTest(const ChartForTest& ChartForTest)     = default;
  ChartForTest(ChartForTest&& ChartForTest) noexcept = default;

  ChartForTest& operator=(const ChartForTest& ChartForTest)     = delete;
  ChartForTest& operator=(ChartForTest&& ChartForTest) noexcept = delete;


  inline void write(const std::vector<Nuclide>& /*massTable*/, const Partition& /*part*/) const override {}

  inline std::string prolog() const override { return {}; }

  inline std::string header() const override { return {}; }

  inline std::string definitions() const override { return {}; }

  inline std::string colours() const override { return {}; }

  inline std::string isotope() const override { return {}; }

  inline std::string infoComment() const override { return {}; }

  inline std::string setup() const override { return {}; }

  inline std::string teardown() const override { return {}; }

  inline std::string KeySetup(const int /*ZRange*/) const override { return {}; }

  inline std::string KeyTearDown() const override { return {}; }
};


TEST_CASE("Canvas Height", "[Chart]")
{
  Options options;
  ChartForTest theChart(options);

  SECTION("Z range is large enough to not use a relatively placed chart")
  {
    theChart.key_relative = false;
    const int ZRange{ 50 };
    const double scale{ 1.5 };
    const double height{ 6.0 };

    REQUIRE(theChart.calculateCanvasHeight(scale, height, ZRange) == Catch::Approx(52.0));
    REQUIRE(theChart.key_relative == false);
  }

  SECTION("Z range means that the chart will be placed relative to the key")
  {
    theChart.key_relative = false;

    const int ZRange{ 1 };
    const double scale{ 2.0 };
    const double height{ 5.0 };

    REQUIRE(theChart.calculateCanvasHeight(scale, height, ZRange) == Catch::Approx(12.0));
    REQUIRE(theChart.key_relative == true);
  }
}


TEST_CASE("Canvas Width", "[Chart]")
{
  Options options;
  ChartForTest theChart(options);

  SECTION("Full chart does not have additional width")
  {
    theChart.options.limits.ResetAllLimits();
    theChart.options.chart_selection = ChartSelection::FULL_CHART;

    const double scale{ 2.0 };
    REQUIRE(theChart.calculateCanvasWidth(scale) == Catch::Approx(179.0));
  }

  SECTION("Partial chart has additional canvas width")
  {
    theChart.options.limits.Nmin     = 20;
    theChart.options.limits.Nmax     = 50;
    theChart.options.limits.Zmin     = 30;
    theChart.options.limits.Zmax     = 70;
    theChart.options.chart_selection = ChartSelection::SUB_CHART;

    const double scale{ 1.5 };
    REQUIRE(theChart.calculateCanvasWidth(scale) == Catch::Approx(53.75));
  }
}


TEST_CASE("Canvas size", "[Chart]")
{
  Options options;
  ChartForTest theChart(options);

  theChart.options.limits.Nmin     = 20;
  theChart.options.limits.Nmax     = 50;
  theChart.options.limits.Zmin     = 30;
  theChart.options.limits.Zmax     = 70;
  theChart.options.chart_selection = ChartSelection::SUB_CHART;

  const double scale{ 1.5 };
  const double height{ 5.0 };

  theChart.setCanvasSize(scale, height);

  REQUIRE(theChart.width == Catch::Approx(53.75));
  REQUIRE(theChart.height == Catch::Approx(42.0));
}


TEST_CASE("Chart title", "[Chart]")
{
  Options options;
  ChartForTest theChart(options);

  REQUIRE(theChart.getTitle(ChartColour::MASSEXCESSERROR) == "Error on mass-excess");
  REQUIRE(theChart.getTitle(ChartColour::REL_MASSEXCESSERROR) == "Relative error on mass-excess");
  REQUIRE(theChart.getTitle(ChartColour::GS_DECAYMODE) == "Major ground-state decay mode");
  REQUIRE(theChart.getTitle(ChartColour::GS_HALFLIFE) == "Ground-state half-life");
  REQUIRE(theChart.getTitle(ChartColour::FIRST_ISOMERENERGY) == "First isomer energy");
}


TEST_CASE("Chart creation time", "[Chart]")
{
  Options options;
  ChartForTest theChart(options);

  auto when = std::time(nullptr);
  auto now  = fmt::format("{:%Y-%m-%dT%H:%M:%S}", *std::localtime(&when));
  REQUIRE(theChart.getTime() == now);
}
