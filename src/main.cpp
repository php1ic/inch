///////////////////////////////////////////////////////////////
//                                                           //
// inch                                                      //
//                                                           //
///////////////////////////////////////////////////////////////
//                                                           //
// Description: This program will create a nuclear chart     //
//              containing the nuclei specified by the user, //
//              coloured by a user chosen property           //
//                                                           //
// Input:       Required - None                              //
//                                                           //
//              Optional - The name of the output file       //
//                       - An input file containing values   //
//                         used to create the chart          //
//                                                           //
//                                                           //
// Output:      An encapsulated postscript (eps) file        //
//              or a scalable vector graphics (svg) file     //
//              or a latex figure file (tikz) file           //
//                                                           //
///////////////////////////////////////////////////////////////

#include "include/createChart.h"

int main(int argc, char *argv[])
{
  inputs *draw = new inputs;

  if (argc == 2
      && (argv[1] == std::string("--version") || argv[1] == std::string("-v"))
      )
    {
      draw->showVersion();
      delete draw;
      return 0;
    }

  partition *part = new partition;
  std::vector<Nuclide> nuc;

  draw->showBanner();
  draw->constructFullyQualifiedPaths();

  if (argc > 1)
    {
      std::vector<std::string> arguments(argv,argv+argc);
      createChart *chart = new createChart(draw,part,nuc,arguments);
      delete chart;
    }
  else
    {
      createChart *chart = new createChart(draw,part,nuc);
      delete chart;
    }

  draw->writeOptionFile();

  std::cout << "Enjoy\n"
	    << "\nTo run again with the same options: " << argv[0]
	    << " -i options.in\n" << std::endl;

  delete draw;
  delete part;

  std::cout << std::endl;

  return 0;
}
