#include <catch/catch.hpp>

#include "converter.hpp"

const Converter converter;


TEST_CASE( "Symbol -> Z", "[Converter]" )
{
  REQUIRE( converter.convertSymbolToZ("He") == 2 );
  REQUIRE( converter.convertSymbolToZ("C") == 6 );
  REQUIRE( converter.convertSymbolToZ("Ag") == 47 );
}


TEST_CASE( "Z -> Symbol", "[Converter]" )
{
  REQUIRE_THAT( converter.convertZToSymbol(1)  , Catch::Matches("H") );
  REQUIRE_THAT( converter.convertZToSymbol(23) , Catch::Matches("V") );
  REQUIRE_THAT( converter.convertZToSymbol(45) , Catch::Matches("Rh") );
}


TEST_CASE( "Float to string with specified decimal places", "[Converter]")
{
  REQUIRE_THAT( converter.FloatToNdp(1.123456789), Catch::Matches("1.1") );
  REQUIRE_THAT( converter.FloatToNdp(1.123456789, 0), Catch::Matches("1") );
  REQUIRE_THAT( converter.FloatToNdp(1.123456789, 1), Catch::Matches("1.1") );
  REQUIRE_THAT( converter.FloatToNdp(1.123456789, 5), Catch::Matches("1.12345") );
  /// Rounding now happens
  REQUIRE_THAT( converter.FloatToNdp(1.123456789, 6), Catch::Matches("1.123457") );
}


TEST_CASE( "Human readable time in seconds", "[Converter]")
{
  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e20), Catch::Matches("3562.4 Gyrs") );
  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e17), Catch::Matches("3.5 Gyrs") );
  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e8), Catch::Matches("3.5 yrs") );
  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e7), Catch::Matches("130.0 days") );
  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e5), Catch::Matches("1.3 days") );
  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e4), Catch::Matches("3.1 hrs") );
  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e3), Catch::Matches("18.7 mins") );
  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e2), Catch::Matches("1.8 mins") );
  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e1), Catch::Matches("11.2 s") );
  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e0), Catch::Matches("1.1 s") );
  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e-1), Catch::Matches("0.1 s") );
  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e-2), Catch::Matches("11.2 ms") );
  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e-3), Catch::Matches("1.1 ms") );
  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e-4), Catch::Matches("0.1 ms") );
  /// EPS specific output strings
  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e-5), Catch::Equals("11.2 1 S (u) tw sh") );
  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e-6), Catch::Equals("1.1 1 S (u) tw sh") );
  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e-7), Catch::Equals("0.1 1 S (u) tw sh") );

  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e-8), Catch::Matches("11.2 ns") );
  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e-9), Catch::Matches("1.1 ns") );
  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e-10), Catch::Matches("0.1 ns") );
  REQUIRE_THAT( converter.SecondsToHuman(1.123456789e-11), Catch::Matches("11.2 ps") );
}


TEST_CASE( "Show energy in keV or MeV", "[Converter]" )
{
  REQUIRE_THAT( converter.IsomerEnergyToHuman(123.45), Catch::Matches("123.4 keV") );
  REQUIRE_THAT( converter.IsomerEnergyToHuman(1234.5), Catch::Matches("1.2 MeV") );
}


TEST_CASE( "Scientific float to tuple of strings", "[Converter]" )
{
  auto out = converter.FloatToExponent(1.0e+5);
  REQUIRE_THAT( std::get<0>(out), Catch::Equals("1.0"));
  REQUIRE_THAT( std::get<1>(out), Catch::Equals("+"));
  REQUIRE_THAT( std::get<2>(out), Catch::Equals("05"));

  out = converter.FloatToExponent(2.0e-5);
  REQUIRE_THAT( std::get<0>(out), Catch::Equals("2.0"));
  REQUIRE_THAT( std::get<1>(out), Catch::Equals("-"));
  REQUIRE_THAT( std::get<2>(out), Catch::Equals("05"));

  out = converter.FloatToExponent(-2.0e-5);
  REQUIRE_THAT( std::get<0>(out), Catch::Equals("-2.0"));
  REQUIRE_THAT( std::get<1>(out), Catch::Equals("-"));
  REQUIRE_THAT( std::get<2>(out), Catch::Equals("05"));

  out = converter.FloatToExponent(1.2345e+13);
  REQUIRE_THAT( std::get<0>(out), Catch::Equals("1.2"));
  REQUIRE_THAT( std::get<1>(out), Catch::Equals("+"));
  REQUIRE_THAT( std::get<2>(out), Catch::Equals("13"));
}
