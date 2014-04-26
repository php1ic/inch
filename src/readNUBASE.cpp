#include "include/functions.h"

#include "extractValue.cpp"

bool readNUBASE(const std::string &table,
		std::vector<Nuclide> &nuc
		)
{
  std::cout << "Reading " << table.substr(table.find_last_of("/")+1) << " for nuclear values <--";

  if (!checkFileExists(table))
    {
      std::cout << "\nERROR: Mass table " << table << " couldn't be opened." << std::endl;
      return false;
    }

  std::ifstream file(table.c_str());

  std::vector<int> pn_side(119,0);
  std::string line("");

  if (file.is_open())
    {
      int i(0);

      while (getline(file,line))
	{
	  if (line.find("non-exist") != std::string::npos)
	    continue;

	  nuc.push_back( Nuclide() );

	  //-Make a substring for the spin and parity of the state if the information is there.
	  //-Do this prior to replacing all '#' with ' '
	  if (line.size() > 79)
	    {
	      //-As a general rule, the first value of spin and/or parity will be taken
	      //-Easiest to extract values by stripping away bits after use
	      std::string jpi = line.substr(79,14);

	      //-HACKS for those nuclei with non-unique assignments.
	      //-42Scr has (1+ to 4+) change to (1+)
	      if (jpi.find("(1+ to 4+)") != std::string::npos) jpi.replace(3,jpi.length(),")");
	      //-118Rh has gs J=4-10, take as 4
	      if (jpi.find("(4-10)") != std::string::npos) jpi.erase(jpi.find("(4-10)")+1,3);
	      //-Remove 'unhelpful' characters
	      if (jpi.find(">") != std::string::npos) jpi.erase(jpi.find(">"),1);
	      if (jpi.find("<") != std::string::npos) jpi.erase(jpi.find("<"),1);
	      //-176Tam has no J value, just (+). Easiest to say both are unknown
	      if (jpi.substr(0,3) == "(+)") jpi = "?";

	      //-If no parity is in the copied section then there is no assignment
	      if (jpi.find("+") == std::string::npos && jpi.find("-") == std::string::npos)
		{
		  nuc[i].J = 100.0;
		  nuc[i].pi = nuc[i].pi_exp = nuc[i].J_exp = nuc[i].J_tent = 2;
		}
	      else
		{
		  int exp, th;
		  if (jpi.find(" ") < 12)
		    jpi.resize(jpi.find(" "));

		  //-Take only the first parity if two are specified for the same state
		  if (jpi.find("+") != std::string::npos && jpi.find("-") != std::string::npos)
		    {
		      if (jpi.find_first_of("+") > jpi.find_first_of("-"))
			jpi.erase(jpi.find("+"),1);
		      else
			jpi.erase(jpi.find("-"),1);
		    }

		  //-Member pi is the parity (0)+ve or (1)-ve
		  //-Member pi_exp is the state exp(0) or theory/extrapolated(1)
		  if (jpi.find("+") != std::string::npos)
		    {
		      nuc[i].pi = 0;
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

		      nuc[i].pi_exp = (th > exp) ? 1 : 0;
		    }
		  else if (jpi.find("-") != std::string::npos)
		    {
		      nuc[i].pi = 1;
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

		      nuc[i].pi_exp = (th > exp) ? 1 : 0;
		    }

		  //-Stripping away the +/- will leave some () so remove them
		  if (jpi.find("()") != std::string::npos)
		    jpi.erase(jpi.find("()"),2);

		  //-Member J_tent shows either definite(0) or tentative(1) assignment
		  if (jpi.find("(") != std::string::npos)
		    {
		      jpi.erase(jpi.find("("),1);
		      jpi.erase(jpi.find(")"),1);
		      nuc[i].J_tent = 1;
		    }
		  else
		    nuc[i].J_tent = 0;

		  //-If multiple spins are given, take only the first
		  if (jpi.find(",")  != std::string::npos) jpi.erase(jpi.find(","));
		  if (jpi.find("..") != std::string::npos) jpi.erase(jpi.find(".."));

		  //-Member J_exp either experiment(0) or theory/extrapolated(1) assigment
		  if (jpi.find("#") != std::string::npos)
		    {
		      jpi.erase(jpi.find("#"),1);
		      nuc[i].J_exp = 1;
		    }
		  else
		    nuc[i].J_exp = 0;

		  //-Member J stores the spin as a double
		  if (jpi.find("/") == std::string::npos)
		    {
		      extractValue(jpi,0,jpi.length(),nuc[i].J);
		    }
		  else
		    {
		      extractValue(jpi,0,jpi.find("/"),nuc[i].J);
		      nuc[i].J /= 2.0;
		    }
		}
	    }
	  else
	    {
	      nuc[i].J = 100.0;
	      nuc[i].pi = nuc[i].pi_exp = nuc[i].J_exp = nuc[i].J_tent = 2;
	    }

	  //-Member exp has 0(experiment) or 1(theory/extrapolation) value
	  if (line.find("#") == std::string::npos)
	    nuc[i].exp = false;
	  else
	    {
	      nuc[i].exp = true;

	      //-Replace # (signifying theoretical/extrapolated values)
	      //-with empty space to maintain the line length
	      replace(line.begin(),line.end(),'#',' ');
	    }

	  //-Store the A value in member A
	  extractValue(line,0,3,nuc[i].A);

	  //-Store the Z value in member Z
	  extractValue(line,4,7,nuc[i].Z);

	  //-Store the symbol in member symbol
	  nuc[i].symbol = convertZToSymbol(nuc[i].Z);

	  //-Store the N value in member N
	  nuc[i].N = nuc[i].A - nuc[i].Z;

	  //-Member st contains state; 0 = gs, 1 = 1st isomer etc.
	  extractValue(line,7,8,nuc[i].st);

	  //-Store mass excess in member ME
	  extractValue(line,19,29,nuc[i].NUBASE_ME);

	  //-Store error on mass excess in member dME
	  extractValue(line,29,38,nuc[i].NUBASE_dME);

	  //-Calculate and store separation energies and dV_pn
	  for(int j=0; j<i; ++(j))
	    {
	      if(nuc[i].A-nuc[j].A==1)
		{
		  //-S_p(Z,N) = M(Z-1,N) - M(Z,N) + M(1,0)
		  if(nuc[i].N==nuc[j].N && nuc[i].Z-nuc[j].Z==1)
		    {
		      nuc[i].s_p  = nuc[j].NUBASE_ME - nuc[i].NUBASE_ME + nuc[1].NUBASE_ME;
		      nuc[i].ds_p = errorQuadrature(3,nuc[j].NUBASE_dME,nuc[i].NUBASE_dME,nuc[1].NUBASE_dME);
		    }
		  //-S_n(Z,N) = M(Z,N-1) - M(Z,N) + M(0,1)
		  if(nuc[i].Z==nuc[j].Z && nuc[i].N-nuc[j].N==1)
		    {
		      nuc[i].s_n  = nuc[j].NUBASE_ME - nuc[i].NUBASE_ME + nuc[0].NUBASE_ME;
		      nuc[i].ds_n = errorQuadrature(3,nuc[j].NUBASE_dME,nuc[i].NUBASE_dME,nuc[0].NUBASE_dME);
		    }
		}
	      else if(nuc[i].A-nuc[j].A==2)
		{
		  //-S_2p(Z,N) = M(Z-2,N) - M(Z,N) + 2*M(1,0)
		  if(nuc[i].N==nuc[j].N && nuc[i].Z-nuc[j].Z==2)
		    {
		      nuc[i].s_2p  = nuc[j].NUBASE_ME - nuc[i].NUBASE_ME + 2*nuc[1].NUBASE_ME;
		      nuc[i].ds_2p = errorQuadrature(4,nuc[j].NUBASE_dME,nuc[i].NUBASE_dME,nuc[1].NUBASE_dME,nuc[1].NUBASE_dME);
		    }
		  //-S_2n(Z,N) = M(Z,N-2) - M(Z,N) + 2*M(0,1)
		  if(nuc[i].Z==nuc[j].Z && nuc[i].N-nuc[j].N==2)
		    {
		      nuc[i].s_2n  = nuc[j].NUBASE_ME - nuc[i].NUBASE_ME + 2*nuc[0].NUBASE_ME;
		      nuc[i].ds_2n = errorQuadrature(4,nuc[j].NUBASE_dME,nuc[i].NUBASE_dME,nuc[0].NUBASE_dME,nuc[0].NUBASE_dME);

		      //-|dV_pn(Z,N)| = 1/4*[S_2p(Z,N) - S_2p(Z,N-2)]
		      nuc[i].dV_pn  = fabs((nuc[i].s_2p - nuc[j].s_2p)/4);
		      nuc[i].ddV_pn = errorQuadrature(2,nuc[j].ds_2p,nuc[i].ds_2p)/4;
		    }
		}
	    }

	  //-Store isomer energy in member is_nrg (gs has 1234.4321)
	  //-Store error on isomer energy in member dis_nrg (gs has 1234.4321)
	  if (nuc[i].st == 0)
	    nuc[i].is_nrg = nuc[i].dis_nrg = 1234.4321;
	  else
	    {
	      extractValue(line,39,46,nuc[i].is_nrg);
	      //Some isomers(3 in total) are measured via beta difference so come out -ve
	      nuc[i].is_nrg = fabs(nuc[i].is_nrg);

	      extractValue(line,48,56,nuc[i].dis_nrg);
	    }

	  //-Store half-life (in seconds) of the state in member hl
	  std::string hl_u(""), lifetime("");
	  float hl_t(0.0);

	  if (line.size() < 59)
	    lifetime = "no_units";
	  else
	    lifetime = line.substr(60,9);

	  if (   lifetime.find("p-unst") != std::string::npos
	      || lifetime.find_first_not_of(" ") == std::string::npos
	      || lifetime.find("R") != std::string::npos
	      )
	    lifetime = "no_units";

	       if (lifetime.find("<") != std::string::npos) lifetime.replace(lifetime.find("<"),1," ");
	  else if (lifetime.find(">") != std::string::npos) lifetime.replace(lifetime.find(">"),1," ");
	  else if (lifetime.find("~") != std::string::npos) lifetime.replace(lifetime.find("~"),1," ");

	  extractValue(lifetime,0,9,hl_t);

	  if (!hl_t)
	    {
	      if (lifetime == "no_units")
		{
		  hl_t = 1.0e-24;
		  hl_u = "ys";
		}
	      else
		{
		  hl_t = 1.0e24;
		  hl_u = "stbl";
		}
	    }
	  else
	    {
	      hl_u = line.substr(69,2);

	      if(hl_u.at(0) == ' ' && hl_u.at(1) == ' ')
		hl_u = "ys";

	      if      (hl_u == "ys") hl_t*=1.0e-24;
	      else if (hl_u == "zs") hl_t*=1.0e-21;
	      else if (hl_u == "as") hl_t*=1.0e-18;
	      else if (hl_u == "ps") hl_t*=1.0e-12;
	      else if (hl_u == "ns") hl_t*=1.0e-09;
	      else if (hl_u == "us") hl_t*=1.0e-06;
	      else if (hl_u == "ms") hl_t*=1.0e-03;
	      else if (hl_u == " s") hl_t*=1.0;
	      else if (hl_u == " m") hl_t*=60.0;
	      else if (hl_u == " h") hl_t*=3600.0;
	      else if (hl_u == " d") hl_t*=86400.0;
	      else if (hl_u == " y") hl_t*=31557600.0;
	      else if (hl_u == "ky") hl_t*=31557600*1.0e03;
	      else if (hl_u == "My") hl_t*=31557600*1.0e06;
	      else if (hl_u == "Gy") hl_t*=31557600*1.0e09;
	      else if (hl_u == "Ty") hl_t*=31557600*1.0e12;
	      else if (hl_u == "Py") hl_t*=31557600*1.0e15;
	      else if (hl_u == "Ey") hl_t*=31557600*1.0e18;
	      else if (hl_u == "Zy") hl_t*=31557600*1.0e21;
	      else if (hl_u == "Yy") hl_t*=31557600*1.0e24;
	    }
	  nuc[i].hl = hl_t;

	  //-Store how ground-state decays in member decay
	  if (nuc[i].st == 0)
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

	      //-Book keeping, swap e+ for B+, and use "stable" instead of "IS"
	      if (decay == "e+")
		decay = "B+";

	      if (decay == "IS")
		{
		  decay = "stable";

		  if ( !pn_side[nuc[i].Z] )
		    pn_side[nuc[i].Z] = 1;
		}

	      nuc[i].decay = decay;
	    }
	  else
	    nuc[i].decay = "isomer";

	  //-Store which side of the chart the nuclei is on
	  if ( !pn_side[nuc[i].Z] )
	    nuc[i].rich = 2;
	  else
	    nuc[i].rich = (nuc[i].decay == "stable") ? 6 : 3;

	  //-HACK-
	  //-Tc(43) and Pm(61) have no stable isotopes so set the 'stable' point by hand
	  if (nuc[i].Z == 43)
	    {
	      if (nuc[i].A <= 96)
		nuc[i].rich = 2;
	      else
		nuc[i].rich = 3;
	    }
	  if (nuc[i].Z == 61)
	    {
	      if (nuc[i].A <= 144)
		nuc[i].rich = 2;
	      else
		nuc[i].rich = 3;
	    }

	  ++i;
	}
      file.close();
    }
  else
    {
      std::cout << "\n\nERROR: " << table << " couldn't be opened, does it exist?\n" << std::endl;
      return false;
    }

  std::cout << "--> done" << std::endl;
  return true;
}
