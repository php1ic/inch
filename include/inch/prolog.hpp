/**
 *
 * \class Prolog
 *
 * \brief Set up the definitions and functions that will be used to create the chart
 *
 * To make smaller files, and create easier to read 'code', we define various functions, methods
 * and constants for all type. Use this class to do that.
 */
#ifndef PROLOG_HPP
#define PROLOG_HPP

#include <fstream>
#include <iomanip>
#include <memory>


class Options;
struct tm;

class Prolog
{
public:
  explicit Prolog(const int size) : chart_size(size) { setTime(std::time(nullptr)); }

  Prolog(const Prolog&) = default;
  Prolog(Prolog&&)      = default;

  Prolog& operator=(Prolog&&) = default;
  Prolog& operator=(const Prolog&) = default;

  ~Prolog() = default;

  /// Get the date and time that chart is created (i.e. runtime)
  inline auto getTime() const { return std::put_time(now, "%Y-%m-%dT%H:%M:%S"); }

  /**
   * Write the functions needed to create the EPS file
   *
   * \param The output file stream representing the chart
   * \param An instance of the Option class containing the chart options
   *
   * \return Nothing
   */
  void EPSWriteProlog(std::ofstream& outFile, const Options& draw) const;

  /**
   * Write the functions needed to create the TikZ file
   *
   * \param The output file stream representing the chart
   * \param An instance of the Option class containing the chart options
   *
   * \return Nothing
   */
  void TIKZWriteProlog(std::ofstream& outFile /*, std::unique_ptr<Options> &draw*/) const;

  /**
   * Write the functions needed to create the SVG file
   *
   * \param The output file stream representing the chart
   * \param An instance of the Option class containing the chart options
   *
   * \return Nothing
   */
  void SVGWriteProlog(std::ofstream& outFile, const Options& draw) const;

private:
  /// The date and time at runtime
  mutable std::tm* now{ nullptr };
  /// The size of the chart in 'iostope units'
  mutable int chart_size{ 0 };
  /// How rounded are the 'squares' representing an isotope, 0=square, 1=circle
  mutable double curve{ 0.25 };

  /// Write the current date and time into the member variable
  inline void setTime(const std::time_t theTime) const { now = std::localtime(&theTime); }
};

#endif // PROLOG_HPP
