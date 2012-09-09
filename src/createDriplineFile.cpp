#include "functions.h"

void createDriplineFile(std::vector<Nuclide> &nuc, inputs *draw, int np)
{
  std::vector<Nuclide> drip_nuc;
  std::vector<Nuclide>::iterator nuc_it;

  int i,j,
    nn=7,
    zz=7,
    z=7,
    n=7,
    n_prev=8,
    nn_prev=8,
    z_prev=0,
    zz_prev=0;

  double
    ME_n=nuc[0].NUBASE_ME/1e3,
    ME_p=nuc[1].NUBASE_ME/1e3;

  std::string *line = new std::string;
  std::string Sym;
  std::stringstream in;
  std::ofstream drip_file;

  if (np == 0)
    {
      drip_file.open(draw->neutron_drip.c_str());
      if (drip_file.is_open())
	{
	  drip_file << "#Neutron drip line\n"
		    << "#calculated using the FRLDM\n"
		    << "#  N    Z   Sn[MeV]\n";
	}
      //else
    }
  else if (np == 1)
    {
      drip_file.open(draw->two_neutron_drip.c_str());
      if (drip_file.is_open())
	{
	  drip_file << "#Two neutron drip line\n"
		    << "#calculated using the FRLDM\n"
		    << "#  N    Z   S2n[MeV]\n";
	}
      //else
    }
  else if (np == 2)
    {
      drip_file.open(draw->proton_drip.c_str());
      if (drip_file.is_open())
	{
	  drip_file << "#Proton drip line\n"
		    << "#calculated using the FRLDM\n"
		    << "#  N    Z   Sp[MeV]\n";
	}
      //else
    }
  else if (np == 3)
    {
      drip_file.open(draw->two_proton_drip.c_str());
      if (drip_file.is_open())
	{
	  drip_file << "#Two proton drip line\n"
		    << "#calculated using the FRLDM\n"
		    << "#  N    Z   S2p[MeV]\n";
	}
      //else
    }
  else
    {
      return;
    }

  drip_file << "#------------------" << std::endl;
  drip_file.precision(4);

  std::ifstream file(draw->FRDM.c_str());
  if(!file.is_open())
    {
      std::cout << "\n\tERROR: Can't find " << draw->FRDM.c_str() << "\n" << std::endl;
      exit(-1);
    }

  i=0;
  while(getline(file,*line))
    {
      if (line->at(0)!='#')
	{
	  drip_nuc.push_back(Nuclide());

	  in.clear();
	  in << *line;

	  in >> drip_nuc[i].A >> drip_nuc[i].Z >> Sym >> drip_nuc[i].NUBASE_ME;

	  drip_nuc[i].N  = drip_nuc[i].A-drip_nuc[i].Z;

	  for (j=0; j<i; j++)
	    {
	      if (drip_nuc[i].A-drip_nuc[j].A==1)
		{
		  //---S(n) = M(Z,N-1) - M(Z,N) + M(0,1)
		  if (drip_nuc[j].Z==drip_nuc[i].Z && z<drip_nuc[i].Z && np==0)
		    {
		      drip_nuc[i].s_n = drip_nuc[j].NUBASE_ME - drip_nuc[i].NUBASE_ME + ME_n;
		      if (drip_nuc[i].s_n<0)
			{
			  drip_file << std::fixed
				    << std::setw(4) << drip_nuc[i].N   << " "
				    << std::setw(4) << drip_nuc[i].Z   << " "
				    << std::setw(8) << drip_nuc[i].s_n << "\n"
				    << std::setw(4) << drip_nuc[i].N   << " "
				    << std::setw(4) << drip_nuc[i].Z+1 << " "
				    << std::setw(8) << drip_nuc[i].s_n << std::endl;
			  z++;
			}
		    }

		  //---S(p) = M(Z-1,N) - M(Z,N) + M(1,0)
		  if (drip_nuc[j].N==drip_nuc[i].N && n<drip_nuc[i].N && np==2)
		    {
		      drip_nuc[i].s_p = drip_nuc[j].NUBASE_ME - drip_nuc[i].NUBASE_ME + ME_p;

		      if (drip_nuc[i].s_p<0)
			{
			  if(drip_nuc[i].N!=n_prev)
			    drip_file << std::fixed
				      << std::setw(4) << n_prev+1   << " "
				      << std::setw(4) << z_prev     << " "
				      << std::setw(8) << drip_nuc[i].s_p << std::endl;

			  drip_file << std::fixed
				    << std::setw(4) << drip_nuc[i].N   << " "
				    << std::setw(4) << drip_nuc[i].Z   << " "
				    << std::setw(8) << drip_nuc[i].s_p << std::endl;

			  n++;
			  z_prev = drip_nuc[i].Z;
			  n_prev = drip_nuc[i].N;
			}
		    }
		}
	      if (drip_nuc[i].A-drip_nuc[j].A==2)
		{
		  //---S(2n) = M(Z,N-2) - M(Z,N) + 2*M(0,1)
		  if (drip_nuc[j].Z==drip_nuc[i].Z && zz<drip_nuc[i].Z && np==1)
		    {
		      drip_nuc[i].s_2n = drip_nuc[j].NUBASE_ME - drip_nuc[i].NUBASE_ME + 2*ME_n;
		      if (drip_nuc[i].s_2n<0)
			{
			  drip_file << std::fixed
				    << std::setw(4) << drip_nuc[i].N    << " "
				    << std::setw(4) << drip_nuc[i].Z    << " "
				    << std::setw(8) << drip_nuc[i].s_2n << "\n"
				    << std::setw(4) << drip_nuc[i].N    << " "
				    << std::setw(4) << drip_nuc[i].Z+1  << " "
				    << std::setw(8) << drip_nuc[i].s_2n << std::endl;
			  zz++;
			}
		    }

		  //---S(2p) = M(Z-2,N) - M(Z,N) + 2*M(1,0)
		  if (drip_nuc[j].N==drip_nuc[i].N && nn<drip_nuc[i].N && np==3 )
		    {
		      drip_nuc[i].s_2p = drip_nuc[j].NUBASE_ME - drip_nuc[i].NUBASE_ME + 2*ME_p;
		      if(drip_nuc[i].s_2p<0)
			{
			  if (drip_nuc[i].N!=nn_prev)
			    drip_file << std::fixed
				      << std::setw(4) << nn_prev+1   << " "
				      << std::setw(4) << zz_prev     << " "
				      << std::setw(8) << drip_nuc[i].s_2p << std::endl;

			  drip_file << std::fixed
				    << std::setw(4) << drip_nuc[i].N    << " "
				    << std::setw(4) << drip_nuc[i].Z    << " "
				    << std::setw(8) << drip_nuc[i].s_2p << std::endl;

			  nn++;
			  zz_prev = drip_nuc[i].Z;
			  nn_prev = drip_nuc[i].N;
			}
		    }
		}
	    }
	  i++;
	}
    }

  drip_file.close();
  file.close();
}