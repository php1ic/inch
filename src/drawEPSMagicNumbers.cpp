#include "functions.h"

void drawEPSMagicNumbers(const std::unique_ptr<inputs> &draw,
                         std::ofstream &outFile
                         )
{
  std::cout << "Drawing magic numbers";

  //0 - Magic number
  //1 - min n(x) value for magic number line
  //2 - max n(x)
  //3 - min z(y)
  //4 - max z(y)
  std::vector< std::vector<double> > EndPoints =
    {
      {  2.0,  0.0,  21.5,  0.0, 11.5},
      {  8.0,  1.0,  29.5,  0.0, 21.5},
      { 20.0,  7.5,  51.5,  1.5, 32.5},
      { 28.0, 16.5,  55.5,  7.5, 37.5},
      { 50.0, 45.5,  91.5, 19.5, 55.5},
      { 82.0, 81.5, 139.5, 43.5, 83.5},
      {126.0,  0.0,   0.0, 76.5, 94.5}, // Z<=118 so Z=126 magic number is not needed
    };

  double max  =  0.0;
  double min  =  0.0;
  double low  = -0.25;
  double high =  1.25;

  outFile << "\n%-----------------\n"
          << "%- Magic Numbers -\n"
          << "%-----------------\n"
          << "gs\n"
          << "black rgb\n"
          << "1 u div sl" << std::endl;

  for ( size_t i=0; i<EndPoints.size(); ++i )
    {
      //-Proton
      if (   draw->Zmax >= EndPoints[i][0]
          && draw->Zmin <= EndPoints[i][0]
             )
        {
          if ( (min = EndPoints[i][1] - draw->Nmin) < 0.0 )
            {
              min = low;
            }

          if ( (max = EndPoints[i][2] - draw->Nmin) > (draw->Nmax - draw->Nmin + high) )
            {
              max = draw->Nmax - draw->Nmin + high;
            }

          outFile << "%Z=" << EndPoints[i][0] << " magic number\n"
                  << min << " " << EndPoints[i][0] - draw->Zmin+1 << " m " << max - min << " 0 rl\n"
                  << min << " " << EndPoints[i][0] - draw->Zmin   << " m " << max - min << " 0 rl st" << std::endl;
        }

      //-Neutron
      if (   draw->Nmax >= EndPoints[i][0]
          && draw->Nmin <= EndPoints[i][0]
             )
        {
          if ( (min = EndPoints[i][3] - draw->Zmin) < 0.0 )
            {
              min = low;
            }

          if ( (max = EndPoints[i][4] - draw->Zmin) > (draw->Zmax - draw->Zmin + high) )
            {
              max = draw->Zmax - draw->Zmin + high;
            }

          outFile << "%N=" << EndPoints[i][0] << " magic number\n"
                  << EndPoints[i][0] - draw->Nmin+1 << " " << min << " m 0 " << max - min << " rl\n"
                  << EndPoints[i][0] - draw->Nmin   << " " << min << " m 0 " << max - min << " rl st" << std::endl;
        }
    }

  outFile << "gr" << std::endl;

  std::cout << " - done" << std::endl;
}
