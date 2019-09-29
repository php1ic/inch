#include "ui.hpp"

#include "converter.hpp"
#include "limits.hpp"

#include <iostream>
#include <string>


void UI::askQuestions()
{
  selectChartSelection();
  selectChartType();
  selectChartColour();
}


void UI::setExtreme(std::string_view limit) const
{
  if (limit != "Zmin" && limit != "Zmax" && limit != "Nmin" && limit != "Nmax")
    {
      std::cout << "**WARNING** - " << limit << " is not a valid input\n"
                << "              choose either Zmin, Zmax, Nmin or Nmax\n"
                << "Setting limits to maxima values." << std::endl;

      options.Zmin = MIN_Z;
      options.Zmax = MAX_Z;
      options.Nmin = MIN_N;
      options.Nmax = MAX_N;

      return;
    }

  bool valid = false;

  do
    {
      int number = 0;
      std::string in;

      std::cout << limit << ": ";
      std::cin >> in;

      // Read the entered value and convert symbol->Z if necessary
      try
        {
          number = stoi(in);
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
              std::cout << "\n"
                        << "Zmin must be in the range " << Limits::MIN_Z << "<Z<" << Limits::MAX_Z << "\n"
                        << std::endl;
              valid = false;
            }
        }
      else if (limit == "Zmax")
        {
          options.Zmax = number;
          valid        = true;
          if (options.Zmax < options.Zmin || options.Zmax > Limits::MAX_Z)
            {
              std::cout << "\n"
                        << "Zmax must be in the range " << options.Zmin << "<Z<" << Limits::MAX_Z << "\n"
                        << std::endl;
              valid = false;
            }
        }
      else if (limit == "Nmin")
        {
          options.Nmin = number;
          valid        = true;
          if (options.Nmin < Limits::MIN_N || options.Nmin > Limits::MAX_N)
            {
              std::cout << "\n"
                        << "Nmin must be in the range " << Limits::MIN_N << "<N<" << Limits::MAX_N << "\n"
                        << std::endl;
              valid = false;
            }
        }
      else if (limit == "Nmax")
        {
          options.Nmax = number;
          valid        = true;
          if (options.Nmax < options.Nmin || options.Nmax > Limits::MAX_N)
            {
              std::cout << "\n"
                        << "Nmax must be in the range " << options.Nmin << "<N<" << Limits::MAX_N << "\n"
                        << std::endl;
              valid = false;
            }
        }
    }
  while (!valid);
}


void UI::selectChartType() const
{
  std::cout << "---------------------------\n"
            << "Display which nuclei?\n"
            << "a) Experimentally measured only\n"
            << "b) Theoretical/Extrapolated values only\n"
            << "c) Both\n";


  bool validChoice = false;

  do
    {
      validChoice = true;
      std::string type;
      std::cout << "Which: ";
      std::cin >> type;

      if (type == "a")
        {
          options.chart_type = ChartType::EXPERIMENTAL;
        }
      else if (type == "b")
        {
          options.chart_type = ChartType::THEORETICAL;
        }
      else
        {
          options.chart_type = ChartType::ALL;
        }

      if (type != "a" && type != "b" && type != "c")
        {
          std::cout << "\nThat wasn't one of the options. Try again" << std::endl;
          validChoice = false;
        }
    }
  while (!validChoice);
}


void UI::selectChartColour() const
{
  // ChartColour chart_colour;
  bool validChoice = false;

  std::cout << "---------------------------\n"
            << "Colour by which property?\n"
            << "a) Error on Mass-Excess\n"
            << "b) Relative Error on Mass-Excess (dm/m)\n";

  if (!options.AME)
    {
      std::cout << "c) Major Ground-State Decay Mode\n"
                << "d) Ground-State Half-Life\n";

      if (options.chart_type != ChartType::THEORETICAL)
        {
          std::cout << "e) First Isomer Energy\n";
        }
    }

  while (!validChoice)
    {
      validChoice = true;
      std::string choice;
      std::cout << "Choice: ";
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
              std::cout << "\nThat wasn't one of the options. Try again" << std::endl;
              validChoice = false;
            }
        }
      else
        {
          if (options.chart_type != ChartType::THEORETICAL && !validChoice)
            {
              std::cout << "\nThat wasn't one of the options. Try again" << std::endl;
              validChoice = false;
            }
          else if (options.chart_type == ChartType::THEORETICAL
                   && options.chart_colour == ChartColour::FIRST_ISOMERENERGY)
            {
              std::cout << "\nThat wasn't one of the options. Try again" << std::endl;
              validChoice = false;
            }
        }
    }
}


void UI::selectChartSelection() const
{
  int stblZmin = Limits::MAX_N;
  int stblZmax = Limits::MIN_N;

  int NminZmin = Limits::MAX_N;
  int NminZmax = Limits::MAX_N;

  int NmaxZmin = Limits::MIN_N;
  int NmaxZmax = Limits::MIN_N;

  bool validChoice = false;

  std::cout << "\n---------------------------\n"
            << "Draw a) The entire chart\n"
            << "     b) A section\n";

  do
    {
      std::string section;

      do
        {
          validChoice = true;
          std::cout << "[a,b]: ";
          std::cin >> section;

          if (section != "a" && section != "b")
            {
              validChoice = false;
              std::cout << "\nThat wasn't one of the options. Try again." << std::endl;
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
          std::cout << "---------------------------\n"
                    << "Enter range of Z, by symbol [n,Ei] or number [0," << Limits::MAX_Z << "]\n";

          setExtreme("Zmin");

          setExtreme("Zmax");

          std::cout << "---------------------------\n"
                    << "Draw a) All required N\n"
                    << "     b) A section\n";

          std::string required;
          do
            {
              std::cout << "[a,b]: ";
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
                  std::cout << "---------------------------\n"
                            << "Enter range of N [0," << Limits::MAX_N << "]\n"
                            << converter.convertZToSymbol(options.Zmin) << "(" << options.Zmin << ") has N from "
                            << NminZmin << " to " << NmaxZmin;

                  if (options.Zmin > 83 || options.Zmin == 43 || options.Zmin == 0)
                    {
                      std::cout << " with no stable isotope\n";
                    }
                  else
                    {
                      std::cout << " and the lightest stable isotope has N=" << stblZmin << "\n";
                    }

                  setExtreme("Nmin");

                  std::cout << converter.convertZToSymbol(options.Zmax) << "(" << options.Zmax << ") has N from "
                            << NminZmax << " to " << NmaxZmax;

                  if (options.Zmax > 83 || options.Zmax == 43 || options.Zmax == 0)
                    {
                      std::cout << " with no stable isotope\n";
                    }
                  else
                    {
                      std::cout << " and the heaviest stable isotope has N=" << stblZmax << "\n";
                    }

                  setExtreme("Nmax");
                }
              else
                {
                  std::cout << "\nThat wasn't one of the options. Try again." << std::endl;
                }
            }
          while (required != "a" && required != "b");
        }
      else
        {
          std::cout << "\nThat wasn't one of the options. Try again." << std::endl;
        }
    }
  while (!validChoice);
}
