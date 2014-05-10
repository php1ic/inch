#include "include/functions.h"

void setCanvasSize(inputs *draw)
{
  if (draw->key_height*draw->key_scale > (draw->Zmax-draw->Zmin+2))
    {
      draw->key_relative=true;
      draw->chart_height=(draw->key_height+1.0)*draw->key_scale;
    }
  else
    draw->chart_height=(draw->Zmax-draw->Zmin+2);

  //HACK - When all nuclei are drawn, key is in top left.
  //Below stops extra space being created on the right.
  if (draw->section == "a" || (draw->Zmax-draw->Zmin) == MAX_Z)
    draw->chart_width=draw->Nmax-draw->Nmin+2;
  else
    draw->chart_width=draw->Nmax-draw->Nmin+2+14.5*draw->key_scale;
}
