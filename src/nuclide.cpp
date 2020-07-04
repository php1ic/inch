#include "inch/nuclide.hpp"

#include "inch/converter.hpp"

#include <fmt/format.h>

#include <algorithm>
#include <cmath>


void Nuclide::setSpinParity() const
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
  if (full_data.size() <= NUBASE_START_SPIN || full_data.at(NUBASE_START_SPIN) == ' ')
    {
      J  = 100.0;
      pi = pi_exp = J_exp = J_tent = 2;

      return;
    }

  // Easiest to extract values by stripping away bits after use
  std::string jpi = full_data.substr(NUBASE_START_SPIN, (NUBASE_END_SPIN - NUBASE_START_SPIN));

  // Some values are set as words (high, low, mix, spmix, fsmix, am)
  // Don't want this so set to 'unknown' and get out.
  if (isalpha(jpi[0]) != 0)
    {
      J  = 100.0;
      pi = pi_exp = J_exp = J_tent = 2;

      return;
    }

  // trim trailing spaces
  size_t endpos = jpi.find_last_not_of(' ');
  if (std::string::npos != endpos)
    {
      jpi = jpi.substr(0, endpos + 1);
    }

  // HACKS for those nuclei with non-unique assignments.
  //
  // 42Sc isomer 5 has (1+ to 4+) change to (1+)
  if (jpi.find("(1+ to 4+)") != std::string::npos)
    {
      jpi.replace(3, jpi.length(), ")");
    }
  // 71Se isomer 1 has 1/2- to 9/2- change to 9/2-
  if (jpi.find("1/2- to 9/2-") != std::string::npos)
    {
      jpi.replace(4, jpi.length(), " ");
    }
  // 142Ho has (6 to 9) change to (6)
  if (jpi.find("(6 to 9)") != std::string::npos)
    {
      jpi.replace(2, jpi.length(), ")");
    }
  // 131I isomer 2 has (19/2+..23/2+) change to (19/2+)
  if (jpi.find("(19/2+..23/2+)") != std::string::npos)
    {
      jpi.replace(6, jpi.length(), ")");
    }
  // 118Rh has gs J=4-10, take as 4
  if (jpi.find("(4-10)") != std::string::npos)
    {
      jpi.erase(jpi.find("(4-10)") + 1, 3);
    }
  // Remove 'unhelpful' characters
  if (jpi.find('>') != std::string::npos)
    {
      jpi.erase(jpi.find('>'), 1);
    }
  if (jpi.find('<') != std::string::npos)
    {
      jpi.erase(jpi.find('<'), 1);
    }
  // 176Tam has no J value, just (+). Easiest to say both are unknown
  if (jpi.substr(0, 3) == "(+)")
    {
      jpi = "?";
    }

  // If no parity is in the copied section then there is no assignment
  // Set to 'unknown' and get out.
  if (jpi.find('+') == std::string::npos && jpi.find('-') == std::string::npos)
    {
      J  = 100.0;
      pi = pi_exp = J_exp = J_tent = 2;

      return;
    }

  // Take only the first parity if two are specified for the same state
  if (jpi.find('+') != std::string::npos && jpi.find('-') != std::string::npos)
    {
      if (jpi.find_first_of('+') > jpi.find_first_of('-'))
        {
          jpi.erase(jpi.find('+'), 1);
        }
      else
        {
          jpi.erase(jpi.find('-'), 1);
        }
    }

  // Member pi is the parity (0)+ve or (1)-ve
  // Member pi_exp is the state experimental(0) or theory/extrapolated(1)
  // We will remove the sign once we record it.
  if (jpi.find('+') != std::string::npos)
    {
      pi = 0;
      bool experimental{ false };
      do
        {
          if (jpi.size() > (jpi.find('+') + 1) && jpi.at(jpi.find('+') + 1) == '#')
            {
              jpi.erase(jpi.find('+'), 2);
            }
          else
            {
              jpi.erase(jpi.find('+'), 1);
              experimental = true;
            }
        }
      while (jpi.find('+') != std::string::npos);

      pi_exp = experimental ? 0 : 1;
    }
  else if (jpi.find('-') != std::string::npos)
    {
      pi = 1;
      bool experimental{ false };
      do
        {
          if (jpi.size() > (jpi.find('-') + 1) && jpi.at(jpi.find('-') + 1) == '#')
            {
              jpi.erase(jpi.find('-'), 2);
            }
          else
            {
              jpi.erase(jpi.find('-'), 1);
              experimental = true;
            }
        }
      while (jpi.find('-') != std::string::npos);

      pi_exp = experimental ? 0 : 1;
    }

  // Stripping away the +/- will leave some () so remove them
  if (jpi.find("()") != std::string::npos)
    {
      jpi.erase(jpi.find("()"), 2);
    }

  // Member J_tent shows either definite(0) or tentative(1) assignment
  if (jpi.find('(') != std::string::npos)
    {
      jpi.erase(jpi.find('('), 1);
      jpi.erase(jpi.find(')'), 1);
      J_tent = 1;
    }
  else
    {
      J_tent = 0;
    }

  // If multiple spins are given, take only the first
  if (jpi.find(',') != std::string::npos)
    {
      jpi.erase(jpi.find(','));
    }

  // Member J_exp either experiment(0) or theory/extrapolated(1) assigment
  if (jpi.find('#') != std::string::npos)
    {
      jpi.erase(jpi.find('#'), 1);
      J_exp = 1;
    }
  else
    {
      J_exp = 0;
    }

  // Member J stores the spin as a double
  if (jpi.find('/') == std::string::npos)
    {
      J = Converter::StringToDouble(jpi, 0, jpi.length());
    }
  else
    {
      J = 0.5 * Converter::StringToDouble(jpi, 0, jpi.find('/'));
    }
}


void Nuclide::setExperimental() const
{
  // Member exp has false(experiment) or true(theory/extrapolation) value
  // Will use mass excess for criteria, the last digit is char NUBASE_END_DME (38)
  // so if there is a '#' but it's after this we will still say experimental
  const auto measured = full_data.find_first_of('#');

  if (measured != std::string::npos)
    {
      std::replace(full_data.begin(), full_data.end(), '#', ' ');
    }

  exp = (measured > NUBASE_END_DME) ? 1 : 0;
}


void Nuclide::setSeparationEnergies(const std::vector<Nuclide>& nuc) const
{
  int numSeparationEnergiesRead{ 0 };

  const double n_ME  = nuc[0].NUBASE_ME;
  const double n_dME = nuc[0].NUBASE_dME;
  const double p_ME  = nuc[1].NUBASE_ME;
  const double p_dME = nuc[1].NUBASE_dME;

  // Loop from the penultimate isotope towards the beginning.
  // As the vector is ordered by A (low to high), this will
  // remove a large number of checks as the vector get bigger.
  for (auto previous = nuc.crbegin(); previous != nuc.crend(); ++previous)
    {
      // Single particle energies.
      if (A - previous->A == 1)
        {
          // S_p(Z,N) = M(Z-1,N) - M(Z,N) + M(1,0)
          if (N - previous->N == 0 && Z - previous->Z == 1)
            {
              s_p  = previous->NUBASE_ME - NUBASE_ME + p_ME;
              ds_p = errorQuadrature(previous->NUBASE_dME, NUBASE_dME, p_dME);
              numSeparationEnergiesRead++;
            }
          // S_n(Z,N) = M(Z,N-1) - M(Z,N) + M(0,1)
          else if (Z - previous->Z == 0 && N - previous->N == 1)
            {
              s_n  = previous->NUBASE_ME - NUBASE_ME + n_ME;
              ds_n = errorQuadrature(previous->NUBASE_dME, NUBASE_dME, n_dME);
              numSeparationEnergiesRead++;
            }
        }
      // Two particle energies.
      else if (A - previous->A == 2)
        {
          // S_2p(Z,N) = M(Z-2,N) - M(Z,N) + 2*M(1,0)
          if (N - previous->N == 0 && Z - previous->Z == 2)
            {
              s_2p  = previous->NUBASE_ME - NUBASE_ME + 2 * p_ME;
              ds_2p = errorQuadrature(previous->NUBASE_dME, NUBASE_dME, p_dME, p_dME);
              numSeparationEnergiesRead++;
            }
          // S_2n(Z,N) = M(Z,N-2) - M(Z,N) + 2*M(0,1)
          else if (Z - previous->Z == 0 && N - previous->N == 2)
            {
              s_2n  = previous->NUBASE_ME - NUBASE_ME + 2 * n_ME;
              ds_2n = errorQuadrature(previous->NUBASE_dME, NUBASE_dME, n_dME, n_dME);

              // |dV_pn(Z,N)| = 1/4*[S_2p(Z,N) - S_2p(Z,N-2)]
              dV_pn  = fabs(0.25 * (s_2p - previous->s_2p));
              ddV_pn = 0.25 * errorQuadrature(previous->ds_2p, ds_2p);
              numSeparationEnergiesRead++;
            }
        }
      // Once the difference in A is greater than 2 we wont get any more useful data
      // so set the 'exit variable' to the get out value.
      else if (A - previous->A >= 3)
        {
          numSeparationEnergiesRead = 4;
        }

      // Get out if we have recorded/calculated all of the values.
      if (numSeparationEnergiesRead == 4)
        {
          break;
        }
    }
}


void Nuclide::setIsomerData(std::vector<Nuclide>& nuc, const int state) const
{
  // Loop from the penultimate isotope towards the beginning.
  // Original order is ground state followed by ascending states,
  // theoretically we could just modify nuc.back(), but that's not safe
  for (auto previous = nuc.rbegin(); previous != nuc.rend(); ++previous)
    {
      if (A == previous->A && Z == previous->Z)
        {
          double energy{ 0.0 };
          double error{ 0.0 };

          setIsomerEnergy(energy);

          // Some isomers(3 in total) are measured via beta difference so come out -ve
          if (energy < 0.0)
            {
              energy = std::fabs(energy);
            }

          setIsomerEnergyError(error);

          previous->energy_levels.emplace_back(State(state, energy, error));
          break;
        }
    }
}


const std::string& Nuclide::missingUnit()
{
  static const std::string noUnits{ "no_units" };
  return noUnits;
}


void Nuclide::setHalfLife() const
{
  // Annoying data file format strikes again
  // Line length is not always as long as the half life position
  // Create a temporary string with either the half life or a know value
  std::string lifetime =
      (full_data.size() < (NUBASE_START_HALFLIFEVALUE - 1))
          ? missingUnit()
          : full_data.substr(NUBASE_START_HALFLIFEVALUE, (NUBASE_END_HALFLIFEVALUE - NUBASE_START_HALFLIFEVALUE));

  // Certain string mean we should not try and parse them as half lives
  // If they are found, convert to our know value
  if (lifetime.find_first_not_of(' ') == std::string::npos || lifetime.find("p-unst") != std::string::npos
      || lifetime.find('R') != std::string::npos)
    {
      lifetime = missingUnit();
    }

  // Not currently interested in approximations or limits
  std::string_view remove{ "<>~" };
  std::transform(lifetime.begin(), lifetime.end(), lifetime.begin(), [&remove](const char c) {
    return remove.find(c) != std::string::npos ? ' ' : c;
  });

  // If noUnits assume unknown so very short half life
  if (lifetime == missingUnit())
    {
      hl = Converter::seconds{ 1.0e-24 };
    }
  // If stable set to very long
  else if (lifetime.find("stbl") != std::string::npos)
    {
      hl = Converter::seconds{ 1.0e24 };
    }
  else
    {
      const double hl_double =
          Converter::StringToDouble(lifetime, 0, NUBASE_END_HALFLIFEVALUE - NUBASE_START_HALFLIFEVALUE);

      setHalfLifeUnit();

      if (halflife_unit.find_first_not_of(' ') == std::string::npos)
        {
          halflife_unit = "ys";
        }

      if (halflife_unit == "ys")
        {
          hl = Converter::attoseconds{ 1.0e-6 * hl_double };
        }
      else if (halflife_unit == "zs")
        {
          hl = Converter::attoseconds{ 1.0e-3 * hl_double };
        }
      else if (halflife_unit == "as")
        {
          hl = Converter::attoseconds{ hl_double };
        }
      else if (halflife_unit == "ps")
        {
          hl = Converter::picoseconds{ hl_double };
        }
      else if (halflife_unit == "ns")
        {
          hl = Converter::nanoseconds{ hl_double };
        }
      else if (halflife_unit == "us")
        {
          hl = Converter::microseconds{ hl_double };
        }
      else if (halflife_unit == "ms")
        {
          hl = Converter::milliseconds{ hl_double };
        }
      else if (halflife_unit == "s")
        {
          hl = Converter::seconds{ hl_double };
        }
      else if (halflife_unit == "m")
        {
          hl = Converter::minutes{ hl_double };
        }
      else if (halflife_unit == "h")
        {
          hl = Converter::hours{ hl_double };
        }
      else if (halflife_unit == "d")
        {
          hl = Converter::days{ hl_double };
        }
      else if (halflife_unit == "y")
        {
          hl = Converter::years{ hl_double };
        }
      else if (halflife_unit == "ky")
        {
          hl = Converter::kiloyears{ hl_double };
        }
      else if (halflife_unit == "My")
        {
          hl = Converter::millionyears{ hl_double };
        }
      else if (halflife_unit == "Gy")
        {
          hl = Converter::billionyears{ hl_double };
        }
      else if (halflife_unit == "Ty")
        {
          hl = Converter::billionyears{ 1.0e3 * hl_double };
        }
      else if (halflife_unit == "Py")
        {
          hl = Converter::billionyears{ 1.0e6 * hl_double };
        }
      else if (halflife_unit == "Ey")
        {
          hl = Converter::billionyears{ 1.0e9 * hl_double };
        }
      else if (halflife_unit == "Zy")
        {
          hl = Converter::billionyears{ 1.0e12 * hl_double };
        }
      else if (halflife_unit == "Yy")
        {
          hl = Converter::billionyears{ 1.0e15 * hl_double };
        }
    }
}


void Nuclide::setDecayMode(std::vector<bool>& pnSide, const int table_year) const
{
  // Store how ground-state decays in member decay
  std::string Decay{ "isomer?" };

  // Format changed after 2003 table
  const size_t startCharacter = (table_year == 3) ? NUBASE_START_DECAYSTRING_03 : NUBASE_START_DECAYSTRING;

  if (full_data.size() >= startCharacter)
    {
      Decay = full_data.substr(startCharacter);
    }

  // If more than 1 decay mode, they are separated by a ';'
  // Currently only want the 1st mode.
  if (Decay.find(';') != std::string::npos)
    {
      Decay.erase(Decay.find(';'));
    }

  // Chop out everything after the '='
  if (Decay.find('=') != std::string::npos)
    {
      Decay.erase(Decay.find('='));
    }
  // Or convert a guess/estimate to unknown
  else if (Decay.find(" ?") != std::string::npos)
    {
      Decay = "unknown";
    }

  // Remove from remaining unwanted characters to end
  const auto found = Decay.find_first_of("~<> ");

  if (found != std::string::npos)
    {
      Decay.erase(found);
    }

  // Book keeping
  // swap e+ for B+
  if (Decay == "e+")
    {
      Decay = "B+";
    }
  // use "stable" instead of "IS"
  else if (Decay == "IS")
    {
      Decay = "stable";

      // In the isotopic chain, mark the N value of the
      // first stable isotope.
      if (!pnSide[Z])
        {
          pnSide[Z] = true;
        }
    }

  decay = Decay;
}


void Nuclide::setNeutronOrProtonRich(const std::vector<bool>& pnSide) const
{
  rich = (!pnSide.at(Z)) ? 2 : (decay == "stable") ? 6 : 3;

  // Tc(43) and Pm(61) have no stable isotopes so set the 'stable' point by hand
  switch (Z)
    {
      case 43:
        rich = (A <= 96) ? 2 : 3;
        break;
      case 61:
        rich = (A <= 144) ? 2 : 3;
        break;
    }
}

double Nuclide::getRelativeMassExcessError(const bool AME, const double min_allowed) const
{
  // 12C has an ME of 0.0 by definition.
  // This is the only place it currently trips us up and this path does not always happen so, in terms of doing as
  // little work as possible, this is the best place to check and make adjustments.
  // The value we set does not matter as the error on the value is also 0.0 so relative error is guarenteed to be 0.0
  if (A == 12 && Z == 6)
    {
      NUBASE_ME = 0.0001;
      AME_ME    = 0.0001;
    }

  return AME ? (fabs(AME_dME / AME_ME) < min_allowed) ? min_allowed : fabs(AME_dME / AME_ME)
             : (fabs(NUBASE_dME / NUBASE_ME) < min_allowed) ? min_allowed : fabs(NUBASE_dME / NUBASE_ME);
}

std::string Nuclide::writeAsEPS(const Options& draw) const
{
  // Everything is set, draw the isotope
  // Add comment in the eps file with isotope ID
  // This is for easy navigation if manually altering the file
  return fmt::format("%{}{}\n"
                     "{} {} {} {} {} {} curve Nucleus\n",
                     A,
                     symbol,
                     getDisplayMode(draw.chart_colour),
                     getIsotopeTextColour(draw.chart_colour, draw.write_isotope),
                     draw.write_isotope ? fmt::format("({}) ({})", symbol, A) : "",
                     colour,
                     (N - draw.limits.Nmin),
                     (Z - draw.limits.Zmin));
}


std::string Nuclide::CSVHeader() const
{
  return "A,Z,N,Symbol,DecayMode,ExperimentallyMeasured,Own,NUBASEMass,ErrorNUBASEMass,AMEMass,ErrorAMEMass,HalfLife,"
         "SingleNeutronSeparationEnergy,ErrorSingleNeutronSeparationEnergy,SingleProtonSeparationEnergy,"
         "ErrorSingleProtonSeparationEnergy,DoubleNeutronSeparationEnergy,ErrorDoubleNeutronSeparationEnergy,"
         "DoubleProtonSeparationEnergy,ErrorDoubleProtonSeparationEnergy,DiscoveryYear";
}


std::string Nuclide::writeAsCSV(std::string_view sep) const
{
  return fmt::format("{1}{0}{2}{0}{3}{0}{4}{0}{5}{0}{6}{0}{7}{0}{8}{0}{9}{0}{10}{0}{11}{0}{12}{0}{13}{0}{14}{0}{15}{0}{"
                     "16}{0}{17}{0}{18}{0}{19}{0}{20}{0}{21}",
                     sep,
                     A,
                     Z,
                     N,
                     symbol,
                     decay,
                     exp,
                     own,
                     NUBASE_ME,
                     NUBASE_dME,
                     AME_ME,
                     AME_dME,
                     hl.count(),
                     s_n,
                     ds_n,
                     s_p,
                     ds_p,
                     s_2n,
                     ds_2n,
                     s_2p,
                     ds_2p,
                     year);
}


std::string Nuclide::writeAsJSON() const
{
  return fmt::format("{{\"A\":{},\"Z\":{},\"N\":{},\"Symbol\":\"{}\",\"Decay\":\"{}\",\"Experiemntal\":{},\"Own\":{},"
                     "\"NubaseMassExcess\":{},\"ErrorNubaseMassExcess\":{},\"AMEMassExcess\":{},\"ErrorAMEMassExcess\":"
                     "{},\"HalfLife\":{},\"SingleNeutronSeparationEnergy\":{},\"ErrorSingleNeutronSeparationEnergy\":{}"
                     ",\"SingleProtonSeparationEnergy\":{},\"ErrorSingleProtonSeparationEnergy\":{},"
                     "\"DoubleNeutronSeparationEnergy\":{},\"ErrorDoubleNeutronSeparationEnergy\":{},"
                     "\"DoubleProtonSeparationEnergy\":{},\"ErrorDoubleProtonSeparationEnergy\":{},\"Year\":{}}}",
                     A,
                     Z,
                     N,
                     symbol,
                     decay,
                     exp,
                     own,
                     NUBASE_ME,
                     NUBASE_dME,
                     AME_ME,
                     AME_dME,
                     hl.count(),
                     s_n,
                     ds_n,
                     s_p,
                     ds_p,
                     s_2n,
                     ds_2n,
                     s_2p,
                     ds_2p,
                     year);
}
