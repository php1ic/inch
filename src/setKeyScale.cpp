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

  if ((draw->Zmax-draw->Zmin)/key_height>3.0 || draw->section=="a" || draw->Zmax-draw->Zmin==118)
    key_scale=3;
  else
    key_scale=(draw->Zmax-draw->Zmin)/key_height;
}
