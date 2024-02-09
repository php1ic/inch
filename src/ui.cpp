#include "inch/ui.hpp"

#include "inch/allNeutrons.hpp"
#include "inch/chartColour.hpp"
#include "inch/chartSelection.hpp"
#include "inch/chartType.hpp"
#include "inch/limits.hpp"
#include "inch/massTable.hpp"
#include "inch/options.hpp"

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/os.h>
#include <fmt/std.h>

#include <iostream>
#include <limits>
#include <string>


// NOLINTBEGIN(bugprone-easily-swappable-parameters)
size_t UI::genericQuestion(const std::string& theQuestion,
                           const std::vector<std::string>& theOptions,
                           const int fallback,
                           const int attempts)
// NOLINTEND(bugprone-easily-swappable-parameters)
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


// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
bool UI::yesNoQuestion(const std::string& question, const std::string& fallback, const int attempts)
{
  std::string answer{ 'n' };
  bool valid_choice{ true };

  // Security against the code being run in a script and getting into an infinite loop
  const int max_allowed_incorrect_responses{ attempts };
  int incorrect_responses{ 0 };

  do
    {
      valid_choice = true;
      fmt::print("{} [y/n]: ", question);
      std::cin >> answer;

      if (answer != "y" && answer != "n")
        {
          fmt::print("That wasn't y or n, try again.\n");
          ++incorrect_responses;
          valid_choice = false;
        }
    }
  while (!valid_choice && incorrect_responses < max_allowed_incorrect_responses);

  if (incorrect_responses >= max_allowed_incorrect_responses)
    {
      fmt::print("\nA 2 option question was answered incorrectly more than {} times.\n"
                 "Falling back to the default answer of {}\n",
                 max_allowed_incorrect_responses,
                 fallback);

      answer = fallback;
    }

  return (answer == "y");
}


void UI::askQuestions(const MassTable& table) const
{
  user_options.chart_selection = selectChartSelection();
  selectZandNLimits(user_options.chart_selection, table);

  user_options.chart_type   = selectChartType();
  user_options.chart_colour = selectChartColour();
}


ChartSelection UI::selectChartSelection()
{
  const std::vector<std::string> choices{ "The entire chart", "A range in Z" };

  const auto choice = genericQuestion("What should be drawn", choices, 0, 3);

  return (choice == 0) ? ChartSelection::FULL_CHART : ChartSelection::SUB_CHART;
}


ChartType UI::selectChartType()
{
  const std::vector<std::string> choices{ "Experimentally measured only",
                                          "Theoretical/Extrapolated values only",
                                          "Both" };

  const auto choice = genericQuestion("Display which nuclei", choices, 2, 5);

  return [=]() {
    switch (choice)
      {
        case 0:
          return ChartType::EXPERIMENTAL;
        case 1:
          return ChartType::THEORETICAL;
        case 2:
        default:
          return ChartType::ALL;
      }
  }();
}


AllNeutrons UI::selectWhichNeutronRange()
{
  const std::vector<std::string> sub_choices{ "All required neutron", "A range in N" };

  const auto choice = genericQuestion("Which neutron range", sub_choices, 0, 3);

  return (choice == 0) ? AllNeutrons::YES : AllNeutrons::NO;
}


void UI::selectZandNLimits(const ChartSelection selection, const MassTable& table) const
{
  if (selection == ChartSelection::FULL_CHART)
    {
      user_options.limits.ResetAllLimits();
    }
  else if (selection == ChartSelection::SUB_CHART)
    {
      fmt::print("---------------------------\n"
                 "Enter range of Z, by symbol [n,Ei] or number [0,{}]\n",
                 Limits::MAX_Z);

      user_options.limits.setExtreme("Zmin");

      user_options.limits.setExtreme("Zmax");

      user_options.all_neutrons = selectWhichNeutronRange();

      switch (user_options.all_neutrons)
        {
          case AllNeutrons::YES:
          default:
            user_options.setNeutronLimits(table.theTable);
            break;
          case AllNeutrons::NO:
            table.setUserNeutronRange();
            break;
        }
    }
}


ChartColour UI::selectChartColour() const
{
  std::vector<std::string> choices{ "Error on Mass-Excess", "Relative Error on Mass-Excess (dm/m)" };

  if (!user_options.AME)
    {
      choices.emplace_back("Major Ground-State Decay Mode");
      choices.emplace_back("Ground-State Half-Life");

      if (user_options.chart_type != ChartType::THEORETICAL)
        {
          choices.emplace_back("First Isomer Energy");
        }
    }

  const auto choice = genericQuestion("Colour by which property", choices, 0, 5);

  return [=]() {
    switch (choice)
      {
        case 0:
        default:
          return ChartColour::MASSEXCESSERROR;
        case 1:
          return ChartColour::REL_MASSEXCESSERROR;
        case 2:
          return ChartColour::GS_DECAYMODE;
        case 3:
          return ChartColour::GS_HALFLIFE;
        case 4:
          return ChartColour::FIRST_ISOMERENERGY;
      }
  }();
}
