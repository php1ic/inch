#ifndef KEY_HPP
#define KEY_HPP

#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>


class Partition;
class inputs;


class Key
{
public:
  // Constructor
  // default
  Key() = default;

  // copy
  Key(const Key& Key) = default;
  // move
  Key(Key&& Key) = default;

  // Assignment
  // copy
  Key& operator=(const Key& Key) = default;
  // move
  Key& operator=(Key&& Key) noexcept = default;

  // Destructor
  ~Key() noexcept = default;

  static constexpr int KEY_YOFFSET = 9;

  mutable double height = 0.5;
  mutable double scale  = 0.0;

  mutable std::vector<std::string> textStrings;

  /// Aesthetic values for the key position when the full chart id drawn.
  /// In all other cases the key is to the right of the chart
  mutable std::vector<std::pair<int, int>> fullChartKeyPosition = {
    { 15, 75 }, { 12, 81 }, { 9, 60 }, { 9, 75 }, { 15, 75 }
  };

  void setScale(const std::unique_ptr<inputs>& draw, const std::unique_ptr<Partition>& part) const;

  void EPSSetText(const std::unique_ptr<inputs>& draw, const std::unique_ptr<Partition>& part) const;
  void EPSSetup(std::ofstream& outFile) const;
  void EPSPlaceKey(std::ofstream& outFile, const std::unique_ptr<inputs>& draw) const;
  void EPSAdditionalFunctions(std::ofstream& outFile, const std::unique_ptr<inputs>& draw) const;
  void EPSWrite(std::ofstream& outFile, const std::unique_ptr<Partition>& part) const;
  void EPSSurroundingBox(std::ofstream& outFile) const;
};

#endif // KEY_HPP
