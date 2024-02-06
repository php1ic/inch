#include "inch/massTable.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Construction", "[MassTable]")
{
  Options options;
  SECTION("Default construction populates variables")
  {
    const MassTable default_construction(options);
    REQUIRE(default_construction.use_AME == false);
    REQUIRE(default_construction.table_year == 3);
    REQUIRE(default_construction.user_isotopes == std::string());
  }
}


TEST_CASE("Alter the table year", "[MassTable]")
{
  Options options;
  const MassTable table(options);
  REQUIRE(table.table_year == 3);
  table.setTableYear(12);
  REQUIRE(table.table_year == 12);
}


TEST_CASE("Absolute paths are constructed", "[MassTable]")
{
  Options options;
  const std::filesystem::path datapath = Options::getAbsolutePath();
  const MassTable table(options);
  table.setFilePaths(3);
  REQUIRE(table.mass_table_NUBASE == datapath / "nubtab03.asc");
  REQUIRE(table.mass_table_AME == datapath / "mass.mas03");

  table.setFilePaths(12);
  REQUIRE(table.mass_table_NUBASE == datapath / "nubase.mas12");
  REQUIRE(table.mass_table_AME == datapath / "mass.mas12");

  table.setFilePaths(16);
  REQUIRE(table.mass_table_NUBASE == datapath / "nubase2016.txt");
  REQUIRE(table.mass_table_AME == datapath / "mass16.txt");
}


TEST_CASE("Read neutron range", "[MassTable]")
{
  Options options;
  MassTable table(options);
  // If we don't read the table then test will definitely fail
  if (!table.populateInternalMassTable())
    {
      REQUIRE(false);
    }

  SECTION("Single stable isotope")
  {
    auto vals = table.GetStableNeutronRange(45);
    REQUIRE(vals.first == 58);
    REQUIRE(vals.second == 58);
  }

  SECTION("Range of stable isotopes")
  {
    auto vals = table.GetStableNeutronRange(30);
    REQUIRE(vals.first == 34);
    REQUIRE(vals.second == 40);
  }
}


TEST_CASE("Try to read non-existant file", "[MassTable]")
{
  Options options;
  MassTable table(options);
  table.setFilePaths(3);

  SECTION("NUBASE") { REQUIRE_FALSE(table.readNUBASE("doesnot.exist")); }
  SECTION("AME") { REQUIRE_FALSE(table.readAME("doesnot.exist")); }
  SECTION("Own") { REQUIRE_FALSE(table.readOWN("doesnot.exist")); }
}
