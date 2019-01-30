#ifndef IO_HPP
#define IO_HPP

#include "allNeutrons.hpp"
#include "chartColour.hpp"
#include "chartSelection.hpp"
#include "chartType.hpp"
#include "fileType.hpp"

#include <map>
#include <string>
#include <vector>


class Nuclide;
class Options;

class IO
{
public:
  // Constructors
  // default
  IO() = default;
  // copy
  IO(const IO&) = default;
  // move
  IO(IO&&) = default;

  // Assignment
  // copy
  IO& operator=(const IO&) = default;
  // move
  IO& operator=(IO&&) = default;

  // Destructors
  ~IO() = default;

  std::map<std::string, std::string> readConsoleArguments(const std::vector<std::string>& console_options) const;
  void saveConsoleArguments(Options& options, const std::map<std::string, std::string>& arguments) const;

  void showBanner() const;
  void showVersion() const;
  void showUsage(const std::string& exe) const;

  std::map<std::string, std::string> readOptionFile(const std::string& inputFilename) const;

  mutable bool valid_inputfile = false;

  mutable bool valid_console = true;
};

#endif // IO_HPP
