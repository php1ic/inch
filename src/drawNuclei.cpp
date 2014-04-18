#include "include/functions.h"

void drawNuclei(std::vector<Nuclide> &in,
		const inputs *draw,
		std::ostream &out_file)
{
  std::vector<Nuclide>::iterator nuc_it;

  for (nuc_it=in.begin(); nuc_it!=in.end(); ++nuc_it)
    {
      if (nuc_it->show == 1)
	{
	  if (draw->file_type == 0)
	    {
	      out_file << "%" << nuc_it->A << nuc_it->symbol << "\n";
	      //-If user specified nuclei are to be drawn,
	      //-mark with a half square along the diagonal
	      if (nuc_it->own)
		out_file << "8";
	      else
		out_file << "0";

	      if (draw->choice=="e"
		  && nuc_it!=in.begin()
		  && (nuc_it-1)->N==nuc_it->N
		  && (nuc_it-1)->Z==nuc_it->Z
		  && (nuc_it-1)->st==0
		  && (nuc_it-1)->decay=="stable")
		out_file << "1";

	      if (draw->write_isotope)
		{
		  //-If the square is coloured black, change text colour to white
		  if (nuc_it->colour == "black")
		    {
		      //-If the square is black and marked as an own nuclei,
		      //-change text colour to red
		      if (nuc_it->own == 1)
			out_file << " red";
		      else
			out_file << " white";
		    }
		  else if (draw->choice=="e"
			   && nuc_it!=in.begin()
			   && (nuc_it-1)->N==nuc_it->N
			   && (nuc_it-1)->Z==nuc_it->Z
			   && (nuc_it-1)->st==0
			   && (nuc_it-1)->decay=="stable")
		    out_file << " white";
		  else
		    out_file << " black";

		  out_file << " (" << nuc_it->symbol << ") (" << nuc_it->A << ")";
		}

	      out_file << " " << nuc_it->colour << " " << nuc_it->N-draw->Nmin << " " << nuc_it->Z-draw->Zmin << " curve Nucleus" << std::endl;
	    }
	  else if (draw->file_type == 1)
	    {
	      out_file << "<!--" << nuc_it->A << nuc_it->symbol << "-->\n";

	      out_file << "<g transform=\"translate(" << nuc_it->N-draw->Nmin << " " << draw->Zmax-nuc_it->Z << ")\"> "
		       << "<use xlink:href=\"#" << nuc_it->colour << "Nucleus\"/></g>" << std::endl;
	      //<< "<text class=\"MidSymbol Black\" dx=\"0.5\" dy=\"0.80\">" << nuc_it->symbol << "</text> "
	      //<< "<text class=\"MidNumber Black\" dx=\"0.5\" dy=\"0.35\">" << nuc_it->A << "</text></g>" << std::endl;
	    }
	  else if (draw->file_type == 2)
	    {
	      out_file << "%" << nuc_it->A << nuc_it->symbol << "\n";

	      out_file << "\\nucleus{" << nuc_it->colour << "}{"
		       << nuc_it->N << "}{"
		       << nuc_it->Z << "}{"
		       << nuc_it->A << "}{"
		       << nuc_it->symbol << "}" << std::endl;
	    }
	}
      else if (nuc_it->show == 2)
	{
	  if (nuc_it->decay=="stable")
	    nuc_it->colour="black";

	  if (draw->file_type == 0)
	    {
	      out_file << "%-" << nuc_it->A << nuc_it->symbol << "-\n"
		       << "0";

	      if (draw->write_isotope)
		{
		  //-If the square is coloured black, change text colour to white
		  if (nuc_it->colour == "black")
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

	      out_file << " " << nuc_it->colour
		       << " " << nuc_it->N-draw->Nmin
		       << " " << nuc_it->Z-draw->Zmin
		       << " curve Nucleus" << std::endl;
	    }
	  else if (draw->file_type == 1)
	    {
	    }
	  else if (draw->file_type == 2)
	    {
	    }
	}
    }
}
