#ifndef PROLOG_HPP
#define PROLOG_HPP

#include "inputs.hpp"

#include <cmath>
#include <ctime>
#include <iomanip>
#include <string>

class Prolog
{
public:
  //Constructors
  //default
  Prolog()
  {
    setTime(std::time(nullptr));
  }

  //copy
  Prolog(const Prolog&) = default;
  //move
  Prolog(Prolog&&) = default;

  //Assignment
  //move
  Prolog& operator=(Prolog&&) = default;
  //copy
  Prolog& operator=(const Prolog&) = default;

  //Destructor
  ~Prolog() = default;

  inline auto getTime() const {return std::put_time(now, "%Y-%m-%dT%H:%M:%S");}

  void EPSWriteProlog(std::ofstream &outFile, std::unique_ptr<inputs> &draw) const;
  void TIKZWriteProlog(std::ofstream &outFile, std::unique_ptr<inputs> &draw) const;
  void SVGWriteProlog(std::ofstream &outFile, std::unique_ptr<inputs> &draw) const;

private:
  mutable std::tm* now = nullptr;
  mutable double curve = 0.25;

  inline void setTime(const std::time_t theTime) const {now = std::localtime(&theTime);}
};

#endif // PROLOG_HPP
