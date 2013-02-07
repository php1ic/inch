#include "functions.h"

void drawMagicNumbers(const inputs *draw, std::ofstream &out_file)
{
  std::cout << "\nDrawing magic numbers";

  //0 - Magic number
  //1 - min n(x) value for magic number line
  //2 - max n(x)
  //3 - min z(y)
  //4 - max z(y)
  float EndPoints[7][5]={
    {2,0,21.5,0,11.5},
    {8,1,29.5,0,21.5},
    {20,7.5,51.5,1.5,32.5},
    {28,16.5,55.5,7.5,37.5},
    {50,45.5,91.5,19.5,55.5},
    {82,81.5,139.5,43.5,83.5},
    {126,0,0,76.5,94.5}, // Z<118 so Z=126 magic number is not needed
  };

  float
    max=0.0,
    min=0.0,
    low=-0.25,
    high=1.25;

  out_file << "\n%-----------------\n"
	   << "%- Magic Numbers -\n"
	   << "%-----------------\n"
	   << "black rgb\n"
	   << "1 u div sl" << std::endl;

  for (int i=0; i<7; ++i)
    {
      //-Proton
      if (   draw->Zmax >= EndPoints[i][0]
	  && draw->Zmin <= EndPoints[i][0]
	     )
	{
	  min = EndPoints[i][1]-draw->Nmin;
	  if (min < 0)
	    min = low;

	  max = EndPoints[i][2]-draw->Nmin;
	  if (max > (draw->Nmax-draw->Nmin+high))
	    max = draw->Nmax-draw->Nmin+high;

	  out_file << "%-Z=" << EndPoints[i][0] << " magic number-\n"
		   << min << " " << EndPoints[i][0]-draw->Zmin+1 << " m " << max-min << " 0 rl\n"
		   << min << " " << EndPoints[i][0]-draw->Zmin   << " m " << max-min << " 0 rl st" << std::endl;
	}

      //-Neutron
      if (   draw->Nmax >= EndPoints[i][0]
	  && draw->Nmin <= EndPoints[i][0]
	     )
	{
	  min = EndPoints[i][3]-draw->Zmin;
	  if (min < 0)
	    min = low;

	  max = EndPoints[i][4]-draw->Zmin;
	  if (max > (draw->Zmax-draw->Zmin+high))
	    max = draw->Zmax-draw->Zmin+high;

	  out_file << "%-N=" << EndPoints[i][0] << " magic number-\n"
		   << EndPoints[i][0]-draw->Nmin+1 << " " << min << " m 0 " << max-min << " rl\n"
		   << EndPoints[i][0]-draw->Nmin   << " " << min << " m 0 " << max-min << " rl st" << std::endl;
	}
    }

  std::cout << " - done" << std::endl;
}
