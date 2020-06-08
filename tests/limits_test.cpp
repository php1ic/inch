#include "inch/limits.hpp"

#include <catch2/catch.hpp>

// TEST_CASE("", "[Limits]")
//{
//}


TEST_CASE("Inital values are set correctly", "[Limits]")
{
  const Limits l;
  REQUIRE(l.Zmin == 0);
  REQUIRE(l.Zmax == 118);
  REQUIRE(l.Nmin == 0);
  REQUIRE(l.Nmax == 177);
}


TEST_CASE("Zmin limit set correctly", "[Limits]")
{
  const Limits limits;
  auto isMinSet = limits.setZmin(23);
  auto isMaxSet = limits.setZmax(56);

  SECTION("Zmin is set with a valid number")
  {
    REQUIRE(isMinSet);
    REQUIRE(isMaxSet);
    REQUIRE(limits.Zmin == 23);
  }

  SECTION("Zmin is not set with an invalid number")
  {
    REQUIRE_FALSE(limits.setZmin(-5));
    REQUIRE_FALSE(limits.setZmin(120));
    REQUIRE_FALSE(limits.setZmin(57));
    REQUIRE(limits.Zmin == 23);
  }
}


TEST_CASE("Zmax limit set correctly", "[Limits]")
{
  const Limits limits;
  auto isMinSet = limits.setZmin(12);
  auto isMaxSet = limits.setZmax(89);

  SECTION("Zmax is set with a valid number")
  {
    REQUIRE(isMinSet);
    REQUIRE(isMaxSet);
    REQUIRE(limits.Zmax == 89);
  }

  SECTION("Zmax is not with an invalid number")
  {
    REQUIRE_FALSE(limits.setZmax(-5));
    REQUIRE_FALSE(limits.setZmax(120));
    REQUIRE_FALSE(limits.setZmax(10));
    REQUIRE(limits.Zmax == 89);
  }
}


TEST_CASE("Nmin limit set correctly", "[Limits]")
{
  const Limits limits;
  auto isMinSet = limits.setNmin(45);
  auto isMaxSet = limits.setNmax(67);

  SECTION("Nmin is set with a valid number")
  {
    REQUIRE(isMinSet);
    REQUIRE(isMaxSet);
    REQUIRE(limits.Nmin == 45);
  }

  SECTION("Nmin is not set with an invalid number")
  {
    REQUIRE_FALSE(limits.setNmin(-5));
    REQUIRE_FALSE(limits.setNmin(180));
    REQUIRE_FALSE(limits.setNmin(80));
    REQUIRE(limits.Nmin == 45);
  }
}


TEST_CASE("Nmax limit set correctly", "[Limits]")
{
  const Limits limits;
  auto isMinSet = limits.setNmin(121);
  auto isMaxSet = limits.setNmax(145);

  SECTION("Nmax is set with a valid number")
  {
    REQUIRE(isMinSet);
    REQUIRE(isMaxSet);
    REQUIRE(limits.Nmax == 145);
  }

  SECTION("Nmax is not with an invalid number")
  {
    REQUIRE_FALSE(limits.setNmax(-5));
    REQUIRE_FALSE(limits.setNmax(180));
    REQUIRE_FALSE(limits.setNmax(111));
    REQUIRE(limits.Nmax == 145);
  }
}


TEST_CASE("The Z limits are reset correctly", "[Limits]")
{
  const Limits limits;
  limits.Zmin = 45;
  limits.Zmax = 89;

  limits.ResetZLimits();

  REQUIRE(limits.Zmin == 0);
  REQUIRE(limits.Zmax == 118);
}


TEST_CASE("The N limtis are reset correctly", "[Limits]")
{
  const Limits limits;
  limits.Nmin = 56;
  limits.Nmax = 123;

  limits.ResetNLimits();

  REQUIRE(limits.Nmin == 0);
  REQUIRE(limits.Nmax == 177);
}


TEST_CASE("All limits are reset correctly", "[Limits]")
{
  const Limits limits;
  limits.Zmin = 12;
  limits.Zmax = 89;
  limits.Nmin = 78;
  limits.Nmax = 156;

  limits.ResetAllLimits();

  REQUIRE(limits.Zmin == 0);
  REQUIRE(limits.Zmax == 118);
  REQUIRE(limits.Nmin == 0);
  REQUIRE(limits.Nmax == 177);
}


TEST_CASE("Z range check is correct", "[Limits]")
{
  const Limits limits;
  limits.Zmin = 56;
  limits.Zmax = 78;

  REQUIRE(limits.inZRange(56));
  REQUIRE(limits.inZRange(78));
  REQUIRE(limits.inZRange(60));
  REQUIRE_FALSE(limits.inZRange(5));
  REQUIRE_FALSE(limits.inZRange(100));
}


TEST_CASE("N range check is correct", "[Limits]")
{
  const Limits limits;
  limits.Nmin = 123;
  limits.Nmax = 167;

  REQUIRE(limits.inNRange(123));
  REQUIRE(limits.inNRange(167));
  REQUIRE(limits.inNRange(150));
  REQUIRE_FALSE(limits.inNRange(100));
  REQUIRE_FALSE(limits.inNRange(170));
}

TEST_CASE("Get correct Z range", "[Limits]")
{
  const Limits limits;
  limits.Zmin = 50;
  limits.Zmax = 100;

  REQUIRE(limits.getZRange() == 50);
  REQUIRE_FALSE(limits.getZRange() == 89);
}


TEST_CASE("Get correct N range", "[Limits]")
{
  const Limits limits;
  limits.Nmin = 23;
  limits.Nmax = 125;

  REQUIRE(limits.getNRange() == 102);
  REQUIRE_FALSE(limits.getNRange() == 56);
}
