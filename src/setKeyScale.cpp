#include "functions.h"

void setKeyScale(const inputs *draw,
		 float &key_height,
		 float &key_scale,
		 partition *part
		 )
{
  for (unsigned int i=0; i<part->draw.size(); ++i)
    {
      if (part->draw[part->draw.size()-(i+1)])
	key_height+=1.5;
    }

  float scale=(draw->Zmax-draw->Zmin)/key_height;

  if (scale>3.0 || draw->section=="a" || draw->Zmax-draw->Zmin==118)
    key_scale=3;
  else
    key_scale=scale;
}
