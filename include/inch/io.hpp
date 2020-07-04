/**
 *
 * \class IO
 *
 * \brief A collection of functions related to user interactions
 *
 *
 */
#ifndef IO_HPP
#define IO_HPP

#include <string_view>

#include <map>
#include <string>
#include <vector>

class Options;

class IO
{
public:
  IO() = default;

  IO(const IO&) = default;
  IO(IO&&)      = default;

  IO& operator=(const IO&) = default;
  IO& operator=(IO&&) = default;

  ~IO() = default;

  /**
   * Read and store the console arguments
   *
   * \param console_options A std::vector of std::string representing all options given on the command line
   *
   * \return[sucess] A std::map with all flags and options
   * \return[failure] A single element std::map
   */
  std::map<std::string, std::string> readConsoleArguments(const std::vector<std::string>& console_options) const;

  /**
   * Take the output from readConsoleArguments and update the Options for the chart to be drawn
   *
   * \param arguments A std::map containing the options to set
   *
   * \return An instance of the Options class representing the current file being drawn
   */
  Options saveConsoleArguments(const std::map<std::string, std::string>& arguments) const;

  /**
   * Show the programs banner
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void showBanner() const;

  /**
   * Print the current version of the project
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void showVersion() const;

  /**
   * Print the banner and how the user all available options
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void showUsage(std::string_view exe) const;

  /**
   * Read an input option file, as an alternative to comamnd line option
   *
   * \param inputFilename A std::string representing the path to the file to read options from
   *
   * \return A std::map with all of the options and values read from the file
   */
  std::map<std::string, std::string> readOptionFile(const std::string& inputFilename) const;

  /// Was the option file parsed correctly and did it contain sensible values
  mutable bool valid_inputfile{ false };
  /// Were the command line options valid to continue running the program
  mutable bool valid_console{ true };
};

#endif // IO_HPP
