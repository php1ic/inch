#include "include/functions.h"

void printSelection(inputs *draw)
{
  std::cout << "\n===========================\n"
	    << "\nBetween Z = " << draw->Zmin << "(" << convertZToSymbol(draw->Zmin)
	    << ") and Z = " << draw->Zmax << "(" << convertZToSymbol(draw->Zmax) << ")";

  if (draw->section == "a" || (draw->section == "b" && draw->required == "a") )
    std::cout << ", with all relevant nuclei,\n";
  else if (draw->required == "b")
    std::cout << ", N = " << draw->Nmin << " and N = " << draw->Nmax << "\n";

  if      (draw->type == "a") std::cout << "experimentally measured";
  else if (draw->type == "b") std::cout << "theoretical/extrapolated";
  else                        std::cout << "both experimental and theoretical";

  std::cout << " values will be drawn and\nthe chart coloured by ";

  if      (draw->choice == "a") std::cout << "error on mass-excess\n";
  else if (draw->choice == "b") std::cout << "relative error on mass-excess\n";
  else if (draw->choice == "c") std::cout << "major ground-state decay mode\n";
  else if (draw->choice == "d") std::cout << "ground-state half-life\n";
  else                          std::cout << "first isomer energy\n";
}
