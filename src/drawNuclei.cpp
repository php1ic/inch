#include "functions.h"

void drawNuclei(const std::vector<Nuclide> &in,
		const std::vector<std::string> &partition_colour,
		const std::vector<float> &partition_value,
		std::vector<bool> &draw_partition,
		const inputs *draw,
		std::ofstream &out_file
		)
{
  std::vector<Nuclide>::const_iterator nuc_it;
  bool flag=false;
  std::string colour;

  //-Using the region specified set the colour of each isotope and draw it
  for (nuc_it=in.begin(); nuc_it!=in.end(); ++nuc_it)
    {
      if (    nuc_it->Z    >= draw->Zmin
	  &&  nuc_it->Z    <= draw->Zmax
	  &&  nuc_it->N    >= draw->Nmin
	  &&  nuc_it->N    <= draw->Nmax
	  &&  nuc_it->exp  != draw->experimental
	  &&  nuc_it->rich  % draw->np_rich == 0
	  )
	{
	  flag=false;
	  //-Error on mass excess units of keV
	  if (draw->choice == "a")
	    {
	      if (nuc_it->st == 0)
		{
		  flag=true;

		  int me;

		  if (draw->AME)
		    me = nuc_it->AME_dME;
		  else
		    me = nuc_it->NUBASE_dME;

		  if (nuc_it->decay == "stable" && me <= partition_value[0])
		    {
		      colour = partition_colour[0];
		      draw_partition[0]=true;
		    }
		  else if (nuc_it->decay == "stable" && me > partition_value[0])
		    {
		      colour = partition_colour[1];
		      draw_partition[1]=true;
		    }
		  else if (me <= partition_value[0])
		    {
		      colour = partition_colour[2];
		      draw_partition[2]=true;
		    }
		  else if (me > partition_value[0] && me <= partition_value[1])
		    {
		      colour = partition_colour[3];
		      draw_partition[3]=true;
		    }
		  else if (me > partition_value[1] && me <= partition_value[2])
		    {
		      colour = partition_colour[4];
		      draw_partition[4]=true;
		    }
		  else if (me > partition_value[2] && me <= partition_value[3])
		    {
		      colour = partition_colour[5];
		      draw_partition[5]=true;
		    }
		  else if (me > partition_value[3] && me <= partition_value[4])
		    {
		      colour = partition_colour[6];
		      draw_partition[6]=true;
		    }
		  else
		    {
		      colour = partition_colour[7];
		      draw_partition[7]=true;
		    }
		}
	      else
		flag=false;
	    }
	  //-Relative error on mass excess units of keV
	  else if (draw->choice == "b")
	    {
	      if (nuc_it->st == 0)
		{
		  flag=true;

		  float dme;

		  if (draw->AME)
		    dme = fabs(nuc_it->AME_dME/nuc_it->AME_ME);
		  else
		    dme = fabs(nuc_it->NUBASE_dME/nuc_it->NUBASE_ME);

		  if (dme <= partition_value[0])
		    {
		      colour = partition_colour[0];
		      draw_partition[0]=true;
		    }
		  else if (dme > partition_value[0] && dme <= partition_value[1])
		    {
		      colour = partition_colour[1];
		      draw_partition[1]=true;
		    }
		  else if (dme > partition_value[1] && dme <= partition_value[2])
		    {
		      colour = partition_colour[2];
		      draw_partition[2]=true;
		    }
		  else if (dme > partition_value[2] && dme <= partition_value[3])
		    {
		      colour = partition_colour[3];
		      draw_partition[3]=true;
		    }
		  else if (dme > partition_value[3] && dme <= partition_value[4])
		    {
		      colour = partition_colour[4];
		      draw_partition[4]=true;
		    }
		  else if (dme > partition_value[4] || dme == 0)
		    {
		      colour = partition_colour[5];
		      draw_partition[5]=true;
		    }
		}
	      else
		flag=false;
	    }
	  //-Major ground-state decay mode
	  else if (draw->choice == "c")
	    {
	      if (nuc_it->st == 0)
		{
		  flag=true;

		  if (nuc_it->decay == "stable")
		    {
		      colour = partition_colour[0];
		      draw_partition[0]=true;
		    }
		  else if (nuc_it->decay == "A")
		    {
		      colour = partition_colour[1];
		      draw_partition[1]=true;
		    }
		  else if (nuc_it->decay == "B-")
		    {
		      colour = partition_colour[2];
		      draw_partition[2]=true;
		    }
		  else if (nuc_it->decay == "B+")
		    {
		      colour = partition_colour[3];
		      draw_partition[3]=true;
		    }
		  else if (nuc_it->decay == "SF")
		    {
		      colour = partition_colour[4];
		      draw_partition[4]=true;
		    }
		  else if (nuc_it->decay == "n")
		    {
		      colour = partition_colour[5];
		      draw_partition[5]=true;
		    }
		  else if (nuc_it->decay == "2n")
		    {
		      colour = partition_colour[6];
		      draw_partition[6]=true;
		    }
		  else if (nuc_it->decay == "p")
		    {
		      colour = partition_colour[7];
		      draw_partition[7]=true;
		    }
		  else if (nuc_it->decay == "2p")
		    {
		      colour = partition_colour[8];
		      draw_partition[8]=true;
		    }
		  else if (nuc_it->decay == "unknown")
		    {
		      colour = partition_colour[9];
		      draw_partition[9]=true;
		    }
		  else if (nuc_it->decay == "EC")
		    {
		      colour = partition_colour[10];
		      draw_partition[10]=true;
		    }
		}
	      else
		flag=false;
	    }
	  //-Half-life of ground-state
	  else if (draw->choice == "d")
	    {
	      if (nuc_it->st == 0)
		{
		  flag=true;

		  if (nuc_it->hl <= partition_value[0])
		    {
		      colour = partition_colour[0];
		      draw_partition[0]=true;
		    }
		  else if (nuc_it->hl > partition_value[0] && nuc_it->hl <= partition_value[1])
		    {
		      colour = partition_colour[1];
		      draw_partition[1]=true;
		    }
		  else if (nuc_it->hl > partition_value[1] && nuc_it->hl <= partition_value[2])
		    {
		      colour = partition_colour[2];
		      draw_partition[2]=true;
		    }
		  else if (nuc_it->hl > partition_value[2] && nuc_it->hl <= partition_value[3])
		    {
		      colour = partition_colour[3];
		      draw_partition[3]=true;
		    }
		  else if (nuc_it->hl > partition_value[3] && nuc_it->hl <= partition_value[4])
		    {
		      colour = partition_colour[4];
		      draw_partition[4]=true;
		    }
		  else if (nuc_it->hl > partition_value[4] && nuc_it->hl <= partition_value[5])
		    {
		      colour = partition_colour[5];
		      draw_partition[5]=true;
		    }
		  else if (nuc_it->hl > partition_value[5] && nuc_it->hl <= partition_value[6])
		    {
		      colour = partition_colour[6];
		      draw_partition[6]=true;
		    }
		  else
		    {
		      colour = partition_colour[7];
		      draw_partition[7]=true;
		    }
		}
	      else
		flag=false;
	    }
	  //-1st isomer energy
	  else if (draw->choice == "e")
	    {
	      if (nuc_it->st == 1)
		{
		  flag=true;

		  if (nuc_it->is_nrg <= partition_value[0])
		    {
		      colour = partition_colour[0];
		      draw_partition[0]=true;
		    }
		  else if (nuc_it->is_nrg > partition_value[0] && nuc_it->is_nrg <= partition_value[1])
		    {
		      colour = partition_colour[1];
		      draw_partition[1]=true;
		    }
		  else if (nuc_it->is_nrg > partition_value[1] && nuc_it->is_nrg <= partition_value[2])
		    {
		      colour = partition_colour[2];
		      draw_partition[2]=true;
		    }
		  else if (nuc_it->is_nrg > partition_value[2] && nuc_it->is_nrg <= partition_value[3])
		    {
		      colour = partition_colour[3];
		      draw_partition[3]=true;
		    }
		  else if (nuc_it->is_nrg > partition_value[3] && nuc_it->is_nrg <= partition_value[4])
		    {
		      colour = partition_colour[4];
		      draw_partition[4]=true;
		    }
		  else
		    {
		      colour = partition_colour[5];
		      draw_partition[5]=true;
		    }
		}
	      else if (nuc_it->st == 0 && (nuc_it+1)->st != 1)
		{
		  //-As not every nucleus has an isomer, draw empty boxes as a visual aid
		  //partition_colour[6] = "white";
		  //colour = partition_colour[6];
		  colour = "white";
		  draw_partition[6]=true;

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
	}
    }
}
