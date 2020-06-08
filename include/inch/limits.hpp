#ifndef LIMITS_HPP
#define LIMITS_HPP

#include <fmt/core.h>

#include <ostream>

class Limits
{
public:
  Limits() = default;

  Limits(const Limits& other)     = default;
  Limits(Limits&& other) noexcept = default;

  Limits& operator=(const Limits& other) = default;
  Limits& operator=(Limits&& other) noexcept = default;

  virtual ~Limits() noexcept = default;

  static constexpr int MIN_Z{ 0 };
  static constexpr int MAX_Z{ 118 };
  static constexpr int MIN_N{ 0 };
  static constexpr int MAX_N{ 177 };

  mutable int Zmin{ MIN_Z };
  mutable int Zmax{ MAX_Z };
  mutable int Nmin{ MIN_N };
  mutable int Nmax{ MAX_N };

  /**
   */
  inline void ResetAllLimits() const
  {
    ResetNLimits();
    ResetZLimits();
  }

  /**
   */
  inline void ResetZLimits() const
  {
    Zmin = MIN_Z;
    Zmax = MAX_Z;
  }

  /**
   */
  inline void ResetNLimits() const
  {
    Nmin = MIN_N;
    Nmax = MAX_N;
  }

  /**
   */
  [[nodiscard]] inline bool inZRange(const int number) const { return (number >= Zmin && number <= Zmax); }

  /**
   */
  [[nodiscard]] inline bool inNRange(const int number) const { return (number >= Nmin && number <= Nmax); }

  /**
   */
  [[nodiscard]] bool setZmin(const int value) const;

  /**
   */
  [[nodiscard]] bool setZmax(const int value) const;

  /**
   */
  [[nodiscard]] bool setNmin(const int value) const;

  /**
   */
  [[nodiscard]] bool setNmax(const int value) const;

  /**
   * Get the range of Z values to be used when creating the chart
   *
   * \param Nothing
   *
   * \return The Z range
   */
  [[nodiscard]] inline int getZRange() const { return (Zmax - Zmin); }

  /**
   * Get the range of N values to be used when creating the chart
   *
   * \param Nothing
   *
   * \return The N range
   */
  [[nodiscard]] inline int getNRange() const { return (Nmax - Nmin); }
};


///// We print this value so need to overload
// inline std::ostream& operator<<(std::ostream& os, const Limits& lim) // NOLINT (fuchsia-overloaded-operator)
//{
//  /// The latin alphabet (lower case) starts at int/unicode value 97,
//  /// add this to the casted underlying type and cast the result
//  /// so we print a char (i.e. what the user is asked for)
//  constexpr int first_abc = 97;
//
//  return os << static_cast<char>(first_abc + static_cast<int>(lim));
//}

#endif // LIMITS_HPP
