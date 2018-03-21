#include "functions.hpp"

void setKeyScale(std::unique_ptr<inputs> &draw,
                 const std::unique_ptr<partition> &part
                 )
{
  if ( !draw->key )
    {
      draw->key_scale = 0.0;
      return;
    }

  /// Set the key height by checking how many partition types are used
  std::for_each(std::cbegin(part->draw), std::cend(part->draw),
                [&draw](const bool val)
                {
                  draw->key_height += static_cast<double>(val)*1.5;
                }
                );

  /// We don't want the key to shrink below a certain size.
  draw->key_scale = ( (draw->Zmax - draw->Zmin) > KEY_YOFFSET )
    ? (draw->Zmax - draw->Zmin)/draw->key_height
    : KEY_YOFFSET/draw->key_height;

  /// Nor do we want it to be larger than a certain size.
  if (   draw->key_scale > 3.0
      || draw->section == "a"
      || (draw->Zmax - draw->Zmin) == MAX_Z)
    {
      draw->key_scale = 3.0;
    }
}
