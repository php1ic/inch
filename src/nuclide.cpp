#include "nuclide.h"

#include <iostream>

Nuclide::Nuclide(const std::string &line):
  full_data(line)
{
}

Nuclide::~Nuclide()
{
}


/*
  inline void setA() {extractValue(full_data,NUBASE_START_A,NUBASE_END_A,A);}
  inline void setZ() {extractValue(full_data,NUBASE_START_Z,NUBASE_END_Z,Z);}
  inline void setOwn(bool val) {own=val;}
  inline void setN() {N = A-Z;}
  inline void setState() {extractValue(full_data,NUBASE_START_STATE,NUBASE_END_STATE,st);}
  inline void setNubaseMassExcess() {extractValue(full_data,NUBASE_START_ME,NUBASE_END_ME,NUBASE_ME);}
  inline void setNubaseMassExcessError() {extractValue(full_data,NUBASE_START_DME,NUBASE_END_DME,NUBASE_dME);}
  inline void setAMEMassExcess(const std::string &line) {extractValue(line,AME_START_ME,AME_END_ME,AME_ME);}
  inline void setAMEMassExcessError(const std::string &line) {extractValue(line,AME_START_DME,AME_END_DME,AME_dME);}
  inline void setYear() {extractValue(full_data, NUBASE_START_YEAR, NUBASE_END_YEAR,year);}
  inline void setExperimental(int val) {exp=val;}
 */


double Nuclide::errorQuadrature(const size_t x, ...)
{
  va_list individualErrors;
  va_start(individualErrors,x);

  double fullError=0.0;

  for ( size_t i=0; i<x; ++i )
    {
      double value=va_arg(individualErrors,double);
      fullError+=value*value;
    }

  va_end(individualErrors);

  return sqrt(fullError);
}


void Nuclide::stripHashes()
{
  // Replace # (signifying theoretical/extrapolated values)
  // with empty space to maintain the line length
  if ( full_data.find_first_of("#") != std::string::npos )
    {
      replace(full_data.begin(),full_data.end(),'#',' ');
    }
}


void Nuclide::setSpinParity()
{
  // This is dirty.

  // Do this prior to replacing all '#' with ' '
  // As a general rule, the first value of spin and/or parity will be taken

  // The information starts at character NUBASE_START_SPIN(79), so if the line is not at least that
  // size set values to 'unknown' and get out.
  // OR
  // The isotope can have no spin/parity, but a decay method, in which case we
  // pass the above condition but there is no need to do any processing. Set
  // the values to unknown and get out.
  if ( full_data.size() <= NUBASE_START_SPIN
       || full_data.at(NUBASE_START_SPIN) == ' '
       )
    {
      J = 100.0;
      pi = pi_exp = J_exp = J_tent = 2;

      return;
    }

  // Easiest to extract values by stripping away bits after use
  std::string jpi = full_data.substr(NUBASE_START_SPIN, (NUBASE_END_SPIN-NUBASE_START_SPIN) );

  // Some values are set as words (high, low, mix, spmix, fsmix, am)
  // Don't want this so set to 'unknown' and get out.
  if ( isalpha(jpi[0]) )
    {
      J = 100.0;
      pi = pi_exp = J_exp = J_tent = 2;

      return;
    }

  // trim trailing spaces
  size_t endpos = jpi.find_last_not_of(" ");
  if( std::string::npos != endpos )
    {
      jpi = jpi.substr( 0, endpos+1 );
    }

  // HACKS for those nuclei with non-unique assignments.
  //
  // 42Sc isomer 5 has (1+ to 4+) change to (1+)
  if (jpi.find("(1+ to 4+)") != std::string::npos) jpi.replace(3,jpi.length(),")");
  // 71Se isomer 1 has 1/2- to 9/2- change to 9/2-
  if (jpi.find("1/2- to 9/2-") != std::string::npos) jpi.replace(4,jpi.length()," ");
  // 142Ho has (6 to 9) change to (6)
  if (jpi.find("(6 to 9)") != std::string::npos) jpi.replace(2,jpi.length(),")");
  // 131I isomer 2 has (19/2+..23/2+) change to (19/2+)
  if (jpi.find("(19/2+..23/2+)") != std::string::npos) jpi.replace(6,jpi.length(),")");
  // 118Rh has gs J=4-10, take as 4
  if (jpi.find("(4-10)") != std::string::npos) jpi.erase(jpi.find("(4-10)")+1,3);
  // Remove 'unhelpful' characters
  if (jpi.find(">") != std::string::npos) jpi.erase(jpi.find(">"),1);
  if (jpi.find("<") != std::string::npos) jpi.erase(jpi.find("<"),1);
  // 176Tam has no J value, just (+). Easiest to say both are unknown
  if (jpi.substr(0,3) == "(+)") jpi = "?";

  // If no parity is in the copied section then there is no assignment
  // Set to 'unknown' and get out.
  if ( jpi.find("+") == std::string::npos && jpi.find("-") == std::string::npos )
    {
      J = 100.0;
      pi = pi_exp = J_exp = J_tent = 2;

      return;
    }

  // Take only the first parity if two are specified for the same state
  if ( jpi.find("+") != std::string::npos && jpi.find("-") != std::string::npos )
    {
      if ( jpi.find_first_of("+") > jpi.find_first_of("-") )
        {
          jpi.erase(jpi.find("+"),1);
        }
      else
        {
          jpi.erase(jpi.find("-"),1);
        }
    }

  bool experimental=false;
  bool theoretical=false;

  // Member pi is the parity (0)+ve or (1)-ve
  // Member pi_exp is the state experimental(0) or theory/extrapolated(1)
  // We will remove the sign once we record it.
  if ( jpi.find("+") != std::string::npos )
    {
      pi = 0;
      theoretical = experimental = false;
      do
        {
          if ( jpi.size() > (jpi.find("+")+1) && jpi.at(jpi.find("+")+1) == '#' )
            {
              jpi.erase(jpi.find("+"),2);
              theoretical=true;
            }
          else
            {
              jpi.erase(jpi.find("+"),1);
              experimental=true;
            }
        }
      while ( jpi.find("+") != std::string::npos );

      pi_exp = (theoretical > experimental) ? 1 : 0;
    }
  else if ( jpi.find("-") != std::string::npos )
    {
      pi = 1;
      theoretical = experimental = false;
      do
        {
          if ( jpi.size() > (jpi.find("-")+1) && jpi.at(jpi.find("-")+1) == '#' )
            {
              jpi.erase(jpi.find("-"),2);
              theoretical=true;
            }
          else
            {
              jpi.erase(jpi.find("-"),1);
              experimental=true;
            }
        }
      while ( jpi.find("-") != std::string::npos );

      pi_exp = (theoretical > experimental) ? 1 : 0;
    }

  // Stripping away the +/- will leave some () so remove them
  if ( jpi.find("()") != std::string::npos )
    {
      jpi.erase(jpi.find("()"),2);
    }

  // Member J_tent shows either definite(0) or tentative(1) assignment
  if ( jpi.find("(") != std::string::npos )
    {
      jpi.erase(jpi.find("("),1);
      jpi.erase(jpi.find(")"),1);
      J_tent = 1;
    }
  else
    {
      J_tent = 0;
    }

  // If multiple spins are given, take only the first
  if ( jpi.find(",") != std::string::npos )
    {
      jpi.erase(jpi.find(","));
    }

  // Member J_exp either experiment(0) or theory/extrapolated(1) assigment
  if ( jpi.find("#") != std::string::npos )
    {
      jpi.erase(jpi.find("#"),1);
      J_exp = 1;
    }
  else
    {
      J_exp = 0;
    }

  // Member J stores the spin as a double
  if ( jpi.find("/") == std::string::npos )
    {
      extractValue(jpi,0,jpi.length(),J);
    }
  else
    {
      extractValue(jpi,0,jpi.find("/"),J);
      J /= 2.0;
    }
}


void Nuclide::setExperimental()
{
  // Member exp has false(experiment) or true(theory/extrapolation) value
  // Will use mass excess for criteria, the last digit is char NUBASE_END_DME (38)
  //so if  there is a '#' but it's after this we will still say experimental
  size_t measured = full_data.find_first_of("#");
  if ( measured == std::string::npos || measured > NUBASE_END_DME )
    {
      exp = 1;
    }
  else
    {
      exp = 0;
    }

  stripHashes();
}


void Nuclide::setSeparationEnergies(std::vector<Nuclide> &nuc)
{
  size_t numDripLinesRead=0;

  Nuclide *previous=NULL;

  // Loop from the penultimate isotope towards the beginning.
  // As the vector is ordered by A (low to high), this will
  // remove a large number of checks as the vector get bigger.
  for ( size_t i=nuc.size()-1; i>0; --i )
    {
      previous = &nuc[i];

      // Only calculate for ground states.
      if ( previous->st == 0 )
        {
          // Single particle energies.
          if ( A - previous->A == 1 )
            {
              // S_p(Z,N) = M(Z-1,N) - M(Z,N) + M(1,0)
              if (   N - previous->N == 0
                  && Z - previous->Z == 1 )
                {
                  s_p  = previous->NUBASE_ME - NUBASE_ME + nuc[1].NUBASE_ME;
                  ds_p = errorQuadrature(3,previous->NUBASE_dME,NUBASE_dME,nuc[1].NUBASE_dME);
                  numDripLinesRead++;
                }
              // S_n(Z,N) = M(Z,N-1) - M(Z,N) + M(0,1)
              else if (   Z - previous->Z == 0
                       && N - previous->N == 1 )
                {
                  s_n  = previous->NUBASE_ME - NUBASE_ME + nuc[0].NUBASE_ME;
                  ds_n = errorQuadrature(3,previous->NUBASE_dME,NUBASE_dME,nuc[0].NUBASE_dME);
                  numDripLinesRead++;
                }
            }
          // Two particle energies.
          else if ( A - previous->A == 2 )
            {
              // S_2p(Z,N) = M(Z-2,N) - M(Z,N) + 2*M(1,0)
              if (   N - previous->N == 0
                  && Z - previous->Z == 2 )
                {
                  s_2p  = previous->NUBASE_ME - NUBASE_ME + 2*nuc[1].NUBASE_ME;
                  ds_2p = errorQuadrature(4,previous->NUBASE_dME,NUBASE_dME,nuc[1].NUBASE_dME,nuc[1].NUBASE_dME);
                  numDripLinesRead++;
                }
              // S_2n(Z,N) = M(Z,N-2) - M(Z,N) + 2*M(0,1)
              else if (   Z - previous->Z == 0
                       && N - previous->N == 2 )
                {
                  s_2n  = previous->NUBASE_ME - NUBASE_ME + 2*nuc[0].NUBASE_ME;
                  ds_2n = errorQuadrature(4,previous->NUBASE_dME,NUBASE_dME,nuc[0].NUBASE_dME,nuc[0].NUBASE_dME);

                  // |dV_pn(Z,N)| = 1/4*[S_2p(Z,N) - S_2p(Z,N-2)]
                  dV_pn  = fabs(0.25*(s_2p - previous->s_2p));
                  ddV_pn = 0.25*errorQuadrature(2,previous->ds_2p,ds_2p);
                  numDripLinesRead++;
                }
            }
          // Once the difference in A is greater than 2 we wont get any more useful data
          // so set the 'exit variable' to the get out value.
          else if ( A - previous->A >= 3 )
            {
              numDripLinesRead=4;
            }
        }

      // Get out if we have recorded/calculated all of the values.
      if ( numDripLinesRead == 4 )
        {
          break;
        }
    }
}


void Nuclide::setIsomerEnergy()
{
  if ( st == 0 )
    return;

  extractValue(full_data,NUBASE_START_ISOMER,NUBASE_END_ISOMER,is_nrg);

  // Some isomers(3 in total) are measured via beta difference so come out -ve
  if ( is_nrg < 0.0 )
    is_nrg *= -1.0;

  extractValue(full_data,NUBASE_START_DISOMER,NUBASE_END_DISOMER,dis_nrg);
}


void Nuclide::setHalfLife()
{
  std::string lifetime = (full_data.size() < (NUBASE_START_HALFLIFEVALUE-1))
    ? "no_units"
    : full_data.substr(NUBASE_START_HALFLIFEVALUE,
                       (NUBASE_END_HALFLIFEVALUE-NUBASE_START_HALFLIFEVALUE)
                       );

  if (   lifetime.find("p-unst") != std::string::npos
      || lifetime.find_first_not_of(" ") == std::string::npos
      || lifetime.find("R") != std::string::npos
      )
    {
      lifetime = "no_units";
    }

  size_t found = lifetime.find_first_of("<>~");
  if ( found != std::string::npos )
    {
      lifetime.at(found) = ' ';
    }

  extractValue(lifetime, 0, (NUBASE_END_HALFLIFEVALUE-NUBASE_START_HALFLIFEVALUE), hl);

  if ( hl < 0.0001 )
    {
      hl = (lifetime == "no_units") ? 1.0e-24 : 1.0e24;
    }
  else
    {
      std::string halfLifeUnit = full_data.substr(NUBASE_START_HALFLIFEUNIT,
                                                  (NUBASE_START_HALFLIFEUNIT-NUBASE_END_HALFLIFEUNIT)
                                                  );

      if( halfLifeUnit.find_first_not_of(' ') == std::string::npos )
        {
          halfLifeUnit = "ys";
        }

      if      (halfLifeUnit == "ys") hl*=1.0e-24;
      else if (halfLifeUnit == "zs") hl*=1.0e-21;
      else if (halfLifeUnit == "as") hl*=1.0e-18;
      else if (halfLifeUnit == "ps") hl*=1.0e-12;
      else if (halfLifeUnit == "ns") hl*=1.0e-09;
      else if (halfLifeUnit == "us") hl*=1.0e-06;
      else if (halfLifeUnit == "ms") hl*=1.0e-03;
      else if (halfLifeUnit == " s") hl*=1.0;
      else if (halfLifeUnit == " m") hl*=60.0;
      else if (halfLifeUnit == " h") hl*=3600.0;
      else if (halfLifeUnit == " d") hl*=86400.0;
      else if (halfLifeUnit == " y") hl*=31557600.0;
      else if (halfLifeUnit == "ky") hl*=31557600*1.0e03;
      else if (halfLifeUnit == "My") hl*=31557600*1.0e06;
      else if (halfLifeUnit == "Gy") hl*=31557600*1.0e09;
      else if (halfLifeUnit == "Ty") hl*=31557600*1.0e12;
      else if (halfLifeUnit == "Py") hl*=31557600*1.0e15;
      else if (halfLifeUnit == "Ey") hl*=31557600*1.0e18;
      else if (halfLifeUnit == "Zy") hl*=31557600*1.0e21;
      else if (halfLifeUnit == "Yy") hl*=31557600*1.0e24;
    }
}


void Nuclide::setDecayMode(std::vector<bool> &pnSide, const int year)
{
  // Store how ground-state decays in member decay
  if ( st != 0 )
    {
      decay = "isomer";
      return;
    }

  std::string Decay="isomer?";

  //Format changed after 2003 table
  const size_t startCharacter = ( year == 3 ) ? NUBASE_START_DECAYSTRING_03 : NUBASE_START_DECAYSTRING;

  if ( full_data.size() >= startCharacter )
    {
      Decay = full_data.substr(startCharacter);
    }

  // If more than 1 decay mode, they are separated by a ';'
  // Currently only want the 1st mode.
  if ( Decay.find(";") != std::string::npos )
    {
      Decay.erase(Decay.find(";"));
    }

  // Chop out everything after the '='
  if ( Decay.find("=")  != std::string::npos )
    {
      Decay.erase(Decay.find("="));
    }
  // Or convert a guess/estimate to unknown
  else if ( Decay.find(" ?") != std::string::npos )
    {
      Decay = "unknown";
    }

  // Remove any remaining unwanted characters from what
  // is left of the string.
  std::string unwantedCharacters("~<> ");
  size_t found = Decay.find_first_of(unwantedCharacters);

  while ( found != std::string::npos )
    {
      Decay.erase(found);
      found = Decay.find_first_of(unwantedCharacters,found+1);
    }

  // Book keeping
  // swap e+ for B+
  if ( Decay == "e+")
    {
      Decay = "B+";
    }
  // use "stable" instead of "IS"
  else if ( Decay == "IS")
    {
      Decay = "stable";

      // In the isotopic chain, mark the N value of the
      // first stable isotope.
      if ( !pnSide[Z] )
        pnSide[Z] = 1;
    }

  decay = Decay;
}


void Nuclide::setNeutronOrProtonRich(std::vector<bool> &pnSide)
{
  if ( !pnSide[Z] )
    {
      rich = 2;
    }
  else
    {
      rich = (decay == "stable") ? 6 : 3;
    }

  // Tc(43) and Pm(61) have no stable isotopes so set the 'stable' point by hand
  if (Z == 43)
    {
      rich = (A <= 96) ? 2 : 3;
    }
  else if (Z == 61)
    {
      rich = (A <= 144) ? 2 : 3;
    }
}
