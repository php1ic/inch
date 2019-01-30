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
  // Constructors
  // default
  explicit Prolog(const int size) : chart_size(size) { setTime(std::time(nullptr)); }

  // copy
  Prolog(const Prolog&) = default;
  // move
  Prolog(Prolog&&) = default;

  // Assignment
  // move
  Prolog& operator=(Prolog&&) = default;
  // copy
  Prolog& operator=(const Prolog&) = default;

  // Destructor
  ~Prolog() = default;

  inline auto getTime() const { return std::put_time(now, "%Y-%m-%dT%H:%M:%S"); }

  void EPSWriteProlog(std::ofstream& outFile, Options& draw) const;
  void TIKZWriteProlog(std::ofstream& outFile /*, std::unique_ptr<Options> &draw*/) const;
  void SVGWriteProlog(std::ofstream& outFile, Options& draw) const;

private:
  mutable std::tm* now   = nullptr;
  mutable int chart_size = 0;
  mutable double curve   = 0.25;

  inline void setTime(const std::time_t theTime) const { now = std::localtime(&theTime); }
};

#endif // PROLOG_HPP
