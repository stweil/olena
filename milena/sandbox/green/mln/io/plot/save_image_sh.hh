// Copyright (C) 2007 EPITA Research and Development Laboratory (LRDE)
// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_IO_PLOT_SAVE_IMAGE_SH_HH
#define MLN_IO_PLOT_SAVE_IMAGE_SH_HH

/// \file
///
/// \brief Define some functions to export to gnuplot format as a script shell.
///
/// Theses routines are dedicated to image visualization. The aim is to display
/// image whatever the value used as pixel. The behaviour of gnuplot is more 
/// like xv than imageMagick. A Gnuplot script shell file is a text dump file
/// with a preambule to let gnuplot interpret data. As a script shell, you can
/// launch it (don't forget the permissions), and every thing is packed in it.
/// The script file call gnuplot in batch mode, the result window persists and
/// that's all.
/// 

#include <fstream>
#include <string>

#include <mln/trace/entering.hh>
#include <mln/trace/exiting.hh>
#include <mln/core/macros.hh>
#include <mln/core/contract.hh>
#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/geom/min_row.hh>
#include <mln/geom/max_row.hh>
#include <mln/geom/min_col.hh>
#include <mln/geom/max_col.hh>
#include <mln/value/int_u.hh>
#include <mln/value/int_s.hh>
#include <mln/value/rgb.hh>
#include <mln/value/hsl.hh>
#include <mln/value/hsi.hh>


namespace mln
{
  
  namespace io
  {
    
    namespace plot
    {

      /// \brief Save an image as a gnuplot script shell.
      ///
      /// Every thing is save. The image could be 1d, 2d or 3d. The value of 
      /// the pixels could be int_u<n>, int_s<n>, float, double, hsl_f, hsl_d,
      /// hsi_f, hsi_d and rgb<n>.
      ///
      /// \param[in] img      the image which contains the data to save.
      /// \param[in] filename the name of the unix script shell.
      /// \return             the status of the opening file operation.
      ///
      /// The result depends on the permission to save the file with
      /// filename parameter as unix path. The script shell file must have the 
      /// permission to execute (chmod 755). Launch the script shell to call 
      /// gnuplot in batchmode with fine parameters.
   
      template <typename I>
      bool save_image_sh(const Image<I>& img, const std::string& filename);

    } // end of namespace mln::accu::stat
  
  } // end of namespace mln::accu


  namespace io
  {
    
    namespace plot
    {
      
#ifndef MLN_INCLUDE_ONLY


      //------------------------------------------------------------------------
      // save_image_sh(const image2d<bool>&, const std::string&)
      //------------------------------------------------------------------------

      inline
      bool save_image_sh_dispatch_overload(const image2d< bool >& img,
					   const std::string&     filename)
      {
	trace::entering("mln::io::plot::save_image_sh<image2d<bool>>");
	mln_precondition(img.is_valid());

	std::ofstream           out(filename.c_str());
	bool                    result  = !out.fail();
	unsigned                min_row = geom::min_row(img);
	unsigned                max_row = geom::max_row(img);
	unsigned                min_col = geom::min_col(img);
	unsigned                max_col = geom::max_col(img);

	if (result)
	{
	  mln_piter_(image2d< bool >) p(img.domain());

	  // Output data prelude (terminal X11, image).
	  out << "#!/bin/sh"                                       << std::endl;
	  out << "####################################"            << std::endl;
	  out << "# Columns = (x, y, bool)           #"            << std::endl;
	  out << "####################################"            << std::endl;
	  out                                                      << std::endl;
	  out << "gnuplot <<EOF"                                   << std::endl;
	  out << "set terminal x11 persist 1"                      << std::endl;
	  out << "set palette gray"                                << std::endl;
	  out << "set xrange ["                                    << min_col;
	  out << ":"                                               << max_col;
	  out << "]"                                               << std::endl;
	  out << "set yrange [-"                                   << max_row;
	  out << ":"                                               << min_row;
	  out << "]"                                               << std::endl;
	  out << "plot '-' using (\\$2):(-\\$1):(\\$3) with image" << std::endl;
	  
	  // Output data.
	  for_all(p)
	  {
	    out << p.row()   << " ";
	    out << p.col()   << " ";
	    out << img(p)    << std::endl;
	  }
	  
	  // Close gnuplot data stream.
	  out << "e"                                               << std::endl;
	  out << "EOF"                                             << std::endl;

	  out.close();
	}
	else
	{
	  std::cerr << "ERROR[mln::io::plot::save_image_sh]:"      << filename;
	  std::cerr << " couldn't be opened !!"                    << std::endl;
	}

	trace::exiting("mln::io::plot::save_image_sh<image2d<bool>>");
	return result;
      }

      //------------------------------------------------------------------------
      // save_image_sh(const image2d<float>&, const std::string&)
      //------------------------------------------------------------------------
      
      inline
      bool save_image_sh_dispatch_overload(const image2d<float>& img,
					   const std::string&    filename)
      {
	trace::entering("mln::io::plot::save_image_sh<image2d<float>>");
	mln_precondition(img.is_valid());

	std::ofstream           out(filename.c_str());
	bool                    result  = !out.fail();
	unsigned                min_row = geom::min_row(img);
	unsigned                max_row = geom::max_row(img);
	unsigned                min_col = geom::min_col(img);
	unsigned                max_col = geom::max_col(img);

	if (result)
	{
	  mln_piter_(image2d< float >) p(img.domain());

	  // Output data prelude (terminal X11, image).
	  out << "#!/bin/sh"                                       << std::endl;
	  out << "####################################"            << std::endl;
	  out << "# Columns = (x, y, float)          #"            << std::endl;
	  out << "####################################"            << std::endl;
	  out                                                      << std::endl;
	  out << "gnuplot <<EOF"                                   << std::endl;
	  out << "set terminal x11 persist 1"                      << std::endl;
	  out << "set palette gray"                                << std::endl;
	  out << "set xrange ["                                    << min_col;
	  out << ":"                                               << max_col;
	  out << "]"                                               << std::endl;
	  out << "set yrange [-"                                   << max_row;
	  out << ":"                                               << min_row;
	  out << "]"                                               << std::endl;
	  out << "plot '-' using (\\$2):(-\\$1):(\\$3) with image" << std::endl;
	  
	  // Output data.
	  for_all(p)
	  {
	    out << p.row()   << " ";
	    out << p.col()   << " ";
	    out << img(p)    << std::endl;
	  }
	  
	  // Close gnuplot data stream.
	  out << "e"                                               << std::endl;
	  out << "EOF"                                             << std::endl;

	  out.close();
	}
	else
	{
	  std::cerr << "ERROR[mln::io::plot::save_image_sh]:"      << filename;
	  std::cerr << " couldn't be opened !!"                    << std::endl;
	}

	trace::exiting("mln::io::plot::save_image_sh<image2d<float>>");
	return result;
      }

      //------------------------------------------------------------------------
      // save_image_sh(const image2d<double>&, const std::string&)
      //------------------------------------------------------------------------

      inline
      bool save_image_sh_dispatch_overload(const image2d<double>& img,
					   const std::string&     filename)
      {
	trace::entering("mln::io::plot::save_image_sh<image2d<double>>");
	mln_precondition(img.is_valid());

	std::ofstream           out(filename.c_str());
	bool                    result  = !out.fail();
	unsigned                min_row = geom::min_row(img);
	unsigned                max_row = geom::max_row(img);
	unsigned                min_col = geom::min_col(img);
	unsigned                max_col = geom::max_col(img);

	if (result)
	{
	  mln_piter_(image2d< double >) p(img.domain());

	  // Output data prelude (terminal X11, image).
	  out << "#!/bin/sh"                                       << std::endl;
	  out << "####################################"            << std::endl;
	  out << "# Columns = (x, y, double)         #"            << std::endl;
	  out << "####################################"            << std::endl;
	  out                                                      << std::endl;
	  out << "gnuplot <<EOF"                                   << std::endl;
	  out << "set terminal x11 persist 1"                      << std::endl;
	  out << "set palette gray"                                << std::endl;
	  out << "set xrange ["                                    << min_col;
	  out << ":"                                               << max_col;
	  out << "]"                                               << std::endl;
	  out << "set yrange [-"                                   << max_row;
	  out << ":"                                               << min_row;
	  out << "]"                                               << std::endl;
	  out << "plot '-' using (\\$2):(-\\$1):(\\$3) with image" << std::endl;
	  
	  // Output data.
	  for_all(p)
	  {
	    out << p.row()   << " ";
	    out << p.col()   << " ";
	    out << img(p)    << std::endl;
	  }
	  
	  // Close gnuplot data stream.
	  out << "e"                                               << std::endl;
	  out << "EOF"                                             << std::endl;

	  out.close();
	}
	else
	{
	  std::cerr << "ERROR[mln::io::plot::save_image_sh]:"      << filename;
	  std::cerr << " couldn't be opened !!"                    << std::endl;
	}

	trace::exiting("mln::io::plot::save_image_sh<image2d<double>>");
	return result;
      }

      //------------------------------------------------------------------------
      // save_image_sh(const image2d<int_u<n>>&, const std::string&)
      //------------------------------------------------------------------------

      template <unsigned n>
      inline
      bool save_image_sh_dispatch_overload(const image2d<value::int_u<n> >& img,
					   const std::string&          filename)
      {
	trace::entering("mln::io::plot::save_image_sh<image2d<int_u<n>>");
	mln_precondition(img.is_valid());

	std::ofstream           out(filename.c_str());
	bool                    result  = !out.fail();
	unsigned                min_row = geom::min_row(img);
	unsigned                max_row = geom::max_row(img);
	unsigned                min_col = geom::min_col(img);
	unsigned                max_col = geom::max_col(img);

	if (result)
	{
	  mln_piter(image2d< value::int_u<n> >) p(img.domain());

	  // Output data prelude (terminal X11, image).
	  out << "#!/bin/sh"                                       << std::endl;
	  out << "####################################"            << std::endl;
	  out << "# Columns = (x, y, int_u<n>)       #"            << std::endl;
	  out << "####################################"            << std::endl;
	  out                                                      << std::endl;
	  out << "gnuplot <<EOF"                                   << std::endl;
	  out << "set terminal x11 persist 1"                      << std::endl;
	  out << "set palette gray"                                << std::endl;
	  out << "set xrange ["                                    << min_col;
	  out << ":"                                               << max_col;
	  out << "]"                                               << std::endl;
	  out << "set yrange [-"                                   << max_row;
	  out << ":"                                               << min_row;
	  out << "]"                                               << std::endl;
	  out << "plot '-' using (\\$2):(-\\$1):(\\$3) with image" << std::endl;
	  
	  // Output data.
	  for_all(p)
	  {
	    out << p.row()   << " ";
	    out << p.col()   << " ";
	    out << img(p)    << std::endl;
	  }
	  
	  // Close gnuplot data stream.
	  out << "e"                                               << std::endl;
	  out << "EOF"                                             << std::endl;

	  out.close();
	}
	else
	{
	  std::cerr << "ERROR[mln::io::plot::save_image_sh]:"      << filename;
	  std::cerr << " couldn't be opened !!"                    << std::endl;
	}

	trace::exiting("mln::io::plot::save_image_sh<image2d<int_u<n>>");
	return result;
      }

      //------------------------------------------------------------------------
      // save_image_sh(const image2d<int_s<n>>&, const std::string&)
      //------------------------------------------------------------------------

      template <unsigned n>
      inline
      bool save_image_sh_dispatch_overload(const image2d<value::int_s<n> >& img,
					   const std::string&          filename)
      {
	trace::entering("mln::io::plot::save_image_sh<image2d<int_s<n>>");
	mln_precondition(img.is_valid());

	std::ofstream           out(filename.c_str());
	bool                    result  = !out.fail();
	unsigned                min_row = geom::min_row(img);
	unsigned                max_row = geom::max_row(img);
	unsigned                min_col = geom::min_col(img);
	unsigned                max_col = geom::max_col(img);

	if (result)
	{
	  mln_piter(image2d< value::int_s<n> >) p(img.domain());

	  // Output data prelude (terminal X11, image).
	  out << "#!/bin/sh"                                       << std::endl;
	  out << "####################################"            << std::endl;
	  out << "# Columns = (x, y, int_s<n>)       #"            << std::endl;
	  out << "####################################"            << std::endl;
	  out                                                      << std::endl;
	  out << "gnuplot <<EOF"                                   << std::endl;
	  out << "set terminal x11 persist 1"                      << std::endl;
	  out << "set palette gray"                                << std::endl;
	  out << "set xrange ["                                    << min_col;
	  out << ":"                                               << max_col;
	  out << "]"                                               << std::endl;
	  out << "set yrange [-"                                   << max_row;
	  out << ":"                                               << min_row;
	  out << "]"                                               << std::endl;
	  out << "plot '-' using 2:(-\\$1):3 with image"           << std::endl;
	  
	  // Output data.
	  for_all(p)
	  {
	    out << p.row()        << " ";
	    out << p.col()        << " ";
	    out << img(p)         << std::endl;
	  }
	  
	  // Close gnuplot data stream.
	  out << "e"                                               << std::endl;
	  out << "EOF"                                             << std::endl;

	  out.close();
	}
	else
	{
	  std::cerr << "ERROR[mln::io::plot::save_image_sh]:"      << filename;
	  std::cerr << " couldn't be opened !!"                    << std::endl;
	}

	trace::exiting("mln::io::plot::save_image_sh<image2d<int_s<n>>");
	return result;
      }

      //------------------------------------------------------------------------
      // save_image_sh(const image2d<rgb<n>>&, const std::string&)
      //------------------------------------------------------------------------

      template <unsigned n>
      inline
      bool save_image_sh_dispatch_overload(const image2d<value::rgb<n> >& img,
					   const std::string&        filename)
      {
	trace::entering("mln::io::plot::save_image_sh<image2d<rgb<n>>");
	mln_precondition(img.is_valid());

	std::ofstream           out(filename.c_str());
	bool                    result  = !out.fail();
	unsigned                min_row = geom::min_row(img);
	unsigned                max_row = geom::max_row(img);
	unsigned                min_col = geom::min_col(img);
	unsigned                max_col = geom::max_col(img);

	if (result)
	{
	  mln_piter(image2d< value::int_u<n> >) p(img.domain());

	  // Output data prelude (terminal X11, image).
	  out << "#!/bin/sh"                                       << std::endl;
	  out << "####################################"            << std::endl;
	  out << "# Columns = (x, y, r, g, b)        #"            << std::endl;
	  out << "####################################"            << std::endl;
	  out                                                      << std::endl;
	  out << "gnuplot <<EOF"                                   << std::endl;
	  out << "set terminal x11 persist 1"                      << std::endl;
	  out << "set xrange ["                                    << min_col;
	  out << ":"                                               << max_col;
	  out << "]"                                               << std::endl;
	  out << "set yrange [-"                                   << max_row;
	  out << ":"                                               << min_row;
	  out << "]"                                               << std::endl;
	  out << "plot '-' using 2:(-\\$1):3:4:5 with rgbimage"    << std::endl;
	  
	  // Output data.
	  for_all(p)
	  {
	    out << p.row()        << " ";
	    out << p.col()        << " ";
	    out << img(p).red()   << " ";
	    out << img(p).green() << " ";
	    out << img(p).blue()  << std::endl;
	  }
	  
	  // Close gnuplot data stream.
	  out << "e"                                               << std::endl;
	  out << "EOF"                                             << std::endl;

	  out.close();
	}
	else
	{
	  std::cerr << "ERROR[mln::io::plot::save_image_sh]:"      << filename;
	  std::cerr << " couldn't be opened !!"                    << std::endl;
	}

	trace::exiting("mln::io::plot::save_image_sh<image2d<rgb<n>>");
	return result;
      }

      //------------------------------------------------------------------------
      // save_image_sh(const image2d<hsl_<T,T,T>>&, const std::string&)
      //------------------------------------------------------------------------

      template <typename T>
      inline
      bool save_image_sh(const image2d<mln::value::hsl_<T,T,T> >& img,
			 const std::string&                       filename)
      {
	trace::entering("mln::io::plot::save_image_sh<image2d<hsl>>");
	mln_precondition(img.is_valid());

	std::ofstream           out(filename.c_str());
	bool                    result  = !out.fail();
	unsigned                min_row = geom::min_row(img);
	unsigned                max_row = geom::max_row(img);
	unsigned                min_col = geom::min_col(img);
	unsigned                max_col = geom::max_col(img);

	typedef mln::value::hsl_<T,T,T> hsl;

	if (result)
	{
	  mln_piter(image2d< hsl >) p(img.domain());

	  // Output data prelude (terminal X11, image).
	  out << "#!/bin/sh"                                       << std::endl;
	  out << "####################################"            << std::endl;
	  out << "# Columns = (x, y, val)            #"            << std::endl;
	  out << "####################################"            << std::endl;
	  out                                                      << std::endl;
	  out << "gnuplot <<EOF"                                   << std::endl;
	  out << "q(l,s)     = (l < 0.5)? (l*(1+s)) : (l+s-(l*s))" << std::endl;
	  out << "p(l,s)     = (2.0 * l - q(l,s))"                 << std::endl;
	  out << "n(x)       = (x < 0)?(x+1) : (x > 1)?(x-1) : (x)"<< std::endl;
	  out                                                      << std::endl;
	  out << "c(p,q,t)   = (t < (1.0/6.0))?(p+(q-p)*6.0*t):\\" << std::endl;
	  out << "             (t < (1.0/2.0))?(q) :\\"            << std::endl;
	  out << " (t < (2.0/3.0))?(p+(q-p)*6.0*((2.0/3.0)-t)):\\" << std::endl;
	  out << "             (p)"				   << std::endl;
	  out                                                      << std::endl;
	  out << "r(h,s,l)   = c(p(l,s),q(l,s),n(h/360.0+1.0/3.0))"<< std::endl;
	  out << "g(h,s,l)   = c(p(l,s),q(l,s),n(h/360.0))"        << std::endl;
	  out << "b(h,s,l)   = c(p(l,s),q(l,s),n(h/360.0-1.0/3.0))"<< std::endl;
	  out                                                      << std::endl;
	  out << "set terminal x11 persist 1"                      << std::endl;
	  out << "set palette gray"                                << std::endl;
	  out << "set xrange ["                                    << min_col;
	  out << ":"                                               << max_col;
	  out << "]"                                               << std::endl;
	  out << "set yrange [-"                                   << max_row;
	  out << ":"                                               << min_row;
	  out << "]"                                               << std::endl;
	  out << "plot '-' using 2:(-\\$1):(r(\\$3,\\$4,\\$5)):\\" << std::endl;
	  out << "               (g(\\$3,\\$4,\\$5)):\\"           << std::endl;
	  out << "               (b(\\$3,\\$4,\\$5)) with rgbimage"<< std::endl;
	  
	  // Output data.
	  for_all(p)
	  {
	    out << p.row()      << " ";
	    out << p.col()      << " ";
	    out << img(p).hue() << " ";
	    out << img(p).sat() << " ";
	    out << img(p).lum() << std::endl;
	  }
	  
	  // Close gnuplot data stream.
	  out << "e"                                               << std::endl;
	  out << "EOF"                                             << std::endl;

	  out.close();
	}
	else
	{
	  std::cerr << "ERROR[mln::io::plot::save_image_sh]:"      << filename;
	  std::cerr << " couldn't be opened !!"                    << std::endl;
	}

	trace::exiting("mln::io::plot::save_image_sh<image2d<hsl>>");
	return result;
      }

      //------------------------------------------------------------------------
      // save_image_sh(const image2d<hsi_<T,T,T>>&, const std::string&)
      //------------------------------------------------------------------------

      template <typename T>
      inline
      bool save_image_sh(const image2d<mln::value::hsi_<T,T,T> >& img,
			 const std::string&                       filename)
      {
	trace::entering("mln::io::plot::save_image_sh<2d>vf");
	mln_precondition(img.is_valid());

	std::ofstream           out(filename.c_str());
	bool                    result  = !out.fail();
	unsigned                min_row = geom::min_row(img);
	unsigned                max_row = geom::max_row(img);
	unsigned                min_col = geom::min_col(img);
	unsigned                max_col = geom::max_col(img);

	typedef mln::value::hsi_<T,T,T> hsi;

	if (result)
	{
	  mln_piter(image2d< hsi >) p(img.domain());

	  // Output data prelude (terminal X11, image).
	  out << "#!/bin/sh"                                       << std::endl;
	  out << "####################################"            << std::endl;
	  out << "# Columns = (x, y, val)            #"            << std::endl;
	  out << "####################################"            << std::endl;
	  out                                                      << std::endl;
	  out << "gnuplot <<EOF"                                   << std::endl;
	  out << "teta(h)    = (h/180.0) * pi"                     << std::endl;
	  out << "alpha(s,h) = s*cos(teta(h))"                     << std::endl;
	  out << "beta(s,h)  = s*sin(teta(h))"                     << std::endl;
	  out << "n(x)       = (x < 0)?(x+1) : (x > 1)?(x-1) : (x)"<< std::endl;
	  out                                                      << std::endl;
	  out << "c(p,q,t)   = (t < (1.0/6.0))?(p+(q-p)*6.0*t):\\" << std::endl;
	  out << "             (t < (1.0/2.0))?(q) :\\"            << std::endl;
	  out << " (t < (2.0/3.0))?(p+(q-p)*6.0*((2.0/3.0)-t)):\\" << std::endl;
	  out << "             (p)"				   << std::endl;
	  out                                                      << std::endl;
	  out << "r(h,s,i)   = (sqrt(3.0)/3.0) * i \\"             << std::endl;
	  out << "             + (2.0/(sqrt(6.0))) * beta(s,h)"    << std::endl;
	  out << "g(h,s,i)   = (sqrt(3.0)/3.0) * i \\"             << std::endl;
	  out << "             + (2.0/(sqrt(2.0))) * alpha(s,h) \\"<< std::endl;
	  out << "             - (1.0/(sqrt(6.0))) * beta(s,h)"    << std::endl;
	  out << "b(h,s,i)   = (sqrt(3.0)/3.0) * i \\"             << std::endl;
	  out << "             - (2.0/(sqrt(2.0))) * alpha(s,h) \\"<< std::endl;
	  out << "             - (1.0/(sqrt(6.0))) * beta(s,h)"    << std::endl;
	  out                                                      << std::endl;
	  out << "set terminal x11 persist 1"                      << std::endl;
	  out << "set palette gray"                                << std::endl;
	  out << "set xrange ["                                    << min_col;
	  out << ":"                                               << max_col;
	  out << "]"                                               << std::endl;
	  out << "set yrange [-"                                   << max_row;
	  out << ":"                                               << min_row;
	  out << "]"                                               << std::endl;
	  out << "plot '-' using 2:(-\\$1):(r(\\$3,\\$4,\\$5)):\\" << std::endl;
	  out << "               (g(\\$3,\\$4,\\$5)):\\"           << std::endl;
	  out << "               (b(\\$3,\\$4,\\$5)) with rgbimage"<< std::endl;
	  
	  // Output data.
	  for_all(p)
	  {
	    out << p.row()       << " ";
	    out << p.col()       << " ";
	    out << img(p).hue()  << " ";
	    out << img(p).sat()  << " ";
	    out << img(p).inty() << std::endl;
	  }
	  
	  // Close gnuplot data stream.
	  out << "e"                                               << std::endl;
	  out << "EOF"                                             << std::endl;

	  out.close();
	}
	else
	{
	  std::cerr << "ERROR[mln::io::plot::save_image_sh]:"      << filename;
	  std::cerr << " couldn't be opened !!"                    << std::endl;
	}

	trace::exiting("mln::io::plot::save_image_sh<2d>vf");
	return result;
      }

      //------------------------------------------------------------------------
      // save_image_sh(const image2d<I>&, const std::string&)
      //------------------------------------------------------------------------

      template <typename I>
      inline
      bool save_image_sh_dispatch_overload(const image2d<I>&  img,
					   const std::string& filename)
      {
	trace::entering("mln::io::plot::save_image_sh<image2d<I>>");
	mln_precondition(img.is_valid());

	std::ofstream           out(filename.c_str());
	bool                    result = !out.fail();

	if (result)
	{
	  mln_piter(image2d<I>) p(img.domain());

	  // Output data prelude (terminal X11, image).
	  out << "#!/bin/sh"                                 << std::endl;
	  out << "####################################"      << std::endl;
	  out << "# Columns = (x, y, builtin)        #"      << std::endl;
	  out << "####################################"      << std::endl;
	  out                                                << std::endl;
	  out << "gnuplot <<EOF"                             << std::endl;
	  out << "set terminal x11 persist 1"                << std::endl;
	  out << "set pm3d"                                  << std::endl;
	  out << "set dgrid3d 20,20,2"                       << std::endl;
	  out << "splot '-' with lines"                      << std::endl;
	  
	  // Output data.
	  for_all(p)
	  {
	    out << p.col()   << " ";
	    out << p.row()   << " ";
	    out << img(p)    << std::endl;
	  }
	  
	  // Close gnuplot data stream.
	  out << "e"                                         << std::endl;
	  out << "EOF"                                       << std::endl;

	  out.close();
	}
	else
	{
	  std::cerr << "ERROR[mln::io::plot::save_image_sh]:"<< filename;
	  std::cerr << " couldn't be opened !!"              << std::endl;
	}
	
	trace::exiting("mln::io::plot::save_image_sh<image2d<I>>");
	return result;
      }

      //------------------------------------------------------------------------
      // save_image_sh(const image1d<I>&, const std::string&)
      //------------------------------------------------------------------------

      template <typename I>
      inline
      bool save_image_sh_dispatch_overload(const image1d<I>&  img,
					   const std::string& filename)
      {
	trace::entering("mln::io::plot::save_image_sh<image1d<I>>");
	mln_precondition(img.is_valid());

	std::ofstream           out(filename.c_str());
	bool                    result = !out.fail();	
	
	if (result)
	{
	  mln_piter(image1d<I>) p(img.domain());

	  // Output data prelude (terminal X11, impulse).
	  out << "#!/bin/sh"                                  << std::endl;
	  out << "##########################"                 << std::endl;
	  out << "# Two columns = (x, val) #"                 << std::endl;
	  out << "##########################"                 << std::endl;
	  out                                                 << std::endl;
	  out << "gnuplot <<EOF"                              << std::endl;
	  out << "set terminal x11 persist 1"                 << std::endl;
	  out << "plot '-' with impulse"                      << std::endl;
	  
	  // Output data.
	  for_all(p)
	  {
	    out << p.ind() << " ";
	    out << img(p)  << std::endl;
	  }
	  
	  // Close gnuplot data stream.
	  out << "e"                                         << std::endl;
	  out << "EOF"                                       << std::endl;
	  
	  out.close();
	}
	else
	{
	  std::cerr << "ERROR[mln::io::plot::save_image_sh]:"<< filename;
	  std::cerr << " couldn't be opened !!"              << std::endl;
	}
	
	trace::exiting("mln::io::plot::save_image_sh<image1d<I>>");
	return result;
      }

      //------------------------------------------------------------------------
      // save_image_sh(const image3d<I>&, const std::string&)
      //------------------------------------------------------------------------


      template <typename I>
      inline
      bool save_image_sh(const image3d<I>& img, const std::string& filename)
      {
	trace::entering("mln::io::plot::save_image_sh<image3d<I>>");
	mln_precondition(img.is_valid());

	std::ofstream           out(filename.c_str());
	bool                    result = !out.fail();

	if (result)
	{
	  mln_piter(image3d<I>) p(img.domain());

	  // Output data prelude (terminal X11, pointtype 0).
	  out << "#!/bin/sh"                                 << std::endl;
	  out << "####################################"      << std::endl;
	  out << "# Columns = (x, y, z, val)         #"      << std::endl;
	  out << "####################################"      << std::endl;
	  out                                                << std::endl;
	  out << "gnuplot <<EOF"                             << std::endl;
	  out << "set terminal x11 persist 1"                << std::endl;
	  out << "splot '-' with points palette pointtype 7" << std::endl;
	
	  // Output data.
	  for_all(p)
	  {
	    out << p.row()   << " ";
	    out << p.col()   << " ";
	    out << p.sli()   << " ";
	    out << img(p)    << std::endl;
	  }

	  // Close gnuplot data stream.
	  out << "e"                                         << std::endl;
	  out << "EOF"                                       << std::endl;

	  out.close();
	}
	else
	{
	  std::cerr << "ERROR[mln::io::plot::save_image_sh]:"<< filename;
	  std::cerr << " couldn't be opened !!"              << std::endl;
	}
	
	trace::exiting("mln::io::plot::save_image_sh<image3d<I>>");
	return result;
      }

      //------------------------------------------------------------------------
      // Facade.
      //------------------------------------------------------------------------

      template <typename I>
      inline
      bool save_image_sh_dispatch(const Image<I>&    img,
				  const std::string& filename)
      {
	return save_image_sh_dispatch_overload(exact(img), filename);
      }

      template <typename I>
      inline
      bool save_image_sh(const Image<I>& img, const std::string& filename)
      {
	trace::entering("mln::io::plot::save_image_sh");

	bool result = save_image_sh_dispatch(img, filename);

	trace::exiting("mln::io::plot::save_image_sh");
	return result;
      }


#endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::accu::stat

  } // end of namespace mln::accu

} // end of namespace mln

#endif // ! MLN_IO_PLOT_SAVE_IMAGE_SH_HH