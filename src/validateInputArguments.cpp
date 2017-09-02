#include "functions.h"

bool validateInputArguments(const std::vector<Nuclide> &nuc,
                            inputs *draw,
                            const std::vector<std::string> &arguments
                            )
{
  //Read option via << -flag file >> so, including the executable, we need
  //an odd number of arguments
  if ( arguments.size()%2 == 0 )
    {
      std::cout << "\n"
                << "***ERROR***: An odd number of arguments is not allowed\n"
                << std::endl;

      draw->constructOutputFilename();

      return false;
    }

  bool validInput=false;
  bool validOutput=false;
  bool validFileType=false;
  bool attemptedToSetFileType=false;

  for ( size_t i=1; i<arguments.size()-1; i=i+2 )
    {
      if ( arguments.at(i) == "-i" )
        {
          validInput = validateInputFile(nuc,draw,arguments.at(i+1));

          if ( !validInput )
            {
              std::cout << "***ERROR***: Bad inputfile - "
                        << arguments.at(i+1) << std::endl;
            }
        }
      else if ( arguments.at(i) == "-o" )
        {
          //Checking of the output file is dependent on if the output
          //type is specified. Look at other arguments to see if it is.
          if ( !attemptedToSetFileType )
            {
              for ( size_t j=i; j<arguments.size()-1; j=j+2 )
                {
                  if ( arguments.at(j) == "-f" )
                    {
                      validFileType = validateFileType(draw,arguments.at(j+1));
                      attemptedToSetFileType=true;

                      if ( !validFileType && attemptedToSetFileType )
                        {
                          std::cout << "**WARNING**: Bad file type - "
                                    << arguments.at(j+1) << std::endl;
                        }
                    }
                }
            }

          validOutput = validateOutputFile(draw,arguments.at(i+1));

          if ( !validOutput )
            {
              std::cout << "***ERROR***: Bad outfile - "
                        << arguments.at(i+1) << std::endl;
            }
        }
      else if ( arguments.at(i) == "-f" && !attemptedToSetFileType )
        {
          std::cout << "Setting file type" << std::endl;
          validFileType = validateFileType(draw,arguments.at(i+1));
          attemptedToSetFileType=true;

          if ( !validFileType )
            {
              std::cout << "**WARNING**: Bad file type - "
                        << arguments.at(i+1) << std::endl;
            }
        }
      else if ( arguments.at(i) == "-y" )
        {
          draw->setTableYear(stoi(arguments.at(i+1)));
        }
      else
        {
          //This feels like a hack. If -o comes before -f we will not fall
          //into the -f condition on the 'top' level so print this warning message.
          //This condition will stop that.
          if ( arguments.at(i) == "-f" && attemptedToSetFileType )
            {
              continue;
            }

          std::cout << "\n**WARNING**: The flag " << arguments.at(i)
                    << " is not currently supported. Ignoring this flag and it's associated options"
                    << std::endl;
        }
    }

  //Validating the output file either exits the code or returns true
  //Thus if validOutput is false, we will not have checked the output file
  if ( !validOutput )
    {
      draw->constructOutputFilename();
    }

  //If the input file was bad we need to ask what to display
  return validInput;
}
