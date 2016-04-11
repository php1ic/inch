#include "functions.h"

void drawEPSGrid(const inputs *draw,
                 std::ofstream &outFile
                 )
{
  outFile << "\n%-Grid----------\n"
          << "gs\n"
          << "0.75 setgray\n"
          << "2 TR\n"
          << "0.1 u div sl\n"
          << "\n5 5 " << draw->chart_width << "{\n"
          << "/i ed\n"
          << "i 1 m\n"
          << "gs\n"
          << "i 10 mod 0 eq {\n"
          << "i 3 string cvs sh\n"
          << "i " << draw->chart_height << " m i 3 string cvs sh\n"
          << "} if\n"
          << "gr\n"
          << "0 " << draw->chart_height-2 << " rl\n"
          << "} for\n"
          << "\n"
          << "5 5 " << draw->chart_height << "{\n"
          << "/i ed\n"
          << "1 i m\n"
          << "gs\n"
          << "i 10 mod 0 eq {\n"
          << "i 3 string cvs sh\n"
          << draw->chart_width << " i m i 3 string cvs sh\n"
          << "} if\n"
          << "gr\n"
          << draw->chart_width-2<< " 0 rl\n"
          << "} for\n"
          << "\nst\n"
          << "gr\n"
          << "%---------------\n" << std::endl;
}
