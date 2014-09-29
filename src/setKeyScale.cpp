#include "include/functions.h"

void setKeyScale(inputs *draw,
		 const partition *part
		 )
{
  if (!draw->key)
    {
      draw->key_scale=0.0;
      return;
    }

  for (size_t i=0; i<part->draw.size(); ++i)
    if (part->draw[part->draw.size()-(i+1)])
      draw->key_height+=1.5;


  //We don't want the key to shrink below a certain size.
  if (draw->Zmax-draw->Zmin < 9)
    draw->key_scale=9/draw->key_height;
  else
    draw->key_scale=(draw->Zmax-draw->Zmin)/draw->key_height;

  //Nor do we want it to be larger than a certain size.
  if (   draw->key_scale > 3.0
      || draw->section == "a"
      || draw->Zmax-draw->Zmin == MAX_Z)
    {
      draw->key_scale=3.0;
    }
}
