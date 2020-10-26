#ifndef ISOTOPEDISPLAYMODE_HPP
#define ISOTOPEDISPLAYMODE_HPP

// How is an individual isotope going to be displayed
// currently implemented options are (hash characters represent black):
//
//    0
//   ___
//  |   |
//  |   |
//  |___|
//
//    1      2      3      4
//   ___    ___    ___    ___
//  |###|  |   |  |#| |  | |#|
//  |---|  |---|  |#| |  | |#|
//  |___|  |###|  |#|_|  |_|#|
//
//    5      6      7      8
//   ___    ___    ___    ___
//  |\##|  |##/|  |  /|  |\  |
//  | \#|  |#/ |  | /#|  |#\ |
//  |__\|  |/__|  |/##|  |##\|
//
enum class IsotopeDisplayMode
{
  EmptySquare      = 0,
  TopHalf          = 1,
  BottomHalf       = 2,
  LeftHalf         = 3,
  RightHalf        = 4,
  TopRightWedge    = 5,
  TopLeftWedge     = 6,
  BottomRightWedge = 7,
  BottomLeftWedge  = 8
};

#endif // ISOTOPEDISPLAYMODE_HPP
