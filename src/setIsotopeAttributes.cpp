#include "include/functions.h"

void setIsotopeAttributes(std::vector<Nuclide> &in,
			  partition *part,
			  const inputs *draw
			  )
{
  std::vector<Nuclide>::iterator it;

  // Using the region specified, flag that the isotope should be drawn
  // together with the corresponding part of the key.
  for (it=in.begin(); it!=in.end(); ++it)
    {
      if (   it->Z    >= draw->Zmin
	  && it->Z    <= draw->Zmax
	  && it->N    >= draw->Nmin
	  && it->N    <= draw->Nmax
	  && it->exp  != draw->experimental
	  && it->rich  % draw->np_rich == 0
	  )
	{
	  // Error on mass excess units of keV
	  if (draw->choice == "a")
	    {
	      if (it->st == 0)
		{
		  it->show=1;

		  double me = draw->AME ? it->AME_dME : it->NUBASE_dME;

		  if (it->decay == "stable" && me <= part->value[0])
		    {
		      it->colour = part->colour[0];
		      part->draw[0]=true;
		    }
		  else if (it->decay == "stable" && me > part->value[0])
		    {
		      it->colour = part->colour[1];
		      part->draw[1]=true;
		    }
		  else if (me <= part->value[0])
		    {
		      it->colour = part->colour[2];
		      part->draw[2]=true;
		    }
		  else if (me > part->value[0] && me <= part->value[1])
		    {
		      it->colour = part->colour[3];
		      part->draw[3]=true;
		    }
		  else if (me > part->value[1] && me <= part->value[2])
		    {
		      it->colour = part->colour[4];
		      part->draw[4]=true;
		    }
		  else if (me > part->value[2] && me <= part->value[3])
		    {
		      it->colour = part->colour[5];
		      part->draw[5]=true;
		    }
		  else if (me > part->value[3] && me <= part->value[4])
		    {
		      it->colour = part->colour[6];
		      part->draw[6]=true;
		    }
		  else
		    {
		      it->colour = part->colour[7];
		      part->draw[7]=true;
		    }
		}
	      else
		it->show=0;
	    }
	  // Relative error on mass excess units of keV
	  else if (draw->choice == "b")
	    {
	      if (it->st == 0)
		{
		  it->show=1;

		  double dme=0.0, min=1.0e-7;

		  if (draw->AME)
		    {
		      if ( fabs(it->AME_ME) < min)
			dme = 0.0;
		      else
			dme = fabs(it->AME_dME/it->AME_ME);
		    }
		  else
		    {
		      if ( fabs(it->NUBASE_ME) < min)
			dme = 0.0;
		      else
			dme = fabs(it->NUBASE_dME/it->NUBASE_ME);
		    }

		  if (dme <= part->value[0])
		    {
		      it->colour = part->colour[0];
		      part->draw[0]=true;
		    }
		  else if (dme > part->value[0] && dme <= part->value[1])
		    {
		      it->colour = part->colour[1];
		      part->draw[1]=true;
		    }
		  else if (dme > part->value[1] && dme <= part->value[2])
		    {
		      it->colour = part->colour[2];
		      part->draw[2]=true;
		    }
		  else if (dme > part->value[2] && dme <= part->value[3])
		    {
		      it->colour = part->colour[3];
		      part->draw[3]=true;
		    }
		  else if (dme > part->value[3] && dme <= part->value[4])
		    {
		      it->colour = part->colour[4];
		      part->draw[4]=true;
		    }
		  else if (dme > part->value[4] || dme < min)
		    {
		      it->colour = part->colour[5];
		      part->draw[5]=true;
		    }
		}
	      else
		it->show=0;
	    }
	  // Major ground-state decay mode
	  else if (draw->choice == "c")
	    {
	      if (it->st == 0)
		{
		  it->show=1;

		  if (it->decay == "stable")
		    {
		      it->colour = part->colour[0];
		      part->draw[0]=true;
		    }
		  else if (it->decay == "A")
		    {
		      it->colour = part->colour[1];
		      part->draw[1]=true;
		    }
		  else if (it->decay == "B-")
		    {
		      it->colour = part->colour[2];
		      part->draw[2]=true;
		    }
		  else if (it->decay == "B+")
		    {
		      it->colour = part->colour[3];
		      part->draw[3]=true;
		    }
		  else if (it->decay == "SF")
		    {
		      it->colour = part->colour[4];
		      part->draw[4]=true;
		    }
		  else if (it->decay == "n")
		    {
		      it->colour = part->colour[5];
		      part->draw[5]=true;
		    }
		  else if (it->decay == "2n")
		    {
		      it->colour = part->colour[6];
		      part->draw[6]=true;
		    }
		  else if (it->decay == "p")
		    {
		      it->colour = part->colour[7];
		      part->draw[7]=true;
		    }
		  else if (it->decay == "2p")
		    {
		      it->colour = part->colour[8];
		      part->draw[8]=true;
		    }
		  else if (it->decay == "unknown")
		    {
		      it->colour = part->colour[9];
		      part->draw[9]=true;
		    }
		  else if (it->decay == "EC")
		    {
		      it->colour = part->colour[10];
		      part->draw[10]=true;
		    }
		}
	      else
		it->show=0;
	    }
	  // Half-life of ground-state
	  else if (draw->choice == "d")
	    {
	      if (it->st == 0)
		{
		  it->show=1;

		  if (it->hl <= part->value[0])
		    {
		      it->colour = part->colour[0];
		      part->draw[0]=true;
		    }
		  else if (it->hl > part->value[0] && it->hl <= part->value[1])
		    {
		      it->colour = part->colour[1];
		      part->draw[1]=true;
		    }
		  else if (it->hl > part->value[1] && it->hl <= part->value[2])
		    {
		      it->colour = part->colour[2];
		      part->draw[2]=true;
		    }
		  else if (it->hl > part->value[2] && it->hl <= part->value[3])
		    {
		      it->colour = part->colour[3];
		      part->draw[3]=true;
		    }
		  else if (it->hl > part->value[3] && it->hl <= part->value[4])
		    {
		      it->colour = part->colour[4];
		      part->draw[4]=true;
		    }
		  else if (it->hl > part->value[4] && it->hl <= part->value[5])
		    {
		      it->colour = part->colour[5];
		      part->draw[5]=true;
		    }
		  else if (it->hl > part->value[5] && it->hl <= part->value[6])
		    {
		      it->colour = part->colour[6];
		      part->draw[6]=true;
		    }
		  else
		    {
		      it->colour = part->colour[7];
		      part->draw[7]=true;
		    }
		}
	      else
		it->show=0;
	    }
	  // 1st isomer energy
	  else if (draw->choice == "e")
	    {
	      if (it->st == 1)
		{
		  it->show=1;

		  if (it->is_nrg <= part->value[0])
		    {
		      it->colour = part->colour[0];
		      part->draw[0]=true;
		    }
		  else if (it->is_nrg > part->value[0] && it->is_nrg <= part->value[1])
		    {
		      it->colour = part->colour[1];
		      part->draw[1]=true;
		    }
		  else if (it->is_nrg > part->value[1] && it->is_nrg <= part->value[2])
		    {
		      it->colour = part->colour[2];
		      part->draw[2]=true;
		    }
		  else if (it->is_nrg > part->value[2] && it->is_nrg <= part->value[3])
		    {
		      it->colour = part->colour[3];
		      part->draw[3]=true;
		    }
		  else if (it->is_nrg > part->value[3] && it->is_nrg <= part->value[4])
		    {
		      it->colour = part->colour[4];
		      part->draw[4]=true;
		    }
		  else
		    {
		      it->colour = part->colour[5];
		      part->draw[5]=true;
		    }
		}
	      else if (it->st == 0 && (it+1)->st != 1)
		{
		  // As not every nucleus has an isomer, draw empty boxes as a visual aid
		  it->colour = "white";
		  part->draw[6]=true;
		  it->show=2;
		}
	    }
	}
    }
}
