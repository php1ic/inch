#include "functions.h"

void displaySection(std::vector<Nuclide> &in,
                    inputs *draw
                    )
{
  int stblZmin=MAX_N;
  int stblZmax=MIN_N;
  int NminZmin=MAX_N;
  int NminZmax=MAX_N;
  int NmaxZmin=MIN_N;
  int NmaxZmax=MIN_N;

  std::cout << "\n---------------------------\n"
            << "Draw a) The entire chart\n"
            << "     b) A section\n";
  do
    {
      std::cout << "[a,b]: ";
      std::cin  >> draw->section;

      if ( draw->section == "a" )
        {
          draw->Zmin = MIN_Z;
          draw->Nmin = MIN_N;
          draw->Zmax = MAX_Z;
          draw->Nmax = MAX_N;
        }
      else if ( draw->section == "b" )
        {
          std::cout << "---------------------------\n"
                    << "Enter range of Z, by symbol [n,Ei] or number [0," << MAX_Z << "]\n";

          draw->setExtreme("Zmin");

          draw->setExtreme("Zmax");

          std::cout << "---------------------------\n"
                    << "Draw a) All required N\n"
                    << "     b) A section\n";
          do
            {
              std::cout << "[a,b]: ";
              std::cin  >> draw->required;

              if ( draw->required == "a" )
                {
                  setNeutronLimits(in,draw);
                }
              else if ( draw->required == "b" )
                {
                  for ( auto it=in.begin(); it!=in.end(); ++it )
                    {
                      //Set N range for Zmin
                      if ( it->Z == draw->Zmin )
                        {
                          if ( it->N < NminZmin )
                            {
                              NminZmin = it->N;
                            }
                          else if ( it->N > NmaxZmin )
                            {
                              NmaxZmin = it->N;
                            }
                        }
                      //Set N range for Zmax
                      else if ( it->Z == draw->Zmax )
                        {
                          if ( it->N < NminZmax )
                            {
                              NminZmax = it->N;
                            }
                          else if ( it->N > NmaxZmax )
                            {
                              NmaxZmax = it->N;
                            }
                        }
                    }

                  //Set high/low stable N for Zmax/Zmin
                  for ( auto it=in.begin(); it!=in.end(); ++it )
                    {
                      if (    it->N >= NminZmin
                           && it->N <= NmaxZmax
                           && it->decay == "stable"
                          )
                        {
                          if ( it->Z == draw->Zmin && it->N < stblZmin )
                            {
                              stblZmin = it->N;
                            }

                          if ( it->Z == draw->Zmax && it->N > stblZmax )
                            {
                              stblZmax = it->N;
                            }
                        }
                    }

                  std::cout << "---------------------------\n"
                            << "Enter range of N [0," << MAX_N << "]\n"
                            << draw->convertZToSymbol(draw->Zmin) << "(" << draw->Zmin << ") has N from "
                            << NminZmin << " to " << NmaxZmin;

                  if ( draw->Zmin > 83 || draw->Zmin == 43 || draw->Zmin == 0 )
                    {
                      std::cout << " with no stable isotope\n";
                    }
                  else
                    {
                      std::cout << " and the lightest stable isotope has N=" << stblZmin << "\n";
                    }

                  draw->setExtreme("Nmin");

                  std::cout << draw->convertZToSymbol(draw->Zmax) << "(" << draw->Zmax << ") has N from "
                            << NminZmax << " to " << NmaxZmax;

                  if ( draw->Zmax > 83 || draw->Zmax == 43 || draw->Zmax == 0 )
                    {
                      std::cout << " with no stable isotope\n";
                    }
                  else
                    {
                      std::cout << " and the heaviest stable isotope has N=" << stblZmax << "\n";
                    }

                  draw->setExtreme("Nmax");
                }
              else
                {
                  std::cout << "\nThat wasn't one of the options. Try again." << std::endl;
                }
            }
          while ( draw->required != "a" && draw->required != "b" );
        }
      else
        {
          std::cout << "\nThat wasn't one of the options. Try again." << std::endl;
        }
    }
  while ( draw->section != "a" && draw->section != "b" );

  std::cout << "---------------------------\n"
            << "Display which nuclei?\n"
            << "a) Experimentally measured only\n"
            << "b) Theoretical/Extrapolated values only\n"
            << "c) Both\n";
  do
    {
      std::cout << "Which: ";
      std::cin  >> draw->type;

      if ( draw->type != "a" && draw->type != "b" && draw->type != "c" )
        {
          std::cout << "\nThat wasn't one of the options. Try again" << std::endl;
        }
    }
  while ( draw->type != "a" && draw->type != "b" && draw->type != "c" );

  if      ( draw->type == "a" ) draw->experimental = 0;
  else if ( draw->type == "b" ) draw->experimental = 1;
  else                          draw->experimental = 2;

  std::cout << "---------------------------\n"
            << "Colour by which property?\n"
            << "a) Error on Mass-Excess\n"
            << "b) Relative Error on Mass-Excess (dm/m)\n";

  if ( !draw->AME )
    {
      std::cout << "c) Major Ground-State Decay Mode\n"
                << "d) Ground-State Half-Life\n";

      if ( draw->type!="b" )
        {
          std::cout << "e) First Isomer Energy\n";
        }
    }

  bool validChoice=false;
  while ( !validChoice )
    {
      std::cout << "Choice: ";
      std::cin  >> draw->choice;

      if ( draw->AME )
        {
          if ( draw->choice != "a" && draw->choice != "b" )
            {
              std::cout << "\nThat wasn't one of the options. Try again" << std::endl;
            }
          else if ( draw->choice == "a" || draw->choice == "b" )
            {
              validChoice = true;
            }
        }
      else
        {
          if (  (draw->type == "a" || draw->type== "c")
              && draw->choice != "a"
              && draw->choice != "b"
              && draw->choice != "c"
              && draw->choice != "d"
              && draw->choice != "e"
              )
            {
              std::cout << "\nThat wasn't one of the options. Try again" << std::endl;
            }
          else if (   draw->type == "b"
                   && draw->choice != "a"
                   && draw->choice != "b"
                   && draw->choice != "c"
                   && draw->choice != "d"
                   )
            {
              std::cout << "\nThat wasn't one of the options. Try again" << std::endl;
            }
          else
            {
              validChoice = true;
            }
        }
    }

  /*
  std::cout << "\n^^^^^^^^^^^^^^^^^^^^^^^^^^^\n"
            << "Z = " << draw->Zmin << " -> " << draw->Zmax << "\n"
            << "N = " << draw->Nmin << " -> " << draw->Nmax << "\n"
            << "===========================\n"
            << draw->Zmin << " : " << NminZmin << " " << stblZmin << " " << NmaxZmin << "\n"
            << draw->Zmax << " : " << NminZmax << " " << stblZmax << " " << NmaxZmax << "\n"
            << "===========================\n"
            << draw->Zmin << " : " << draw->Zmin+draw->Nmin << "  " << draw->Zmin+NmaxZmin << "\n"
            << draw->Zmax << " : " << draw->Zmax+draw->Nmin << "  " << draw->Zmax+NmaxZmax << "\n"
            << "___________________________\n" << std::endl;
  */
}
