#include "inch/converter.hpp"

#include <catch2/catch_all.hpp>


TEST_CASE("String -> int", "[Converter]")
{
  REQUIRE(Converter::StringToInt("abc123def", 3, 6) == 123);
  REQUIRE(Converter::StringToInt("456", 0, 3) == 456);
}


TEST_CASE("String -> double", "[Converter]")
{
  REQUIRE(Converter::StringToDouble("abc1.23def", 3, 7) == Catch::Approx(1.23));
  REQUIRE(Converter::StringToDouble("45.6", 0, 4) == Catch::Approx(45.6));
}


TEST_CASE("CaseCorrection", "[Converter]")
{
  REQUIRE(Converter::caseCorrection("he") == "He");
  REQUIRE(Converter::caseCorrection("N") == "N");
  REQUIRE(Converter::caseCorrection("n") == "n");
  REQUIRE(Converter::caseCorrection("c") == "C");
  REQUIRE(Converter::caseCorrection("AG") == "Ag");
}


TEST_CASE("String/Symbol to Int", "[Converter]")
{
  REQUIRE(Converter::StringToInt("C") == 6);
  REQUIRE(Converter::StringToInt("He") == 2);
  REQUIRE(Converter::StringToInt("Ag") == 47);
}


TEST_CASE("Symbol -> Z", "[Converter]")
{
  SECTION("A valid isotope symbol is given")
  {
    REQUIRE(Converter::SymbolToZ("He") == 2);
    REQUIRE(Converter::SymbolToZ("C") == 6);
    REQUIRE(Converter::SymbolToZ("Ag") == 47);
  }

  SECTION("An invalid isotope symbol is given")
  {
    REQUIRE(Converter::SymbolToZ("Xy") == 200);
    REQUIRE(Converter::SymbolToZ("ab") == 200);
    REQUIRE(Converter::SymbolToZ("IS") == 200);
  }
}


TEST_CASE("Z -> Symbol", "[Converter]")
{
  SECTION("A valid proton number is given")
  {
    REQUIRE(Converter::ZToSymbol(1) == "H");
    REQUIRE(Converter::ZToSymbol(23) == "V");
    REQUIRE(Converter::ZToSymbol(45) == "Rh");
  }

  SECTION("An invalid proton number is given")
  {
    REQUIRE(Converter::ZToSymbol(-2) == "Xy");
    REQUIRE(Converter::ZToSymbol(120) == "Xy");
  }
}


TEST_CASE("Float to string with specified decimal places", "[Converter]")
{
  REQUIRE(Converter::FloatToNdp(1.123456789) == "1.1");
  REQUIRE(Converter::FloatToNdp(1.123456789, 0) == "1");
  REQUIRE(Converter::FloatToNdp(1.123456789, 1) == "1.1");
  REQUIRE(Converter::FloatToNdp(1.123456789, 2) == "1.12");
  REQUIRE(Converter::FloatToNdp(1.123456789, 3) == "1.123");
  /// Rounding now happens
  REQUIRE(Converter::FloatToNdp(1.123456789, 4) == "1.1235");
  REQUIRE(Converter::FloatToNdp(1.123456789, 5) == "1.12346");
  REQUIRE(Converter::FloatToNdp(1.123456789, 6) == "1.123457");
}


// We are implicitly testing the is??Seconds() functions here
// Should we explicitly check them? Probably yes.
TEST_CASE("Human readable time in seconds", "[Converter]")
{
  REQUIRE(Converter::SecondsToHuman(1.123456789e20) == "3560.1 Gyrs");
  REQUIRE(Converter::SecondsToHuman(1.123456789e17) == "3.6 Gyrs");
  REQUIRE(Converter::SecondsToHuman(1.123456789e14) == "3.6 Myrs");
  REQUIRE(Converter::SecondsToHuman(1.123456789e8) == "3.6 yrs");
  REQUIRE(Converter::SecondsToHuman(1.123456789e7) == "130.0 days");
  REQUIRE(Converter::SecondsToHuman(1.123456789e5) == "1.3 days");
  REQUIRE(Converter::SecondsToHuman(1.123456789e4) == "3.1 hrs");
  REQUIRE(Converter::SecondsToHuman(1.123456789e3) == "18.7 mins");
  REQUIRE(Converter::SecondsToHuman(1.123456789e2) == "1.9 mins");
  REQUIRE(Converter::SecondsToHuman(1.123456789e1) == "11.2 s");
  REQUIRE(Converter::SecondsToHuman(1.123456789e0) == "1.1 s");
  REQUIRE(Converter::SecondsToHuman(1.123456789e-1) == "0.1 s");
  REQUIRE(Converter::SecondsToHuman(1.123456789e-2) == "11.2 ms");
  REQUIRE(Converter::SecondsToHuman(1.123456789e-3) == "1.1 ms");
  REQUIRE(Converter::SecondsToHuman(1.123456789e-4) == "0.1 ms");
  /// EPS specific output strings
  REQUIRE(Converter::SecondsToHuman(1.123456789e-5) == "11.2 1 S (u) tw sh");
  REQUIRE(Converter::SecondsToHuman(1.123456789e-6) == "1.1 1 S (u) tw sh");
  REQUIRE(Converter::SecondsToHuman(1.123456789e-7) == "0.1 1 S (u) tw sh");

  REQUIRE(Converter::SecondsToHuman(1.123456789e-8) == "11.2 ns");
  REQUIRE(Converter::SecondsToHuman(1.123456789e-9) == "1.1 ns");
  REQUIRE(Converter::SecondsToHuman(1.123456789e-10) == "0.1 ns");
  REQUIRE(Converter::SecondsToHuman(1.123456789e-11) == "11.2 ps");
}


TEST_CASE("Show energy in keV or MeV", "[Converter]")
{
  REQUIRE(Converter::IsomerEnergyToHuman(123.45) == "123.5 keV");
  REQUIRE(Converter::IsomerEnergyToHuman(123.45, 2) == "123.45 keV");
  REQUIRE(Converter::IsomerEnergyToHuman(1234.5, 2) == "1.23 MeV");
  REQUIRE(Converter::IsomerEnergyToHuman(5678.9) == "5.7 MeV");
}


TEST_CASE("Scientific float to tuple of strings", "[Converter]")
{
  auto out = Converter::FloatToExponent(1.0e+5);
  REQUIRE(std::get<0>(out) == "1.0");
  REQUIRE(std::get<1>(out) == "+");
  REQUIRE(std::get<2>(out) == "05");

  out = Converter::FloatToExponent(2.0e-5);
  REQUIRE(std::get<0>(out) == "2.0");
  REQUIRE(std::get<1>(out) == "-");
  REQUIRE(std::get<2>(out) == "05");

  out = Converter::FloatToExponent(-2.0e-5);
  REQUIRE(std::get<0>(out) == "-2.0");
  REQUIRE(std::get<1>(out) == "-");
  REQUIRE(std::get<2>(out) == "05");

  out = Converter::FloatToExponent(1.2345e+13);
  REQUIRE(std::get<0>(out) == "1.2");
  REQUIRE(std::get<1>(out) == "+");
  REQUIRE(std::get<2>(out) == "13");
}
