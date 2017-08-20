#ifndef NUCLIDE_H
#define NUCLIDE_H

#include <string>
#include <vector>
#include <cmath>
#include <cstdarg>
#include <algorithm>

#include "extractValue.h"

const int NUBASE_START_A=0;
const int NUBASE_END_A=3;
const int NUBASE_START_Z=4;
const int NUBASE_END_Z=7;
const int NUBASE_START_STATE=7;
const int NUBASE_END_STATE=8;
const int NUBASE_START_ME=18;
const int NUBASE_END_ME=29;
const int NUBASE_START_DME=29;
const int NUBASE_END_DME=38;
const int NUBASE_START_ISOMER=39;
const int NUBASE_END_ISOMER=46;
const int NUBASE_START_DISOMER=48;
const int NUBASE_END_DISOMER=56;
const int NUBASE_START_HALFLIFEVALUE=60;
const int NUBASE_END_HALFLIFEVALUE=69;
const int NUBASE_START_HALFLIFEUNIT=69;
const int NUBASE_END_HALFLIFEUNIT=71;
const int NUBASE_START_SPIN=79;
const int NUBASE_END_SPIN=93;
//After the 2003 table, the discovery
//year was added, alterting the positions
const int NUBASE_START_YEAR=106;
const int NUBASE_END_YEAR=110;
//Let the 03 position be the odd-one-out and thus
//have the slightly awkward name
const int NUBASE_START_DECAYSTRING_03=106;
const int NUBASE_START_DECAYSTRING=111;
//The decay string goes to EOL, put here, commented,
//to show that we haven't just forgotten about it.
//const int NUBASE_END_DECAYSTRING=EOL;

const int AME_START_Z=16;
const int AME_END_Z=19;
const int AME_START_A=11;
const int AME_END_A=14;
const int AME_START_ME=29;
const int AME_END_ME=41;
const int AME_START_DME=42;
const int AME_END_DME=53;

const int AME_HEADER_LENGTH=39;
const int AME_EXPERIMENTAL_MARKER=52;

class Nuclide
{
public:
  explicit Nuclide(const std::string &line);
  ~Nuclide();

  bool own;

  int exp;
  int A;
  int Z;
  int N;
  int st;
  int pi;
  int pi_exp;
  int J_exp;
  int J_tent;
  int rich;
  int show;
  int year;

  double NUBASE_ME;
  double NUBASE_dME;
  double AME_ME;
  double AME_dME;
  double s_n;
  double ds_n;
  double s_2n;
  double ds_2n;
  double s_p;
  double ds_p;
  double s_2p;
  double ds_2p;
  double dV_pn;
  double ddV_pn;
  double is_nrg;
  double dis_nrg;
  double hl;
  double J;

  std::string decay;
  std::string colour;
  std::string full_data;

  double errorQuadrature(const size_t x, ...);
  void stripHashes();

  inline void setA() {extractValue(full_data,NUBASE_START_A,NUBASE_END_A,A);}
  inline void setZ() {extractValue(full_data,NUBASE_START_Z,NUBASE_END_Z,Z);}
  inline void setOwn(bool val) {own=val;}
  inline void setN() {N = A-Z;}
  inline void setState() {extractValue(full_data,NUBASE_START_STATE,NUBASE_END_STATE,st);}
  inline void setNubaseMassExcess() {extractValue(full_data,NUBASE_START_ME,NUBASE_END_ME,NUBASE_ME);}
  inline void setNubaseMassExcessError() {extractValue(full_data,NUBASE_START_DME,NUBASE_END_DME,NUBASE_dME);}
  inline void setYear() {extractValue(full_data, NUBASE_START_YEAR, NUBASE_END_YEAR,year);}

  inline void setAMEMassExcess(const std::string &line) {extractValue(line,AME_START_ME,AME_END_ME,AME_ME);}
  inline void setAMEMassExcessError(const std::string &line) {extractValue(line,AME_START_DME,AME_END_DME,AME_dME);}

  void setExperimental();
  inline void setExperimental(int val) {exp=val;}

  void setSpinParity();
  void setSeparationEnergies(std::vector<Nuclide> &nuc);
  void setIsomerEnergy();
  void setHalfLife();
  void setDecayMode(std::vector<bool> &pnSide);
  void setNeutronOrProtonRich(std::vector<bool> &pnSide);
};

#endif
