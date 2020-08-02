/**
 *
 * \class Grid
 *
 * \brief A square grid to help with positioning
 *
 * When adding new elements to the chart it can be useful to draw a square grid
 * to aid with positioning
 *
 */
#ifndef EPSGRID_HPP
#define EPSGRID_HPP

#include "inch/grid.hpp"

#include <string>


class EPSGrid : public Grid
{
public:
  EPSGrid() = default;

  EPSGrid(const EPSGrid& EPSGrid)     = default;
  EPSGrid(EPSGrid&& EPSGrid) noexcept = default;

  EPSGrid& operator=(const EPSGrid& EPSGrid) = default;
  EPSGrid& operator=(EPSGrid&& EPSGrid) noexcept = default;

  ~EPSGrid() noexcept = default;

  /**
   * Output, as a std::string, the necessary EPS code to create the grid <width> units wide, <height> units high and
   * with spacing of <spacing>.
   *
   * \param width The width of the chart in 'isotope' units i.e. The square representing an isotope is 1 unit
   * \param height The height of the chart in 'iostope' units i.e. The square representing an isotope is 1 unit
   * \param spacing The spacing of the square grid
   */
  [[nodiscard]] std::string DrawGrid(const int width, const int height, const int spacing = 5) const;
};

#endif // EPSGRID_HPP
