#include "include/functions.h"

void showNuclei(std::vector<Nuclide> &in,
		partition *part,
		const inputs *draw
		)
{
  std::vector<Nuclide>::iterator nuc_it;

  //-Using the region specified set the colour of each isotope and draw it
  for (nuc_it=in.begin(); nuc_it!=in.end(); ++nuc_it)
    {
      if (   nuc_it->Z    >= draw->Zmin
	  && nuc_it->Z    <= draw->Zmax
	  && nuc_it->N    >= draw->Nmin
	  && nuc_it->N    <= draw->Nmax
	  && nuc_it->exp  != draw->experimental
	  && nuc_it->rich  % draw->np_rich == 0
	  )
	{
	  nuc_it->show=false;
	  //-Error on mass excess units of keV
	  if (draw->choice == "a")
	    {
	      if (nuc_it->st == 0)
		{
		  nuc_it->show=true;

		  int me;

		  if (draw->AME)
		    me = nuc_it->AME_dME;
		  else
		    me = nuc_it->NUBASE_dME;

		  if (nuc_it->decay == "stable" && me <= part->value[0])
		    {
		      nuc_it->colour = part->colour[0];
		      part->draw[0]=true;
		    }
		  else if (nuc_it->decay == "stable" && me > part->value[0])
		    {
		      nuc_it->colour = part->colour[1];
		      part->draw[1]=true;
		    }
		  else if (me <= part->value[0])
		    {
		      nuc_it->colour = part->colour[2];
		      part->draw[2]=true;
		    }
		  else if (me > part->value[0] && me <= part->value[1])
		    {
		      nuc_it->colour = part->colour[3];
		      part->draw[3]=true;
		    }
		  else if (me > part->value[1] && me <= part->value[2])
		    {
		      nuc_it->colour = part->colour[4];
		      part->draw[4]=true;
		    }
		  else if (me > part->value[2] && me <= part->value[3])
		    {
		      nuc_it->colour = part->colour[5];
		      part->draw[5]=true;
		    }
		  else if (me > part->value[3] && me <= part->value[4])
		    {
		      nuc_it->colour = part->colour[6];
		      part->draw[6]=true;
		    }
		  else
		    {
		      nuc_it->colour = part->colour[7];
		      part->draw[7]=true;
		    }
		}
	      else
		nuc_it->show=false;
	    }
	  //-Relative error on mass excess units of keV
	  else if (draw->choice == "b")
	    {
	      if (nuc_it->st == 0)
		{
		  nuc_it->show=true;

		  float dme;

		  if (draw->AME)
		    {
		      if (!nuc_it->AME_ME)
			dme = 0;
		      else
			dme = fabs(nuc_it->AME_dME/nuc_it->AME_ME);
		    }
		  else
		    {
		      if (!nuc_it->NUBASE_ME)
			dme = 0;
		      else
			dme = fabs(nuc_it->NUBASE_dME/nuc_it->NUBASE_ME);
		    }

		  if (dme <= part->value[0])
		    {
		      nuc_it->colour = part->colour[0];
		      part->draw[0]=true;
		    }
		  else if (dme > part->value[0] && dme <= part->value[1])
		    {
		      nuc_it->colour = part->colour[1];
		      part->draw[1]=true;
		    }
		  else if (dme > part->value[1] && dme <= part->value[2])
		    {
		      nuc_it->colour = part->colour[2];
		      part->draw[2]=true;
		    }
		  else if (dme > part->value[2] && dme <= part->value[3])
		    {
		      nuc_it->colour = part->colour[3];
		      part->draw[3]=true;
		    }
		  else if (dme > part->value[3] && dme <= part->value[4])
		    {
		      nuc_it->colour = part->colour[4];
		      part->draw[4]=true;
		    }
		  else if (dme > part->value[4] || dme == 0)
		    {
		      nuc_it->colour = part->colour[5];
		      part->draw[5]=true;
		    }
		}
	      else
		nuc_it->show=false;
	    }
	  //-Major ground-state decay mode
	  else if (draw->choice == "c")
	    {
	      if (nuc_it->st == 0)
		{
		  nuc_it->show=true;

		  if (nuc_it->decay == "stable")
		    {
		      nuc_it->colour = part->colour[0];
		      part->draw[0]=true;
		    }
		  else if (nuc_it->decay == "A")
		    {
		      nuc_it->colour = part->colour[1];
		      part->draw[1]=true;
		    }
		  else if (nuc_it->decay == "B-")
		    {
		      nuc_it->colour = part->colour[2];
		      part->draw[2]=true;
		    }
		  else if (nuc_it->decay == "B+")
		    {
		      nuc_it->colour = part->colour[3];
		      part->draw[3]=true;
		    }
		  else if (nuc_it->decay == "SF")
		    {
		      nuc_it->colour = part->colour[4];
		      part->draw[4]=true;
		    }
		  else if (nuc_it->decay == "n")
		    {
		      nuc_it->colour = part->colour[5];
		      part->draw[5]=true;
		    }
		  else if (nuc_it->decay == "2n")
		    {
		      nuc_it->colour = part->colour[6];
		      part->draw[6]=true;
		    }
		  else if (nuc_it->decay == "p")
		    {
		      nuc_it->colour = part->colour[7];
		      part->draw[7]=true;
		    }
		  else if (nuc_it->decay == "2p")
		    {
		      nuc_it->colour = part->colour[8];
		      part->draw[8]=true;
		    }
		  else if (nuc_it->decay == "unknown")
		    {
		      nuc_it->colour = part->colour[9];
		      part->draw[9]=true;
		    }
		  else if (nuc_it->decay == "EC")
		    {
		      nuc_it->colour = part->colour[10];
		      part->draw[10]=true;
		    }
		}
	      else
		nuc_it->show=false;
	    }
	  //-Half-life of ground-state
	  else if (draw->choice == "d")
	    {
	      if (nuc_it->st == 0)
		{
		  nuc_it->show=true;

		  if (nuc_it->hl <= part->value[0])
		    {
		      nuc_it->colour = part->colour[0];
		      part->draw[0]=true;
		    }
		  else if (nuc_it->hl > part->value[0] && nuc_it->hl <= part->value[1])
		    {
		      nuc_it->colour = part->colour[1];
		      part->draw[1]=true;
		    }
		  else if (nuc_it->hl > part->value[1] && nuc_it->hl <= part->value[2])
		    {
		      nuc_it->colour = part->colour[2];
		      part->draw[2]=true;
		    }
		  else if (nuc_it->hl > part->value[2] && nuc_it->hl <= part->value[3])
		    {
		      nuc_it->colour = part->colour[3];
		      part->draw[3]=true;
		    }
		  else if (nuc_it->hl > part->value[3] && nuc_it->hl <= part->value[4])
		    {
		      nuc_it->colour = part->colour[4];
		      part->draw[4]=true;
		    }
		  else if (nuc_it->hl > part->value[4] && nuc_it->hl <= part->value[5])
		    {
		      nuc_it->colour = part->colour[5];
		      part->draw[5]=true;
		    }
		  else if (nuc_it->hl > part->value[5] && nuc_it->hl <= part->value[6])
		    {
		      nuc_it->colour = part->colour[6];
		      part->draw[6]=true;
		    }
		  else
		    {
		      nuc_it->colour = part->colour[7];
		      part->draw[7]=true;
		    }
		}
	      else
		nuc_it->show=false;
	    }
	  //-1st isomer energy
	  else if (draw->choice == "e")
	    {
	      if (nuc_it->st == 1)
		{
		  nuc_it->show=true;

		  if (nuc_it->is_nrg <= part->value[0])
		    {
		      nuc_it->colour = part->colour[0];
		      part->draw[0]=true;
		    }
		  else if (nuc_it->is_nrg > part->value[0] && nuc_it->is_nrg <= part->value[1])
		    {
		      nuc_it->colour = part->colour[1];
		      part->draw[1]=true;
		    }
		  else if (nuc_it->is_nrg > part->value[1] && nuc_it->is_nrg <= part->value[2])
		    {
		      nuc_it->colour = part->colour[2];
		      part->draw[2]=true;
		    }
		  else if (nuc_it->is_nrg > part->value[2] && nuc_it->is_nrg <= part->value[3])
		    {
		      nuc_it->colour = part->colour[3];
		      part->draw[3]=true;
		    }
		  else if (nuc_it->is_nrg > part->value[3] && nuc_it->is_nrg <= part->value[4])
		    {
		      nuc_it->colour = part->colour[4];
		      part->draw[4]=true;
		    }
		  else
		    {
		      nuc_it->colour = part->colour[5];
		      part->draw[5]=true;
		    }
		}
	      else if (nuc_it->st == 0 && (nuc_it+1)->st != 1)
		{
		  //-As not every nucleus has an isomer, draw empty boxes as a visual aid
		  nuc_it->colour = "white";
		  part->draw[6]=true;
		  nuc_it->show=2;
		  /*
		  if (nuc_it->decay=="stable")
		    colour="black";

		  if (draw->file_type == 0)
		    {
		      out_file << "%-" << nuc_it->A << nuc_it->symbol << "-\n"
			       << "0";

		      if (draw->write_isotope)
			{
			  //-If the square is coloured black, change text colour to white
			  if (colour == "black")
			    {
			      //-If the square is black and marked as an own nuclei,
			      //-change text colour to red
			      if (nuc_it->own == 1)
				out_file << " red";
			      else
				out_file << " white";
			    }
			  else
			    out_file << " black";

			  out_file << " (" << nuc_it->symbol << ") (" << nuc_it->A << ")";
			}

		      out_file << " " << colour
			       << " " << nuc_it->N-draw->Nmin
			       << " " << nuc_it->Z-draw->Zmin
			       << " curve n" << std::endl;
		    }
		  else if (draw->file_type == 1)
		    {
		    }
		  else if (draw->file_type == 2)
		    {
		    }
		  */
		}
	    }
	  /*
	    std::cout << "==========================================\n"
	    << "symbol  	  = " << nuc_it->symbol     << "\n"
	    << "A       	  = " << nuc_it->A          << "\n"
	    << "Z       	  = " << nuc_it->Z          << "\n"
	    << "N       	  = " << nuc_it->N          << "\n"
	    << "exp     	  = " << nuc_it->exp        << "\n"
	    << "decay   	  = " << nuc_it->decay      << "\n"
	    << "st      	  = " << nuc_it->st         << "\n"
	    << "AME_ME        = " << nuc_it->AME_ME     << "\n"
	    << "AME_dME    = " << nuc_it->AME_dME    << "\n"
	    << "NUBASE_ME  = " << nuc_it->NUBASE_ME  << "\n"
	    << "NUBASE_dME = " << nuc_it->NUBASE_dME << "\n"
	    << "is_nrg  	  = " << nuc_it->is_nrg     << "\n"
	    << "dis_nrg 	  = " << nuc_it->dis_nrg    << "\n"
	    << "J       	  = " << nuc_it->J          << "\n"
	    << "J_exp   	  = " << nuc_it->J_exp      << "\n"
	    << "J_tent  	  = " << nuc_it->J_tent     << "\n"
	    << "pi      	  = " << nuc_it->pi         << "\n"
	    << "pi_exp  	  = " << nuc_it->pi_exp     << "\n"
	    << "hl      	  = " << nuc_it->hl         << "\n"
	    << "rich    	  = " << nuc_it->rich       << "\n"
	    << "own     	  = " << nuc_it->own        << "\n"
	    << "==========================================\n" << std::endl;
	  *///exit(-1);
	  //-Write the postscript code to create a black box outline
	  //-with a filled square coloured as required
	  /*
	  if (flag)
	    {
	      if (draw->file_type == 0)
		{
		  out_file << "%-" << nuc_it->A << nuc_it->symbol << "-\n";
		  //-If user specified nuclei are to be drawn,
		  //-mark with a half square along the diagonal
		  if (nuc_it->own)
		    out_file << "8";
		  else
		    out_file << "0";

		  if (draw->choice=="e"
		      && (nuc_it-1)->N==nuc_it->N
		      && (nuc_it-1)->Z==nuc_it->Z
		      && (nuc_it-1)->st==0
		      && (nuc_it-1)->decay=="stable")
		    out_file << "1";

		  if (draw->write_isotope)
		    {
		      //-If the square is coloured black, change text colour to white
		      if (colour == "black")
			{
			  //-If the square is black and marked as an own nuclei,
			  //-change text colour to red
			  if (nuc_it->own == 1)
			    out_file << " red";
			  else
			    out_file << " white";
			}
		      else if (draw->choice=="e"
			       && (nuc_it-1)->N==nuc_it->N
			       && (nuc_it-1)->Z==nuc_it->Z
			       && (nuc_it-1)->st==0
			       && (nuc_it-1)->decay=="stable")
			out_file << " white";
		      else
			out_file << " black";

		      out_file << " (" << nuc_it->symbol << ") (" << nuc_it->A << ")";
		    }

		  out_file << " " << colour << " " << nuc_it->N-draw->Nmin << " " << nuc_it->Z-draw->Zmin << " curve n" << std::endl;
		}
	      else if (draw->file_type == 1)
		{
		  out_file << "<!--" << nuc_it->A << nuc_it->symbol << "-->\n";

		  out_file << "<g transform=\"translate(" << nuc_it->N-draw->Nmin << " " << draw->Zmax-nuc_it->Z << ")\"> "
			   << "<use xlink:href=\"#" << colour << "Nucleus\"/></g>" << std::endl;
		  //<< "<text class=\"MidSymbol Black\" dx=\"0.5\" dy=\"0.80\">" << nuc_it->symbol << "</text> "
		  //<< "<text class=\"MidNumber Black\" dx=\"0.5\" dy=\"0.35\">" << nuc_it->A << "</text></g>" << std::endl;
		}
	      else if (draw->file_type == 2)
		{
		  out_file << "%" << nuc_it->A << nuc_it->symbol << "\n";

		  out_file << "\\nucleus{" << colour << "}{"
			   << nuc_it->N << "}{"
			   << nuc_it->Z << "}{"
			   << nuc_it->A << "}{"
			   << nuc_it->symbol << "}" << std::endl;
		}
	    }
	  */
	}
    }
}
