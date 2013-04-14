#include "functions.h"

void setKeyScale(const inputs *draw,
		 float &key_height,
		 float &key_scale,
		 const std::vector<bool> &draw_partition
		 )
{
  for (unsigned int i=0; i<draw_partition.size(); ++i)
    {
      if (draw_partition[draw_partition.size()-(i+1)])
	key_height+=1.5;
    }

  float scale=(draw->Zmax-draw->Zmin)/key_height;

  if (scale>3.0 || draw->section=="a" || draw->Zmax-draw->Zmin==118)
    key_scale=3;
  //HACK - Zmax-Zmin<=8 doesn't display the key properly
  //Set the scale manually in those cases.
  else if (scale<=0.636364)
    key_scale=0.727273;
  else
    key_scale=scale;
}
