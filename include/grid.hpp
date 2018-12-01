#ifndef GRID_HPP
#define GRID_HPP

#include "inputs.hpp"

#include <fstream>
#include <memory>

class Grid
{
public:
  // Constructors
  // default
  Grid() = default;
  // copy
  Grid(const Grid& Grid) = default;
  // move
  Grid(Grid&& Grid) noexcept = default;

  // Assignment
  // copy
  Grid& operator=(const Grid& Grid) = default;
  // move
  Grid& operator=(Grid&& Grid) noexcept = default;

  // Destructor
  ~Grid() noexcept = default;

  void EPSDrawGrid(std::ofstream& outFile, const double width, const double height, const int spacing = 5) const;
};

#endif // GRID_HPP
