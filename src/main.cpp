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

#include "include/functions.h"

int main(int argc, char *argv[])
{
  static inputs *draw = new inputs;
  static partition *part = new partition;
  static std::vector<Nuclide> nuc;

  static bool inputfile(false);
  static std::vector<std::string> arguments(argv,argv+argc);

  createChart(draw,part,nuc,arguments,inputfile);

  std::cout << "Enjoy\n"
	    << "\nTo run again with the same options: " << arguments.at(0) << " -i options.in\n" << std::endl;

  delete draw;
  delete part;

  std::cout << std::endl;

  return 0;
}
