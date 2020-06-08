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
   * Reset both neutron and proton limits to their original, i.e. maxima, values
   *
   * \param Notthing
   *
   * \return Nothing
   */
  inline void ResetAllLimits() const
  {
    ResetNLimits();
    ResetZLimits();
  }

  /**
   * Reset both proton limits to their original, i.e. maxima, values
   *
   * \param Notthing
   *
   * \return Nothing
   */
  inline void ResetZLimits() const
  {
    Zmin = MIN_Z;
    Zmax = MAX_Z;
  }

  /**
   * Reset both neutron limits to their original, i.e. maxima, values
   *
   * \param Notthing
   *
   * \return Nothing
   */
  inline void ResetNLimits() const
  {
    Nmin = MIN_N;
    Nmax = MAX_N;
  }

  /**
   * Is the provides <number> in the currently set N range
   *
   * \param The number to check
   *
   * \return [true] The number is in the current range
   * \return [false] The number is not in the current range
   */
  [[nodiscard]] inline bool inZRange(const int number) const { return (number >= Zmin && number <= Zmax); }

  /**
   * Is the provides <number> in the currently set N range
   *
   * \param The number to check
   *
   * \return [true] The number is in the current range
   * \return [false] The number is not in the current range
   */
  [[nodiscard]] inline bool inNRange(const int number) const { return (number >= Nmin && number <= Nmax); }

  /**
   * Set the Zmax value, and use limits checks to validate.
   * Upon failure, the limit is left unchanged
   *
   * \param The value to set the limit todo
   *
   * \return [true] Limit has been successfully set
   * \return [false] Limits has not be set
   */
  [[nodiscard]] bool setZmin(const int value) const;

  /**
   * Set the Zmax value, and use limits checks to validate.
   * Upon failure, the limit is left unchanged
   *
   * \param The value to set the limit todo
   *
   * \return [true] Limit has been successfully set
   * \return [false] Limits has not be set
   */
  [[nodiscard]] bool setZmax(const int value) const;

  /**
   * Set the Nmin value, and use limits checks to validate.
   * Upon failure, the limit is left unchanged
   *
   * \param The value to set the limit todo
   *
   * \return [true] Limit has been successfully set
   * \return [false] Limits has not be set
   */
  [[nodiscard]] bool setNmin(const int value) const;

  /**
   * Set the Nmax value, and use limits checks to validate.
   * Upon failure, the limit is left unchanged
   *
   * \param The value to set the limit todo
   *
   * \return [true] Limit has been successfully set
   * \return [false] Limits has not be set
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

#endif // LIMITS_HPP
