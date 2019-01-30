#ifndef CHART_HPP
#define CHART_HPP

#include <iostream>
#include <memory>
#include <vector>


class Nuclide;
class Partition;
class Options;


class Chart
{
public:
  /// Default constructor
  Chart() = default;

  /// Copy constructor
  Chart(const Chart& Chart) = default;

  /// Move constructor
  Chart(Chart&& Chart) noexcept = default;

  /// Destructor
  ~Chart() noexcept = default;

  /// Copy assignment operator
  Chart& operator=(const Chart& Chart) = default;

  /// Move assignment operator
  Chart& operator=(Chart&& Chart) noexcept = default;

  void write(std::vector<Nuclide>& nuc, Options& draw, Partition& part) const;

  void writeEPS(std::vector<Nuclide>& nuc, Options& draw, Partition& part) const;

  void writeSVG(std::vector<Nuclide>& nuc, Options& draw) const;

  void writeTIKZ(std::vector<Nuclide>& nuc, Options& draw) const;

  void drawNuclei(const std::vector<Nuclide>& in, const Options& draw, std::ostream& outFile) const;

  void setCanvasSize(const double key_scale, const double key_height, const Options& draw) const;

  //-----------------------------------------------------------------------
  //- If the file is to be converted into some other format (eg.jpg,png), -
  //- without any resizing, and the whole chart is drawn. The following  --
  //- values are the largest allowed for the given page size. -------------
  //-----------------------------------------------------------------------
  // A0 -> size = 18
  // A1 -> size = 13
  // A2 -> size =  9
  // A3 -> size =  6
  // A4 -> size =  4
  // A5 -> size =  3
  //-----------------
  mutable int size = 4;

  mutable bool key_relative = false;

  mutable double height = 0.0;
  mutable double width  = 0.0;
};

#endif // CHART_HPP
