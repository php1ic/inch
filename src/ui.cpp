#include "inch/ui.hpp"

#include "inch/converter.hpp"
#include "inch/limits.hpp"

#include <fmt/format.h>

#include <iostream>
#include <string>


void UI::askQuestions() const
{
  selectChartSelection();
  selectChartType();
  selectChartColour();
}


void UI::setExtreme(std::string_view limit) const
{
  if (limit != "Zmin" && limit != "Zmax" && limit != "Nmin" && limit != "Nmax")
    {
      fmt::print("**WARNING** - {} is not a valid input\n"
                 "              choose either Zmin, Zmax, Nmin or Nmax\n"
                 "Setting limits to maxima values\n",
                 limit);

      options.Zmin = MIN_Z;
      options.Zmax = MAX_Z;
      options.Nmin = MIN_N;
      options.Nmax = MAX_N;

      return;
    }

  bool valid{ false };

  do
    {
      int number{ 0 };
      std::string in;

      fmt::print("{}: ", limit);
      std::cin >> in;

      // Read the entered value and convert symbol->Z if necessary
      try
        {
          number = std::stoi(in);
        }
      catch (const std::invalid_argument& ia)
        {
          const Converter converter;
          number = (limit.front() == 'Z') ? converter.convertSymbolToZ(in) : -1;
        }

      // Validate the number
      if (limit == "Zmin")
        {
          options.Zmin = number;
          valid        = true;
          if (options.Zmin < Limits::MIN_Z || options.Zmin > Limits::MAX_Z)
            {
              fmt::print("\nZmin must be in the range {}<Z<{}\n\n", Limits::MIN_Z, Limits::MAX_Z);
              valid = false;
            }
        }
      else if (limit == "Zmax")
        {
          options.Zmax = number;
          valid        = true;
          if (options.Zmax < options.Zmin || options.Zmax > Limits::MAX_Z)
            {
              fmt::print("\nZmax must be in the range {}<Z<{}\n\n", options.Zmin, Limits::MAX_Z);
              valid = false;
            }
        }
      else if (limit == "Nmin")
        {
          options.Nmin = number;
          valid        = true;
          if (options.Nmin < Limits::MIN_N || options.Nmin > Limits::MAX_N)
            {
              fmt::print("\nNmin must be in the range {}<N<{}\n\n", Limits::MIN_N, Limits::MAX_N);
              valid = false;
            }
        }
      else if (limit == "Nmax")
        {
          options.Nmax = number;
          valid        = true;
          if (options.Nmax < options.Nmin || options.Nmax > Limits::MAX_N)
            {
              fmt::print("\nNmax must be in the range {}<N<{}\n\n", options.Nmin, Limits::MAX_N);
              valid = false;
            }
        }
    }
  while (!valid);
}


void UI::selectChartType() const
{
  fmt::print("---------------------------\n"
             "Display which nuclei?\n"
             "a) Experimentally measured only\n"
             "b) Theoretical/Extrapolated values only\n"
             "c) Both\n");

  bool validChoice{ false };

  do
    {
      validChoice = false;
      std::string type;
      fmt::print("Which: ");
      std::cin >> type;

      if (type == "a")
        {
          validChoice        = true;
          options.chart_type = ChartType::EXPERIMENTAL;
        }
      else if (type == "b")
        {
          validChoice        = true;
          options.chart_type = ChartType::THEORETICAL;
        }
      else if (type == "c")
        {
          validChoice        = true;
          options.chart_type = ChartType::ALL;
        }

      if (!validChoice)
        {
          fmt::print("\nThat wasn't one of the optins, Try again\n");
        }
    }
  while (!validChoice);
}


void UI::selectChartColour() const
{
  bool validChoice{ false };

  fmt::print("---------------------------\n"
             "Colour by which property?\n"
             "a) Error on Mass-Excess\n"
             "b) Relative Error on Mass-Excess (dm/m)\n");

  if (!options.AME)
    {
      fmt::print("c) Major Ground-State Decay Mode\n"
                 "d) Ground-State Half-Life\n");

      if (options.chart_type != ChartType::THEORETICAL)
        {
          fmt::print("e) First Isomer Energy\n");
        }
    }

  while (!validChoice)
    {
      validChoice = true;
      std::string choice;
      fmt::print("Choice: ");
      std::cin >> choice;

      if (choice == "a")
        {
          options.chart_colour = ChartColour::MASSEXCESSERROR;
        }
      else if (choice == "b")
        {
          options.chart_colour = ChartColour::REL_MASSEXCESSERROR;
        }
      else if (choice == "c")
        {
          options.chart_colour = ChartColour::GS_DECAYMODE;
        }
      else if (choice == "d")
        {
          options.chart_colour = ChartColour::GS_HALFLIFE;
        }
      else if (choice == "e")
        {
          options.chart_colour = ChartColour::FIRST_ISOMERENERGY;
        }
      else
        {
          validChoice = false;
        }

      if (options.AME)
        {
          if (choice != "a" && choice != "b")
            {
              fmt::print("\nThat wasn't one of the options. Try again\n");
              validChoice = false;
            }
        }
      else if ((options.chart_type != ChartType::THEORETICAL && !validChoice)
               || (options.chart_type == ChartType::THEORETICAL
                   && options.chart_colour == ChartColour::FIRST_ISOMERENERGY))
        {
          fmt::print("\nThat wasn't one of the options. Try again\n");
          validChoice = false;
        }
    }
}


void UI::selectChartSelection() const
{
  int stblZmin{ Limits::MAX_N };
  int stblZmax{ Limits::MIN_N };

  int NminZmin{ Limits::MAX_N };
  int NminZmax{ Limits::MAX_N };

  int NmaxZmin{ Limits::MIN_N };
  int NmaxZmax{ Limits::MIN_N };

  bool validChoice{ false };

  fmt::print("\n---------------------------\n"
             "Draw a) The entire chart\n"
             "     b) A section\n");

  do
    {
      std::string section;

      do
        {
          validChoice = true;
          fmt::print("[a,b]: ");
          std::cin >> section;

          if (section != "a" && section != "b")
            {
              validChoice = false;
              fmt::print("\nThat wasn't one of the options. Try again.\n");
            }
        }
      while (!validChoice);

      options.chart_selection = (section == "a") ? ChartSelection::FULL_CHART : ChartSelection::SUB_CHART;

      if (options.chart_selection == ChartSelection::FULL_CHART)
        {
          options.Zmin = Limits::MIN_Z;
          options.Nmin = Limits::MIN_N;
          options.Zmax = Limits::MAX_Z;
          options.Nmax = Limits::MAX_N;
        }
      else if (options.chart_selection == ChartSelection::SUB_CHART)
        {
          fmt::print("---------------------------\n"
                     "Enter range of Z, by symbol [n,Ei] or number [0,{}]\n",
                     Limits::MAX_Z);

          setExtreme("Zmin");

          setExtreme("Zmax");

          fmt::print("---------------------------\n"
                     "Draw a) All required N\n"
                     "     b) A section\n");

          std::string required;
          do
            {
              fmt::print("[a,b]: ");
              std::cin >> required;

              if (required == "a")
                {
                  options.all_neutrons = AllNeutrons::YES;
                  options.setNeutronLimits(table);
                }
              else if (required == "b")
                {
                  options.all_neutrons = AllNeutrons::NO;
                  for (const auto& it : table)
                    {
                      // Set N range for Zmin
                      if (it.Z == options.Zmin)
                        {
                          if (it.N < NminZmin)
                            {
                              NminZmin = it.N;
                            }
                          else if (it.N > NmaxZmin)
                            {
                              NmaxZmin = it.N;
                            }
                        }
                      // Set N range for Zmax
                      else if (it.Z == options.Zmax)
                        {
                          if (it.N < NminZmax)
                            {
                              NminZmax = it.N;
                            }
                          else if (it.N > NmaxZmax)
                            {
                              NmaxZmax = it.N;
                            }
                        }
                    }

                  // Set high/low stable N for Zmax/Zmin
                  for (const auto& it : table)
                    {
                      if (it.N >= NminZmin && it.N <= NmaxZmax && it.decay == "stable")
                        {
                          if (it.Z == options.Zmin && it.N < stblZmin)
                            {
                              stblZmin = it.N;
                            }

                          if (it.Z == options.Zmax && it.N > stblZmax)
                            {
                              stblZmax = it.N;
                            }
                        }
                    }

                  const Converter converter;
                  fmt::print("---------------------------\n"
                             "Enter range of N [0,{}]\n"
                             "{}({}) has N from {} to {}",
                             Limits::MAX_N,
                             converter.convertZToSymbol(options.Zmin),
                             options.Zmin,
                             NminZmin,
                             NmaxZmin);

                  if (options.Zmin > 83 || options.Zmin == 43 || options.Zmin == 0)
                    {
                      fmt::print(" with no stable isotope\n");
                    }
                  else
                    {
                      fmt::print(" and the lightest stable isotope has N={}\n", stblZmin);
                    }

                  setExtreme("Nmin");

                  fmt::print("{}({}) has N from {} to {}",
                             converter.convertZToSymbol(options.Zmax),
                             options.Zmax,
                             NminZmax,
                             NmaxZmax);

                  if (options.Zmax > 83 || options.Zmax == 43 || options.Zmax == 0)
                    {
                      fmt::print(" with no stable isotope\n");
                    }
                  else
                    {
                      fmt::print(" and the heaviest stable isotope has N={}\n", stblZmax);
                    }

                  setExtreme("Nmax");
                }
              else
                {
                  fmt::print("\nThat wasn't one of the options. Try again.");
                }
            }
          while (required != "a" && required != "b");
        }
      else
        {
          fmt::print("\nThat wasn't one of the options. Try again.");
        }
    }
  while (!validChoice);
}
