#include "functions.h"

void setKeyScale(inputs *draw,
		 const partition *part
		 )
{
  for (unsigned int i=0; i<part->draw.size(); ++i)
    {
      if (part->draw[part->draw.size()-(i+1)])
	draw->key_height+=1.5;
    }

  float scale=0;

  //(Bit of a)HACK - We don't want the key to shrink below a certain size.
  if (draw->Zmax-draw->Zmin < 9)
    scale=9/draw->key_height;
  else
    scale=(draw->Zmax-draw->Zmin)/draw->key_height;

  if (scale>3.0 || draw->section=="a" || draw->Zmax-draw->Zmin==118)
    draw->key_scale=3;
  else
    draw->key_scale=scale;
}
