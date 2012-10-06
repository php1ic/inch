#include "functions.h"

void drawNuclei(std::vector<Nuclide> &in,
		 std::vector<std::string> &kcol,
		 std::vector<float> &n,
		 std::vector<bool> &k,
		 inputs *draw,
		 //File is already opened in write_[EPS,SVG]() so pass open stream in rather than opening the file again.
		 std::ofstream &out_file
		 )
{
  std::vector<Nuclide>::iterator nuc_it;
  bool flag=0;
  std::string colour;

  //-Using the region specified set the colour of each isotope and draw it
  for (nuc_it=in.begin(); nuc_it!=in.end(); nuc_it++)
    {
      if (    nuc_it->Z    >= draw->Zmin
	  &&  nuc_it->Z    <= draw->Zmax
	  &&  nuc_it->N    >= draw->Nmin
	  &&  nuc_it->N    <= draw->Nmax
	  &&  nuc_it->exp  != draw->experimental
	  &&  nuc_it->rich  % draw->np_rich == 0
	  )
	{
	  flag = 0;
	  //-Error on mass excess units of keV
	  if (draw->choice == "a")
	    {
	      if (nuc_it->st == 0)
		{
		  flag = 1;

		  int me;

		  if (draw->AME)
		    me = nuc_it->AME_dME;
		  else
		    me = nuc_it->NUBASE_dME;

		  if (nuc_it->decay == "stable" && me <= n[0])
		    {
		      colour = kcol.at(0);
		      k[0]=1;
		    }
		  else if (nuc_it->decay == "stable" && me > n[0])
		    {
		      colour = kcol.at(1);
		      k[1]=1;
		    }
		  else if (me <= n[0])
		    {
		      colour = kcol.at(2);
		      k[2]=1;
		    }
		  else if (me > n[0] && me <= n[1])
		    {
		      colour = kcol.at(3);
		      k[3]=1;
		    }
		  else if (me > n[1] && me <= n[2])
		    {
		      colour = kcol.at(4);
		      k[4]=1;
		    }
		  else if (me > n[2] && me <= n[3])
		    {
		      colour = kcol.at(5);
		      k[5]=1;
		    }
		  else if (me > n[3] && me <= n[4])
		    {
		      colour = kcol.at(6);
		      k[6]=1;
		    }
		  else
		    {
		      colour = kcol.at(7);
		      k[7]=1;
		    }
		}
	      else
		flag = 0;
	    }
	  //-Relative error on mass excess units of keV
	  else if (draw->choice == "b")
	    {
	      if (nuc_it->st == 0)
		{
		  flag = 1;

		  float dme;

		  if (draw->AME)
		    dme = fabs(nuc_it->AME_dME/nuc_it->AME_ME);
		  else
		    dme = fabs(nuc_it->NUBASE_dME/nuc_it->NUBASE_ME);

		  if (dme <= n[0])
		    {
		      colour = kcol.at(0);
		      k[0]=1;
		    }
		  else if (dme > n[0] && dme <= n[1])
		    {
		      colour = kcol.at(1);
		      k[1]=1;
		    }
		  else if (dme > n[1] && dme <= n[2])
		    {
		      colour = kcol.at(2);
		      k[2]=1;
		    }
		  else if (dme > n[2] && dme <= n[3])
		    {
		      colour = kcol.at(3);
		      k[3]=1;
		    }
		  else if (dme > n[3] && dme <= n[4])
		    {
		      colour = kcol.at(4);
		      k[4]=1;
		    }
		  else if (dme > n[4] || dme == 0)
		    {
		      colour = kcol.at(5);
		      k[5]=1;
		    }
		}
	      else
		flag = 0;
	    }
	  //-Major ground-state decay mode
	  else if (draw->choice == "c")
	    {
	      if (nuc_it->st == 0)
		{
		  flag = 1;

		  if (nuc_it->decay == "stable")
		    {
		      colour = kcol.at(0);
		      k[0]=1;
		    }
		  else if (nuc_it->decay == "A")
		    {
		      colour = kcol.at(1);
		      k[1]=1;
		    }
		  else if (nuc_it->decay == "B-")
		    {
		      colour = kcol.at(2);
		      k[2]=1;
		    }
		  else if (nuc_it->decay == "B+")
		    {
		      colour = kcol.at(3);
		      k[3]=1;
		    }
		  else if (nuc_it->decay == "SF")
		    {
		      colour = kcol.at(4);
		      k[4]=1;
		    }
		  else if (nuc_it->decay == "n")
		    {
		      colour = kcol.at(5);
		      k[5]=1;
		    }
		  else if (nuc_it->decay == "2n")
		    {
		      colour = kcol.at(6);
		      k[6]=1;
		    }
		  else if (nuc_it->decay == "p")
		    {
		      colour = kcol.at(7);
		      k[7]=1;
		    }
		  else if (nuc_it->decay == "2p")
		    {
		      colour = kcol.at(8);
		      k[8]=1;
		    }
		  else if (nuc_it->decay == "unknown")
		    {
		      colour = kcol.at(9);
		      k[9]=1;
		    }
		  else if (nuc_it->decay == "EC")
		    {
		      colour = kcol.at(10);
		      k[10]=1;
		    }
		}
	      else
		flag = 0;
	    }
	  //-Half-life of ground-state
	  else if (draw->choice == "d")
	    {
	      if (nuc_it->st == 0)
		{
		  flag = 1;

		  if (nuc_it->hl <= n[0])
		    {
		      colour = kcol.at(0);
		      k[0]=1;
		    }
		  else if (nuc_it->hl > n[0] && nuc_it->hl <= n[1])
		    {
		      colour = kcol.at(1);
		      k[1]=1;
		    }
		  else if (nuc_it->hl > n[1] && nuc_it->hl <= n[2])
		    {
		      colour = kcol.at(2);
		      k[2]=1;
		    }
		  else if (nuc_it->hl > n[2] && nuc_it->hl <= n[3])
		    {
		      colour = kcol.at(3);
		      k[3]=1;
		    }
		  else if (nuc_it->hl > n[3] && nuc_it->hl <= n[4])
		    {
		      colour = kcol.at(4);
		      k[4]=1;
		    }
		  else if (nuc_it->hl > n[4] && nuc_it->hl <= n[5])
		    {
		      colour = kcol.at(5);
		      k[5]=1;
		    }
		  else if (nuc_it->hl > n[5] && nuc_it->hl <= n[6])
		    {
		      colour = kcol.at(6);
		      k[6]=1;
		    }
		  else
		    {
		      colour = kcol.at(7);
		      k[7]=1;
		    }
		}
	      else
		flag = 0;
	    }
	  //-1st isomer energy
	  else if (draw->choice == "e")
	    {
	      if (nuc_it->st == 1)
		{
		  flag = 1;

		  if (nuc_it->is_nrg <= n[0])
		    {
		      colour = kcol.at(0);
		      k[0]=1;
		    }
		  else if (nuc_it->is_nrg > n[0] && nuc_it->is_nrg <= n[1])
		    {
		      colour = kcol.at(1);
		      k[1]=1;
		    }
		  else if (nuc_it->is_nrg > n[1] && nuc_it->is_nrg <= n[2])
		    {
		      colour = kcol.at(2);
		      k[2]=1;
		    }
		  else if (nuc_it->is_nrg > n[2] && nuc_it->is_nrg <= n[3])
		    {
		      colour = kcol.at(3);
		      k[3]=1;
		    }
		  else if (nuc_it->is_nrg > n[3] && nuc_it->is_nrg <= n[4])
		    {
		      colour = kcol.at(4);
		      k[4]=1;
		    }
		  else
		    {
		      colour = kcol.at(5);
		      k[5]=1;
		    }
		}
	      else if (nuc_it->st == 0 && (nuc_it+1)->st != 1)
		{
		  //-As not every nucleus has an isomer, draw empty boxes as a visual aid
		  kcol.at(6) = "white";
		  colour = kcol.at(6);
		  k[6]=1;

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
		  if (nuc_it->own == 1)
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

		  out_file << " " << colour
			   << " " << nuc_it->N-draw->Nmin
			   << " " << nuc_it->Z-draw->Zmin
			   << " curve n" << std::endl;
		}
	      else if (draw->file_type == 1)
		{
		  out_file << "<!--" << nuc_it->A << nuc_it->symbol << "-->\n";

		  out_file << "<g transform=\"translate(" << nuc_it->N-draw->Nmin << " " << draw->Zmax-nuc_it->Z << ")\"> "
			   << "<use xlink:href=\"#" << colour << "Nucleus\"/></g>" << std::endl;
		  //<< "<text class=\"MidSymbol Black\" dx=\"0.5\" dy=\"0.80\">" << nuc_it->symbol << "</text> "
		  //<< "<text class=\"MidNumber Black\" dx=\"0.5\" dy=\"0.35\">" << nuc_it->A << "</text></g>" << std::endl;
		}
	      else if (draw->file_type ==2)
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
