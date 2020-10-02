#include "inch/ui.hpp"

#include "inch/allNeutrons.hpp"
#include "inch/chartColour.hpp"
#include "inch/chartSelection.hpp"
#include "inch/chartType.hpp"
#include "inch/converter.hpp"
#include "inch/limits.hpp"
#include "inch/options.hpp"

#include <fmt/core.h>
#include <fmt/format.h>

#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <regex>
#include <string>


size_t UI::genericQuestion(const std::string& theQuestion,
                           const std::vector<std::string>& theOptions,
                           const int fallback,
                           const int attempts) const
{
  size_t answer{ 0 };
  bool valid_choice{ true };

  // Security against the code being run in a script and getting into an infinite loop
  const int max_allowed_incorrect_responses{ attempts };
  int incorrect_responses{ 0 };

  fmt::print("---------------------\n"
             "{}\n",
             theQuestion);

  for (size_t i = 0; i < theOptions.size(); ++i)
    {
      fmt::print("{}) {}\n", i, theOptions.at(i));
    }

  do
    {
      valid_choice = true;
      fmt::print("Choice: ");

      if (!(std::cin >> answer))
        {
          ++incorrect_responses;
          valid_choice = false;
          std::cin.clear();
          std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        }

      // answer is of type size_t so can't be negative
      if (answer >= theOptions.size() || !valid_choice)
        {
          fmt::print("That wasn't one of the options, try again.\n");
          ++incorrect_responses;
          valid_choice = false;
        }
    }
  while (!valid_choice && incorrect_responses < max_allowed_incorrect_responses);

  if (incorrect_responses >= max_allowed_incorrect_responses)
    {
      fmt::print("\nAn invalid option has been given {} times.\n"
                 "Falling back to the default answer of {}\n",
                 max_allowed_incorrect_responses,
                 fallback);

      answer = fallback;
    }

  return answer;
}


void UI::askQuestions() const
{
  selectChartSelection();
  selectChartType();
  selectChartColour();
}


void UI::selectChartType() const
{
  const std::vector<std::string> choices{ "Experimentally measured only",
                                          "Theoretical/Extrapolated values only",
                                          "Both" };

  const auto choice = genericQuestion("Display which nuclei", choices, 0, 5);

  switch (choice)
    {
      case 0:
        options.chart_type = ChartType::EXPERIMENTAL;
        break;
      case 1:
        options.chart_type = ChartType::THEORETICAL;
        break;
      case 2:
      default:
        options.chart_type = ChartType::ALL;
        break;
    }
}


void UI::selectChartColour() const
{
  std::vector<std::string> choices{ "Error on Mass-Excess", "Relative Error on Mass-Excess (dm/m)" };

  if (!options.AME)
    {
      choices.emplace_back("Major Ground-State Decay Mode");
      choices.emplace_back("Ground-State Half-Life");

      if (options.chart_type != ChartType::THEORETICAL)
        {
          choices.emplace_back("First Isomer Energy");
        }
    }

  const auto choice = genericQuestion("Colour by which property", choices, 0, 5);

  switch (choice)
    {
      case 0:
      default:
        options.chart_colour = ChartColour::MASSEXCESSERROR;
        break;
      case 1:
        options.chart_colour = ChartColour::REL_MASSEXCESSERROR;
        break;
      case 2:
        options.chart_colour = ChartColour::GS_DECAYMODE;
        break;
      case 3:
        options.chart_colour = ChartColour::GS_HALFLIFE;
        break;
      case 4:
        options.chart_colour = ChartColour::FIRST_ISOMERENERGY;
        break;
    }
}


std::pair<int, int> UI::GetNeutronRange(const int Z, const std::string& decayMode) const
{
  int Nmin{ Limits::MAX_N };
  int Nmax{ Limits::MIN_N };

  const std::regex decay(decayMode);

  for (const auto& isotope : table)
    {
      if (isotope.Z == Z && std::regex_search(isotope.decay, decay))
        {
          if (isotope.N < Nmin)
            {
              Nmin = isotope.N;
            }
          else if (isotope.N > Nmax)
            {
              Nmax = isotope.N;
            }
        }
    }

  return std::make_pair(Nmin, Nmax);
}


void UI::SetNeutronLimitForZ(const int Z, std::string_view limit) const
{
  const auto Nrange = GetNeutronRange(Z);

  fmt::print("{}({}) has N from {} to {}", Converter::ZToSymbol(Z), Z, Nrange.first, Nrange.second);

  if (Z > 83 || Z == 43 || Z == 0)
    {
      fmt::print(" with no stable isotope\n");
    }
  else
    {
      const auto stableNrange = GetStableNeutronRange(Z);
      fmt::print(" and the {} stable isotope has N={}\n",
                 limit == "Nmin" ? "lightest" : "heaviest",
                 limit == "Nmin" ? stableNrange.first : stableNrange.second);
    }

  options.limits.setExtreme(limit);
}


void UI::setUserNeutronRange() const
{
  fmt::print("---------------------------\n"
             "Enter range of N [0,{}]\n",
             Limits::MAX_N);

  // Bottom left (N,Z)
  SetNeutronLimitForZ(options.limits.Zmin, "Nmin");
  // Top right (N,Z)
  SetNeutronLimitForZ(options.limits.Zmax, "Nmax");
}


void UI::selectChartSelection() const
{
  const std::vector<std::string> choices{ "The entire chart", "A range in Z" };

  const auto choice = genericQuestion("What should be drawn", choices, 0, 3);

  options.chart_selection = (choice == 0) ? ChartSelection::FULL_CHART : ChartSelection::SUB_CHART;

  if (options.chart_selection == ChartSelection::FULL_CHART)
    {
      options.limits.ResetAllLimits();
    }
  else if (options.chart_selection == ChartSelection::SUB_CHART)
    {
      fmt::print("---------------------------\n"
                 "Enter range of Z, by symbol [n,Ei] or number [0,{}]\n",
                 Limits::MAX_Z);

      options.limits.setExtreme("Zmin");

      options.limits.setExtreme("Zmax");

      const std::vector<std::string> sub_choices{ "All required neutron", "A range in N" };

      const auto subChoice = genericQuestion("Which neutron range", sub_choices, 0, 3);

      switch (subChoice)
        {
          case 0:
          default:
            options.all_neutrons = AllNeutrons::YES;
            options.setNeutronLimits(table);
            break;
          case 1:
            options.all_neutrons = AllNeutrons::NO;
            setUserNeutronRange();
            break;
        }
    }
}
