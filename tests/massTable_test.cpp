#include "inch/massTable.hpp"

#include <catch2/catch.hpp>

TEST_CASE("Construction", "[MassTable]")
{
  SECTION("Default construction populates variables")
  {
    const MassTable default_construction("userdata.dat");
    REQUIRE(default_construction.use_AME == false);
    REQUIRE(default_construction.table_year == 3);
    REQUIRE(default_construction.user_isotopes == "userdata.dat");
  }

  SECTION("Set all values in construction")
  {
    const std::filesystem::path user{ "somedata.dat" };
    const bool AME{ true };
    const int year{ 12 };
    const MassTable construction(user, year, AME);
    REQUIRE(construction.use_AME == AME);
    REQUIRE(construction.table_year == year);
    REQUIRE(construction.user_isotopes == user);
  }
}


TEST_CASE("Alter the table year", "[MassTable]")
{
  const MassTable table("userdata.dat");
  REQUIRE(table.table_year == 3);
  table.setTableYear(12);
  REQUIRE(table.table_year == 12);
}


TEST_CASE("Absolute paths are constructed", "[MassTable]")
{
  const std::filesystem::path datapath = Options::getAbsolutePath();
  const MassTable table("userdata.dat");
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
