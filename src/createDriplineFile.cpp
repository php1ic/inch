#include "include/functions.h"

void createDriplineFile(const std::vector<Nuclide> &nuc,
			const inputs *draw,
			const int &np
			)
{
  if( !checkFileExists(draw->FRDM) )
    {
      std::cout << "\n"
		<< "***ERROR***: Can't find " << draw->FRDM
		<< "\n" << std::endl;
      exit(-1);
    }

  std::ofstream dripFile;

  switch (np)
    {
    case 0:
      dripFile.open(draw->neutron_drip.c_str());
      if (dripFile.is_open())
	{
	  dripFile << "#Neutron drip line\n"
		   << "#calculated using the FRLDM\n"
		   << "#  N    Z   Sn[MeV]\n";
	}
      else
	{
	  std::cout << "**WARNING**: The file " << draw->neutron_drip
		    << " could not be opened. Not creating drip line file." << std::endl;
	  return;
	}
      break;
    case 1:
      dripFile.open(draw->two_neutron_drip.c_str());
      if (dripFile.is_open())
	{
	  dripFile << "#Two neutron drip line\n"
		   << "#calculated using the FRLDM\n"
		   << "#  N    Z   S2n[MeV]\n";
	}
      else
	{
	  std::cout << "**WARNING**: The file " << draw->two_neutron_drip
		    << " could not be opened. Not creating drip line file." << std::endl;
	  return;
	}
      break;
    case 2:
      dripFile.open(draw->proton_drip.c_str());
      if (dripFile.is_open())
	{
	  dripFile << "#Proton drip line\n"
		   << "#calculated using the FRLDM\n"
		   << "#  N    Z   Sp[MeV]\n";
	}
      else
	{
	  std::cout << "**WARNING**: The file " << draw->proton_drip
		    << " could not be opened. Not creating drip line file." << std::endl;
	  return;
	}
      break;
    case 3:
      dripFile.open(draw->two_proton_drip.c_str());
      if (dripFile.is_open())
	{
	  dripFile << "#Two proton drip line\n"
		   << "#calculated using the FRLDM\n"
		   << "#  N    Z   S2p[MeV]\n";
	}
      else
	{
	  std::cout << "**WARNING**: The file " << draw->two_proton_drip
		    << " could not be opened. Not creating drip line file." << std::endl;
	  return;
	}
      break;
    default:
      return;
    }

  dripFile << "#------------------" << std::endl;
  dripFile.precision(4);

  std::ifstream file(draw->FRDM.c_str());

  if ( !file.is_open() )
    {
      std::cout << "\n"
		<< "***ERROR***: " << draw->FRDM
		<< " couldn't be opened, does it exist?"
		<< "\n" << std::endl;

      dripFile.close();

      return;
    }

  int i=0;
  int z=7;
  int n=7;
  int nPrev=8;
  int zPrev=0;
  int zz=7;
  int nn=7;
  int nnPrev=8;
  int zzPrev=0;

  double meN=nuc[0].NUBASE_ME/1.0e3;
  double meP=nuc[1].NUBASE_ME/1.0e3;

  std::string line;

  /// TODO
  /// We don't need to use such a large class (Nuclide) to store
  /// four values.
  std::vector<Nuclide> dripNuc;
  dripNuc.reserve(countLinesInFile(file));

  while(getline(file,line))
    {
      if ( !line.compare("") || line[0] == '#' )
	continue;

      dripNuc.push_back(Nuclide(""));

      std::string Sym;
      std::stringstream in(line);
      in >> dripNuc[i].A >> dripNuc[i].Z >> Sym >> dripNuc[i].NUBASE_ME;

      dripNuc[i].N = dripNuc[i].A - dripNuc[i].Z;

      for (int j=0; j<i; ++j)
	{
	  if (dripNuc[i].A-dripNuc[j].A==1)
	    {
	      //---S(n) = M(Z,N-1) - M(Z,N) + M(0,1)
	      if (dripNuc[j].Z==dripNuc[i].Z && z<dripNuc[i].Z && np==0)
		{
		  dripNuc[i].s_n = dripNuc[j].NUBASE_ME - dripNuc[i].NUBASE_ME + meN;
		  if (dripNuc[i].s_n<0)
		    {
		      dripFile << std::fixed
			       << std::setw(4) << dripNuc[i].N   << " "
			       << std::setw(4) << dripNuc[i].Z   << " "
			       << std::setw(8) << dripNuc[i].s_n << "\n"
			       << std::setw(4) << dripNuc[i].N   << " "
			       << std::setw(4) << dripNuc[i].Z+1 << " "
			       << std::setw(8) << dripNuc[i].s_n << std::endl;
		      ++z;
		    }
		}

	      //---S(p) = M(Z-1,N) - M(Z,N) + M(1,0)
	      if (dripNuc[j].N==dripNuc[i].N && n<dripNuc[i].N && np==2)
		{
		  dripNuc[i].s_p = dripNuc[j].NUBASE_ME - dripNuc[i].NUBASE_ME + meP;

		  if (dripNuc[i].s_p<0)
		    {
		      if(dripNuc[i].N!=nPrev)
			dripFile << std::fixed
				 << std::setw(4) << nPrev+1   << " "
				 << std::setw(4) << zPrev     << " "
				 << std::setw(8) << dripNuc[i].s_p << std::endl;

		      dripFile << std::fixed
			       << std::setw(4) << dripNuc[i].N   << " "
			       << std::setw(4) << dripNuc[i].Z   << " "
			       << std::setw(8) << dripNuc[i].s_p << std::endl;

		      ++n;
		      zPrev = dripNuc[i].Z;
		      nPrev = dripNuc[i].N;
		    }
		}
	    }
	  else if (dripNuc[i].A-dripNuc[j].A==2)
	    {
	      //---S(2n) = M(Z,N-2) - M(Z,N) + 2*M(0,1)
	      if (dripNuc[j].Z==dripNuc[i].Z && zz<dripNuc[i].Z && np==1)
		{
		  dripNuc[i].s_2n = dripNuc[j].NUBASE_ME - dripNuc[i].NUBASE_ME + 2*meN;
		  if (dripNuc[i].s_2n<0)
		    {
		      dripFile << std::fixed
			       << std::setw(4) << dripNuc[i].N    << " "
			       << std::setw(4) << dripNuc[i].Z    << " "
			       << std::setw(8) << dripNuc[i].s_2n << "\n"
			       << std::setw(4) << dripNuc[i].N    << " "
			       << std::setw(4) << dripNuc[i].Z+1  << " "
			       << std::setw(8) << dripNuc[i].s_2n << std::endl;
		      ++zz;
		    }
		}

	      //---S(2p) = M(Z-2,N) - M(Z,N) + 2*M(1,0)
	      if (dripNuc[j].N==dripNuc[i].N && nn<dripNuc[i].N && np==3 )
		{
		  dripNuc[i].s_2p = dripNuc[j].NUBASE_ME - dripNuc[i].NUBASE_ME + 2*meP;
		  if (dripNuc[i].s_2p<0)
		    {
		      if (dripNuc[i].N!=nnPrev)
			dripFile << std::fixed
				 << std::setw(4) << nnPrev+1   << " "
				 << std::setw(4) << zzPrev     << " "
				 << std::setw(8) << dripNuc[i].s_2p << std::endl;

		      dripFile << std::fixed
			       << std::setw(4) << dripNuc[i].N    << " "
			       << std::setw(4) << dripNuc[i].Z    << " "
			       << std::setw(8) << dripNuc[i].s_2p << std::endl;

		      ++nn;
		      zzPrev = dripNuc[i].Z;
		      nnPrev = dripNuc[i].N;
		    }
		}
	    }
	}
      ++i;
    }
  file.close();

  dripFile.close();
}
