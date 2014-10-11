#include "include/functions.h"

bool readNUBASE(const std::string &table,
		std::vector<Nuclide> &nuc
		)
{
  std::cout << "Reading " << table.substr(table.find_last_of("/")+1)
	    << " for nuclear values <--";

  if ( !checkFileExists(table) )
    {
      std::cout << "\n"
		<< "***ERROR***: Mass table " << table
		<< " couldn't be opened." << std::endl;
      return false;
    }

  std::ifstream file(table.c_str());

  if ( !file.is_open() )
    {
      std::cout << "\n"
		<< "***ERROR***: " << table
		<< " couldn't be opened, does it exist?\n" << std::endl;
      return false;
    }


  // Reserving space avoids multiple calls to the copy constructor
  nuc.reserve(countLinesInFile(file));

  std::vector<int> pnSide(MAX_Z+1,0);
  std::string line;

  while (getline(file,line))
    {
      if (line.find("non-exist") != std::string::npos)
	continue;

      Nuclide isotope;

      // Make a substring for the spin and parity of the state if the information is there.
      // Do this prior to replacing all '#' with '
      if (line.size() > 79)
	{
	  // As a general rule, the first value of spin and/or parity will be taken
	  // Easiest to extract values by stripping away bits after use
	  std::string jpi = line.substr(79,14);

	  // HACKS for those nuclei with non-unique assignments.
	  // 42Scr has (1+ to 4+) change to (1+)
	  if (jpi.find("(1+ to 4+)") != std::string::npos) jpi.replace(3,jpi.length(),")");
	  // 118Rh has gs J=4-10, take as 4
	  if (jpi.find("(4-10)") != std::string::npos) jpi.erase(jpi.find("(4-10)")+1,3);
	  // Remove 'unhelpful' characters
	  if (jpi.find(">") != std::string::npos) jpi.erase(jpi.find(">"),1);
	  if (jpi.find("<") != std::string::npos) jpi.erase(jpi.find("<"),1);
	  // 176Tam has no J value, just (+). Easiest to say both are unknown
	  if (jpi.substr(0,3) == "(+)") jpi = "?";

	  // If no parity is in the copied section then there is no assignment
	  if (jpi.find("+") == std::string::npos && jpi.find("-") == std::string::npos)
	    {
	      isotope.J = 100.0;
	      isotope.pi = isotope.pi_exp = isotope.J_exp = isotope.J_tent = 2;
	    }
	  else
	    {
	      int exp;
	      int th;
	      if (jpi.find(" ") < 12)
		jpi.resize(jpi.find(" "));

	      // Take only the first parity if two are specified for the same state
	      if (jpi.find("+") != std::string::npos && jpi.find("-") != std::string::npos)
		{
		  if (jpi.find_first_of("+") > jpi.find_first_of("-"))
		    jpi.erase(jpi.find("+"),1);
		  else
		    jpi.erase(jpi.find("-"),1);
		}

	      // Member pi is the parity (0)+ve or (1)-ve
	      // Member pi_exp is the state exp(0) or theory/extrapolated(1)
	      if (jpi.find("+") != std::string::npos)
		{
		  isotope.pi = 0;
		  th = exp = 0;
		  do
		    {
		      if (jpi.size() > (jpi.find("+")+1) && jpi.at(jpi.find("+")+1) == '#')
			{
			  jpi.erase(jpi.find("+"),2);
			  ++th;
			}
		      else
			{
			  jpi.erase(jpi.find("+"),1);
			  ++exp;
			}
		    }
		  while (jpi.find("+") != std::string::npos);

		  isotope.pi_exp = (th > exp) ? 1 : 0;
		}
	      else if (jpi.find("-") != std::string::npos)
		{
		  isotope.pi = 1;
		  th = exp = 0;
		  do
		    {
		      if (jpi.size() > (jpi.find("-")+1) && jpi.at(jpi.find("-")+1) == '#')
			{
			  jpi.erase(jpi.find("-"),2);
			  ++th;
			}
		      else
			{
			  jpi.erase(jpi.find("-"),1);
			  ++exp;
			}
		    }
		  while (jpi.find("-") != std::string::npos);

		  isotope.pi_exp = (th > exp) ? 1 : 0;
		}

	      // Stripping away the +/- will leave some () so remove them
	      if (jpi.find("()") != std::string::npos)
		jpi.erase(jpi.find("()"),2);

	      // Member J_tent shows either definite(0) or tentative(1) assignment
	      if (jpi.find("(") != std::string::npos)
		{
		  jpi.erase(jpi.find("("),1);
		  jpi.erase(jpi.find(")"),1);
		  isotope.J_tent = 1;
		}
	      else
		isotope.J_tent = 0;

	      // If multiple spins are given, take only the first
	      if (jpi.find(",")  != std::string::npos) jpi.erase(jpi.find(","));
	      if (jpi.find("..") != std::string::npos) jpi.erase(jpi.find(".."));

	      // Member J_exp either experiment(0) or theory/extrapolated(1) assigment
	      if (jpi.find("#") != std::string::npos)
		{
		  jpi.erase(jpi.find("#"),1);
		  isotope.J_exp = 1;
		}
	      else
		isotope.J_exp = 0;

	      // Member J stores the spin as a double
	      if (jpi.find("/") == std::string::npos)
		{
		  extractValue(jpi,0,jpi.length(),isotope.J);
		}
	      else
		{
		  extractValue(jpi,0,jpi.find("/"),isotope.J);
		  isotope.J /= 2.0;
		}
	    }
	}
      else
	{
	  isotope.J = 100.0;
	  isotope.pi = isotope.pi_exp = isotope.J_exp = isotope.J_tent = 2;
	}

      // Member exp has false(experiment) or true(theory/extrapolation) value
      // Will use mass excess for criteria, the last digit is char 38 so if
      // there is a '#' but it's after this we will still say experimental
      size_t measured = line.find_first_of("#");
      if (measured == std::string::npos || measured > 38)
	isotope.exp = 1;
      else
	isotope.exp = 0;

      // Replace # (signifying theoretical/extrapolated values)
      // with empty space to maintain the line length
      if (measured != std::string::npos)
	replace(line.begin(),line.end(),'#',' ');

      // Store the A value in member A
      extractValue(line,0,3,isotope.A);

      // Store the Z value in member Z
      extractValue(line,4,7,isotope.Z);

      // Store the N value in member N
      isotope.N = isotope.A - isotope.Z;

      // Member st contains state; 0 = gs, 1 = 1st isomer etc.
      extractValue(line,7,8,isotope.st);

      // Store mass excess in member ME
      extractValue(line,19,29,isotope.NUBASE_ME);

      // Store error on mass excess in member dME
      extractValue(line,29,38,isotope.NUBASE_dME);

      // Calculate and store separation energies and dV_pn
      if (isotope.st == 0)
	{
	  int numDripLinesRead=0;

	  std::vector<Nuclide>::iterator it;

	  for (it=nuc.end(); it>=nuc.begin(); --it)
	    {
	      if (it->st == 0)
		{
		  if (isotope.A - it->A == 1)
		    {
		      // S_p(Z,N) = M(Z-1,N) - M(Z,N) + M(1,0)
		      if (   isotope.N - it->N == 0
			  && isotope.Z - it->Z == 1)
			{
			  isotope.s_p  = it->NUBASE_ME - isotope.NUBASE_ME + nuc[1].NUBASE_ME;
			  isotope.ds_p = errorQuadrature(3,it->NUBASE_dME,isotope.NUBASE_dME,nuc[1].NUBASE_dME);
			  numDripLinesRead++;
			}
		      // S_n(Z,N) = M(Z,N-1) - M(Z,N) + M(0,1)
		      else if (   isotope.Z - it->Z == 0
			       && isotope.N - it->N == 1)
			{
			  isotope.s_n  = it->NUBASE_ME - isotope.NUBASE_ME + nuc[0].NUBASE_ME;
			  isotope.ds_n = errorQuadrature(3,it->NUBASE_dME,isotope.NUBASE_dME,nuc[0].NUBASE_dME);
			  numDripLinesRead++;
			}
		    }
		  else if (isotope.A - it->A == 2)
		    {
		      // S_2p(Z,N) = M(Z-2,N) - M(Z,N) + 2*M(1,0)
		      if (   isotope.N - it->N == 0
			  && isotope.Z - it->Z == 2)
			{
			  isotope.s_2p  = it->NUBASE_ME - isotope.NUBASE_ME + 2*nuc[1].NUBASE_ME;
			  isotope.ds_2p = errorQuadrature(4,it->NUBASE_dME,isotope.NUBASE_dME,nuc[1].NUBASE_dME,nuc[1].NUBASE_dME);
			  numDripLinesRead++;
			}
		      // S_2n(Z,N) = M(Z,N-2) - M(Z,N) + 2*M(0,1)
		      else if (   isotope.Z - it->Z == 0
			       && isotope.N - it->N == 2)
			{
			  isotope.s_2n  = it->NUBASE_ME - isotope.NUBASE_ME + 2*nuc[0].NUBASE_ME;
			  isotope.ds_2n = errorQuadrature(4,it->NUBASE_dME,isotope.NUBASE_dME,nuc[0].NUBASE_dME,nuc[0].NUBASE_dME);

			  // |dV_pn(Z,N)| = 1/4*[S_2p(Z,N) - S_2p(Z,N-2)]
			  isotope.dV_pn  = fabs(0.25*(isotope.s_2p - it->s_2p));
			  isotope.ddV_pn = 0.25*errorQuadrature(2,it->ds_2p,isotope.ds_2p);
			  numDripLinesRead++;
			}
		    }
		  else if (isotope.A - it->A >= 3)
		    numDripLinesRead=4;
		}

	      if (numDripLinesRead == 4) break;
	    }
	}

      // Store isomer energy in member is_nrg (gs has 1234.4321)
      // Store error on isomer energy in member dis_nrg (gs has 1234.4321)
      if (isotope.st == 0)
	{
	  isotope.is_nrg = isotope.dis_nrg = 1234.4321;
	}
      else
	{
	  extractValue(line,39,46,isotope.is_nrg);
	  // Some isomers(3 in total) are measured via beta difference so come out -ve
	  isotope.is_nrg = fabs(isotope.is_nrg);

	  extractValue(line,48,56,isotope.dis_nrg);
	}

      // Store half-life (in seconds) of the state in member hl
      std::string halfLifeUnit;
      std::string lifetime;
      double halfLife=0.0;

      lifetime = (line.size() < 59) ? "no_units" : line.substr(60,9);

      if (   lifetime.find("p-unst") != std::string::npos
	  || lifetime.find_first_not_of(" ") == std::string::npos
	  || lifetime.find("R") != std::string::npos
	  )
	{
	  lifetime = "no_units";
	}

           if (lifetime.find("<") != std::string::npos) lifetime.replace(lifetime.find("<"),1," ");
      else if (lifetime.find(">") != std::string::npos) lifetime.replace(lifetime.find(">"),1," ");
      else if (lifetime.find("~") != std::string::npos) lifetime.replace(lifetime.find("~"),1," ");

      extractValue(lifetime,0,9,halfLife);

      if (halfLife < 0.0001)
	{
	  if (lifetime == "no_units")
	    {
	      halfLife = 1.0e-24;
	      halfLifeUnit = "ys";
	    }
	  else
	    {
	      halfLife = 1.0e24;
	      halfLifeUnit = "stbl";
	    }
	}
      else
	{
	  halfLifeUnit = line.substr(69,2);

	  if(halfLifeUnit.at(0) == ' ' && halfLifeUnit.at(1) == ' ')
	    halfLifeUnit = "ys";

	  if      (halfLifeUnit == "ys") halfLife*=1.0e-24;
	  else if (halfLifeUnit == "zs") halfLife*=1.0e-21;
	  else if (halfLifeUnit == "as") halfLife*=1.0e-18;
	  else if (halfLifeUnit == "ps") halfLife*=1.0e-12;
	  else if (halfLifeUnit == "ns") halfLife*=1.0e-09;
	  else if (halfLifeUnit == "us") halfLife*=1.0e-06;
	  else if (halfLifeUnit == "ms") halfLife*=1.0e-03;
	  else if (halfLifeUnit == " s") halfLife*=1.0;
	  else if (halfLifeUnit == " m") halfLife*=60.0;
	  else if (halfLifeUnit == " h") halfLife*=3600.0;
	  else if (halfLifeUnit == " d") halfLife*=86400.0;
	  else if (halfLifeUnit == " y") halfLife*=31557600.0;
	  else if (halfLifeUnit == "ky") halfLife*=31557600*1.0e03;
	  else if (halfLifeUnit == "My") halfLife*=31557600*1.0e06;
	  else if (halfLifeUnit == "Gy") halfLife*=31557600*1.0e09;
	  else if (halfLifeUnit == "Ty") halfLife*=31557600*1.0e12;
	  else if (halfLifeUnit == "Py") halfLife*=31557600*1.0e15;
	  else if (halfLifeUnit == "Ey") halfLife*=31557600*1.0e18;
	  else if (halfLifeUnit == "Zy") halfLife*=31557600*1.0e21;
	  else if (halfLifeUnit == "Yy") halfLife*=31557600*1.0e24;
	}
      isotope.hl = halfLife;

      // Store how ground-state decays in member decay
      if (isotope.st == 0)
	{
	  std::string decay="isomer?";

	  if (line.size() >= 106)
	    decay = line.substr(106);

	  if (decay.find(";")  != std::string::npos) decay.erase(decay.find(";"));
	  if (decay.find("=")  != std::string::npos) decay.erase(decay.find("="));
	  if (decay.find("~")  != std::string::npos) decay.erase(decay.find("~"));
	  if (decay.find(">")  != std::string::npos) decay.erase(decay.find(">"));
	  if (decay.find("<")  != std::string::npos) decay.erase(decay.find("<"));
	  if (decay.find(" ?") != std::string::npos) decay = "unknown";
	  if (decay.find(" ")  != std::string::npos) decay.erase(decay.find(" "));

	  // Book keeping, swap e+ for B+, and use "stable" instead of "IS"
	  if (decay == "e+")
	    decay = "B+";

	  if (decay == "IS")
	    {
	      decay = "stable";

	      if ( !pnSide[isotope.Z] )
		pnSide[isotope.Z] = 1;
	    }

	  isotope.decay = decay;
	}
      else
	isotope.decay = "isomer";

      // Store which side of the chart the nuclei is on
      if ( !pnSide[isotope.Z] )
	isotope.rich = 2;
      else
	isotope.rich = (isotope.decay == "stable") ? 6 : 3;

      // Tc(43) and Pm(61) have no stable isotopes so set the 'stable' point by hand
      if (isotope.Z == 43)
	isotope.rich = (isotope.A <= 96) ? 2 : 3;
      else if (isotope.Z == 61)
	isotope.rich = (isotope.A <= 144) ? 2 : 3;

      nuc.push_back(isotope);
    }

  file.close();

  std::cout << "--> done" << std::endl;
  return true;
}
