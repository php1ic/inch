#ifndef CHART_HPP
#define CHART_HPP

#include "dripline.hpp"
#include "grid.hpp"
#include "inputs.hpp"
#include "key.hpp"
#include "magicNumbers.hpp"
#include "nuclide.hpp"
#include "partition.hpp"
#include "prolog.hpp"
#include "rProcess.hpp"

#include <iostream>

class Chart
{
public:
  //! Default constructor
  Chart() = default;

  //! Copy constructor
  Chart(const Chart &Chart) = default;

  //! Move constructor
  Chart(Chart &&Chart) noexcept = default;

  //! Destructor
  ~Chart() noexcept = default;

  //! Copy assignment operator
  Chart& operator=(const Chart &Chart) = default;

  //! Move assignment operator
  Chart& operator=(Chart &&Chart) noexcept = default;

  void write(std::vector<Nuclide> &nuc,
             std::unique_ptr<inputs> &draw,
             std::unique_ptr<Partition> &part) const;

  void writeEPS(std::vector<Nuclide> &nuc,
                std::unique_ptr<inputs> &draw,
                std::unique_ptr<Partition> &part) const;

  void writeSVG(std::vector<Nuclide> &nuc,
                std::unique_ptr<inputs> &draw) const;

  void writeTIKZ(std::vector<Nuclide> &nuc,
                 std::unique_ptr<inputs> &draw) const;

  void drawNuclei(const std::vector<Nuclide> &in, const std::unique_ptr<inputs> &draw, std::ostream &outFile) const;

protected:
private:
};

#endif // CHART_HPP
