#include "nuclide.hpp"

#include <iostream>

Nuclide::Nuclide(std::string line):
  full_data(std::move(line))
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

//Create a function to calculate the error on a value
//
//Template functions to do the maths
template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
constexpr T squarer(T v) noexcept
{
  return v*v;
}

template<typename T, typename... Args, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
constexpr T squarer(T first, Args... args) noexcept
{
  return first*first + squarer(args...);
}

//The actual function that will be called with a variable number of arguments
template<typename... Args>
constexpr double Nuclide::errorQuadrature(Args... args) const
{
  return std::sqrt(squarer(args...));
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
  if ( isalpha(jpi[0]) != 0 )
    {
      J = 100.0;
      pi = pi_exp = J_exp = J_tent = 2;

      return;
    }

  // trim trailing spaces
  size_t endpos = jpi.find_last_not_of(' ');
  if( std::string::npos != endpos )
    {
      jpi = jpi.substr( 0, endpos+1 );
    }

  // HACKS for those nuclei with non-unique assignments.
  //
  // 42Sc isomer 5 has (1+ to 4+) change to (1+)
  if (jpi.find("(1+ to 4+)") != std::string::npos)
    {
      jpi.replace(3,jpi.length(),")");
    }
  // 71Se isomer 1 has 1/2- to 9/2- change to 9/2-
  if (jpi.find("1/2- to 9/2-") != std::string::npos)
    {
      jpi.replace(4,jpi.length()," ");
    }
  // 142Ho has (6 to 9) change to (6)
  if (jpi.find("(6 to 9)") != std::string::npos)
    {
      jpi.replace(2,jpi.length(),")");
    }
  // 131I isomer 2 has (19/2+..23/2+) change to (19/2+)
  if (jpi.find("(19/2+..23/2+)") != std::string::npos)
    {
      jpi.replace(6,jpi.length(),")");
    }
  // 118Rh has gs J=4-10, take as 4
  if (jpi.find("(4-10)") != std::string::npos)
    {
      jpi.erase(jpi.find("(4-10)")+1,3);
    }
  // Remove 'unhelpful' characters
  if (jpi.find('>') != std::string::npos)
    {
      jpi.erase(jpi.find('>'),1);
    }
  if (jpi.find('<') != std::string::npos)
    {
      jpi.erase(jpi.find('<'),1);
    }
  // 176Tam has no J value, just (+). Easiest to say both are unknown
  if (jpi.substr(0,3) == "(+)")
    {
      jpi = "?";
    }

  // If no parity is in the copied section then there is no assignment
  // Set to 'unknown' and get out.
  if ( jpi.find('+') == std::string::npos && jpi.find('-') == std::string::npos )
    {
      J = 100.0;
      pi = pi_exp = J_exp = J_tent = 2;

      return;
    }

  // Take only the first parity if two are specified for the same state
  if ( jpi.find('+') != std::string::npos && jpi.find('-') != std::string::npos )
    {
      if ( jpi.find_first_of('+') > jpi.find_first_of('-') )
        {
          jpi.erase(jpi.find('+'),1);
        }
      else
        {
          jpi.erase(jpi.find('-'),1);
        }
    }

  bool experimental=false;

  // Member pi is the parity (0)+ve or (1)-ve
  // Member pi_exp is the state experimental(0) or theory/extrapolated(1)
  // We will remove the sign once we record it.
  if ( jpi.find('+') != std::string::npos )
    {
      pi = 0;
      experimental = false;
      do
        {
          if ( jpi.size() > (jpi.find('+')+1) && jpi.at(jpi.find('+')+1) == '#' )
            {
              jpi.erase(jpi.find('+'),2);
            }
          else
            {
              jpi.erase(jpi.find('+'),1);
              experimental=true;
            }
        }
      while ( jpi.find('+') != std::string::npos );

      pi_exp = experimental ? 0 : 1;
    }
  else if ( jpi.find('-') != std::string::npos )
    {
      pi = 1;
      experimental = false;
      do
        {
          if ( jpi.size() > (jpi.find('-')+1) && jpi.at(jpi.find('-')+1) == '#' )
            {
              jpi.erase(jpi.find('-'),2);
            }
          else
            {
              jpi.erase(jpi.find('-'),1);
              experimental=true;
            }
        }
      while ( jpi.find('-') != std::string::npos );

      pi_exp = experimental ? 0 : 1;
    }

  // Stripping away the +/- will leave some () so remove them
  if ( jpi.find("()") != std::string::npos )
    {
      jpi.erase(jpi.find("()"),2);
    }

  // Member J_tent shows either definite(0) or tentative(1) assignment
  if ( jpi.find('(') != std::string::npos )
    {
      jpi.erase(jpi.find('('),1);
      jpi.erase(jpi.find(')'),1);
      J_tent = 1;
    }
  else
    {
      J_tent = 0;
    }

  // If multiple spins are given, take only the first
  if ( jpi.find(',') != std::string::npos )
    {
      jpi.erase(jpi.find(','));
    }

  // Member J_exp either experiment(0) or theory/extrapolated(1) assigment
  if ( jpi.find('#') != std::string::npos )
    {
      jpi.erase(jpi.find('#'),1);
      J_exp = 1;
    }
  else
    {
      J_exp = 0;
    }

  // Member J stores the spin as a double
  if ( jpi.find('/') == std::string::npos )
    {
      extractValue(jpi,0,jpi.length(),J);
    }
  else
    {
      extractValue(jpi,0,jpi.find('/'),J);
      J /= 2.0;
    }
}


void Nuclide::setExperimental()
{
  // Member exp has false(experiment) or true(theory/extrapolation) value
  // Will use mass excess for criteria, the last digit is char NUBASE_END_DME (38)
  //so if  there is a '#' but it's after this we will still say experimental
  auto measured = full_data.find_first_of('#');

  if ( measured != std::string::npos )
    {
      std::replace(std::begin(full_data), std::end(full_data), '#', ' ');
    }

  exp = ( measured > NUBASE_END_DME )
    ? 1
    : 0;
}


void Nuclide::setSeparationEnergies(std::vector<Nuclide> &nuc)
{
  int numSeparationEnergiesRead = 0;

  double n_ME  = nuc[0].NUBASE_ME;
  double n_dME = nuc[0].NUBASE_dME;
  double p_ME  = nuc[1].NUBASE_ME;
  double p_dME = nuc[1].NUBASE_dME;

  // Loop from the penultimate isotope towards the beginning.
  // As the vector is ordered by A (low to high), this will
  // remove a large number of checks as the vector get bigger.
  for ( auto previous = std::crbegin(nuc); previous != std::crend(nuc); ++previous )
    {
      // Only calculate for ground states.
      if ( previous->st != 0 )
        {
          continue;
        }

      // Single particle energies.
      if ( A - previous->A == 1 )
        {
          // S_p(Z,N) = M(Z-1,N) - M(Z,N) + M(1,0)
          if (    N - previous->N == 0
               && Z - previous->Z == 1 )
            {
              s_p  = previous->NUBASE_ME - NUBASE_ME + p_ME;
              ds_p = errorQuadrature(previous->NUBASE_dME, NUBASE_dME, p_dME);
              numSeparationEnergiesRead++;
            }
          // S_n(Z,N) = M(Z,N-1) - M(Z,N) + M(0,1)
          else if (    Z - previous->Z == 0
                    && N - previous->N == 1 )
            {
              s_n  = previous->NUBASE_ME - NUBASE_ME + n_ME;
              ds_n = errorQuadrature(previous->NUBASE_dME, NUBASE_dME, n_dME);
              numSeparationEnergiesRead++;
            }
        }
      // Two particle energies.
      else if ( A - previous->A == 2 )
        {
          // S_2p(Z,N) = M(Z-2,N) - M(Z,N) + 2*M(1,0)
          if (    N - previous->N == 0
               && Z - previous->Z == 2 )
            {
              s_2p  = previous->NUBASE_ME - NUBASE_ME + 2*p_ME;
              ds_2p = errorQuadrature(previous->NUBASE_dME, NUBASE_dME, p_dME, p_dME);
              numSeparationEnergiesRead++;
            }
          // S_2n(Z,N) = M(Z,N-2) - M(Z,N) + 2*M(0,1)
          else if (    Z - previous->Z == 0
                    && N - previous->N == 2 )
            {
              s_2n  = previous->NUBASE_ME - NUBASE_ME + 2*n_ME;
              ds_2n = errorQuadrature(previous->NUBASE_dME, NUBASE_dME, n_dME, n_dME);

              // |dV_pn(Z,N)| = 1/4*[S_2p(Z,N) - S_2p(Z,N-2)]
              dV_pn  = fabs(0.25*(s_2p - previous->s_2p));
              ddV_pn = 0.25*errorQuadrature(previous->ds_2p, ds_2p);
              numSeparationEnergiesRead++;
            }
        }
      // Once the difference in A is greater than 2 we wont get any more useful data
      // so set the 'exit variable' to the get out value.
      else if ( A - previous->A >= 3 )
        {
          numSeparationEnergiesRead = 4;
        }

      // Get out if we have recorded/calculated all of the values.
      if ( numSeparationEnergiesRead == 4 )
        {
          break;
        }
    }
}


void Nuclide::setIsomerData()
{
  if ( st == 0 )
    {
      return;
    }

  setIsomerEnergy();

  // Some isomers(3 in total) are measured via beta difference so come out -ve
  if ( is_nrg < 0.0 )
    {
      is_nrg = std::fabs(is_nrg);
    }

  setIsomerEnergyError();
}

const std::string Nuclide::noUnits {"no_units"};

void Nuclide::setHalfLife()
{
  // Annoying data file format strikes again
  // Line length is not always as long as the half life position
  // Create a temporary string with either the half life or a know value
  std::string lifetime = (full_data.size() < (NUBASE_START_HALFLIFEVALUE-1))
    ? noUnits
    : full_data.substr(NUBASE_START_HALFLIFEVALUE,
                       (NUBASE_END_HALFLIFEVALUE-NUBASE_START_HALFLIFEVALUE)
                       );

  // Certain string mean we should not try and parse them as half lives
  // If they are found, convert to our know value
  if (   lifetime.find_first_not_of(' ') == std::string::npos
      || lifetime.find("p-unst") != std::string::npos
      || lifetime.find('R') != std::string::npos
      )
    {
      lifetime = noUnits;
    }

  // Not currently interested in approximations or limits
  std::string remove {"<>~"};
  std::transform(std::begin(lifetime), std::end(lifetime), std::begin(lifetime),
                 [&remove](const char c)
                  {
                    return remove.find(c) != std::string::npos ? ' ' : c;
                  }
                 );

  // Extract the numeric value from the temporary string we created
  extractValue(lifetime, 0, (NUBASE_END_HALFLIFEVALUE - NUBASE_START_HALFLIFEVALUE), hl);

  // Stable (stbl) and empty values (no_units) will have numeric value 0
  if ( hl > 0.0 )
    {
      setHalfLifeUnit();

      if ( halflife_unit.find_first_not_of(' ') == std::string::npos )
        {
          halflife_unit = "ys";
        }

      if (halflife_unit == "ys")
        {
          hl*=1.0e-24;
        }
      else if (halflife_unit == "zs")
        {
          hl*=1.0e-21;
        }
      else if (halflife_unit == "as")
        {
          hl*=1.0e-18;
        }
      else if (halflife_unit == "ps")
        {
          hl*=1.0e-12;
        }
      else if (halflife_unit == "ns")
        {
          hl*=1.0e-09;
        }
      else if (halflife_unit == "us")
        {
          hl*=1.0e-06;
        }
      else if (halflife_unit == "ms")
        {
          hl*=1.0e-03;
        }
      else if (halflife_unit == " s")
        {
          hl*=static_cast<double>(TimeInSeconds::seconds);
        }
      else if (halflife_unit == " m")
        {
          hl*=static_cast<double>(TimeInSeconds::minutes);
        }
      else if (halflife_unit == " h")
        {
          hl*=static_cast<double>(TimeInSeconds::hours);
        }
      else if (halflife_unit == " d")
        {
          hl*=static_cast<double>(TimeInSeconds::days);
        }
      else if (halflife_unit == " y")
        {
          hl*=static_cast<double>(TimeInSeconds::years);
        }
      else if (halflife_unit == "ky")
        {
          hl*=1.0e03*static_cast<double>(TimeInSeconds::years);
        }
      else if (halflife_unit == "My")
        {
          hl*=1.0e06*static_cast<double>(TimeInSeconds::years);
        }
      else if (halflife_unit == "Gy")
        {
          hl*=1.0e09*static_cast<double>(TimeInSeconds::years);
        }
      else if (halflife_unit == "Ty")
        {
          hl*=1.0e12*static_cast<double>(TimeInSeconds::years);
        }
      else if (halflife_unit == "Py")
        {
          hl*=1.0e15*static_cast<double>(TimeInSeconds::years);
        }
      else if (halflife_unit == "Ey")
        {
          hl*=1.0e18*static_cast<double>(TimeInSeconds::years);
        }
      else if (halflife_unit == "Zy")
        {
          hl*=1.0e21*static_cast<double>(TimeInSeconds::years);
        }
      else if (halflife_unit == "Yy")
        {
          hl*=1.0e24*static_cast<double>(TimeInSeconds::years);
        }
    }
  else
    {
      //If noUnits assume unknown so very short half life
      //else stable so very long
      hl = (lifetime == noUnits) ? 1.0e-24 : 1.0e24;
    }
}


void Nuclide::setDecayMode(std::vector<bool> &pnSide, const int table_year)
{
  // Store how ground-state decays in member decay
  if ( st != 0 )
    {
      decay = "isomer";
      return;
    }

  std::string Decay="isomer?";

  //Format changed after 2003 table
  const size_t startCharacter = ( table_year == 3 ) ? NUBASE_START_DECAYSTRING_03 : NUBASE_START_DECAYSTRING;

  if ( full_data.size() >= startCharacter )
    {
      Decay = full_data.substr(startCharacter);
    }

  // If more than 1 decay mode, they are separated by a ';'
  // Currently only want the 1st mode.
  if ( Decay.find(';') != std::string::npos )
    {
      Decay.erase(Decay.find(';'));
    }

  // Chop out everything after the '='
  if ( Decay.find('=')  != std::string::npos )
    {
      Decay.erase(Decay.find('='));
    }
  // Or convert a guess/estimate to unknown
  else if ( Decay.find(" ?") != std::string::npos )
    {
      Decay = "unknown";
    }

  // Remove from remaining unwanted characters to end
  auto found = Decay.find_first_of("~<> ");

  if ( found != std::string::npos )
    {
      Decay.erase(found);
    }

  // Book keeping
  // swap e+ for B+
  if ( Decay == "e+")
    {
      Decay = "B+";
    }
  // use "stable" instead of "IS"
  else if ( Decay == "IS" )
    {
      Decay = "stable";

      // In the isotopic chain, mark the N value of the
      // first stable isotope.
      if ( !pnSide[Z] )
        {
          pnSide[Z] = true;
        }
    }

  decay = Decay;
}


void Nuclide::setNeutronOrProtonRich(const std::vector<bool> &pnSide)
{
  rich = ( !pnSide.at(Z) )
    ? 2
    : ( decay == "stable" ) ? 6 : 3;

  // Tc(43) and Pm(61) have no stable isotopes so set the 'stable' point by hand
  switch( Z )
    {
    case 43 :
      rich = ( A <=  96 ) ? 2 : 3;
      break;
    case 61 :
      rich = ( A <= 144 ) ? 2 : 3;
      break;
    }
}
