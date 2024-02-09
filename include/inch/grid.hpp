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
#ifndef GRID_HPP
#define GRID_HPP

#include <string>

class Grid
{
public:
  Grid() = default;

  Grid(const Grid& Grid)     = default;
  Grid(Grid&& Grid) noexcept = default;

  Grid& operator=(const Grid& Grid)     = default;
  Grid& operator=(Grid&& Grid) noexcept = default;

  virtual ~Grid() noexcept = default;

  /**
   * Output, as a std::string, the necessary EPS code to create the grid <width> units wide, <height> units high and
   * with spacing of <spacing>.
   *
   * \param width The width of the chart in 'isotope' units i.e. The square representing an isotope is 1 unit
   * \param height The height of the chart in 'iostope' units i.e. The square representing an isotope is 1 unit
   * \param spacing The spacing of the square grid
   */
  [[nodiscard]] virtual std::string DrawGrid(const int width, const int height, const int spacing) const = 0;
};

#endif // GRID_HPP
