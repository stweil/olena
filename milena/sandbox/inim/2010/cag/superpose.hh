// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.


#ifndef MLN_CAG_SUPERPOSE_HH
# define MLN_CAG_SUPERPOSE_HH

/// \file superpose.hh
///
/// Convert an image to a rgb8 image and draw points
/// whose value is less that mln_max(...).

# include <mln/core/concept/image.hh>
# include <mln/core/image/dmorph/image_if.hh>
# include <mln/data/convert.hh>
# include <mln/data/fill.hh>
# include <mln/value/rgb8.hh>
# include <mln/literal/colors.hh>
# include <mln/pw/all.hh>



namespace mln
{

  namespace cag
  {

    /// Convert an image to a rgb8 image and draw the watershed lines.
    /*
    ** \param[in] input_    An image.
    ** \param[in] ws_ima_   The watershed transform of \p input_.
    ** \param[in] wsl_color The color used for the watershed line.
    */
    template <typename I, typename J>
    inline
    mln_ch_value(I,value::rgb8)
      superpose(const Image<I>& input_,
		const Image<J>& ws_ima_,
		const value::rgb8& wsl_color);


    /// Convert an image to a rgb8 image and draw the watershed lines.
    /*
    ** \param[in] input_    An image.
    ** \param[in] ws_ima_   The watershed transform of \p input_.
    **
    ** The watershed line is drawn in red.
    */
    template <typename I, typename J>
    inline
    mln_ch_value(I,value::rgb8)
      superpose(const Image<I>& input,
		const Image<J>& ws_ima);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename J>
    inline
    mln_ch_value(I,value::rgb8)
      superpose(const Image<I>& input_,
		const Image<J>& ws_ima_,
		const value::rgb8& wsl_color)
    {
      trace::entering("cag::superpose");

      const I& input = exact(input_);
      const J& ws_ima = exact(ws_ima_);
      mln_precondition(input.is_valid());
      mln_precondition(ws_ima.is_valid());

      mln_ch_value(I,value::rgb8) output =
	data::convert(value::rgb8(), input);

      data::fill((output |
		  (pw::value(ws_ima) != pw::cst(mln_max(mln_value(J))))).rw(),
		 wsl_color);

      trace::exiting("cag::superpose");
      return output;
    }


    template <typename I, typename J>
    inline
    mln_ch_value(I,value::rgb8)
      superpose(const Image<I>& input,
		const Image<J>& ws_ima)
    {
      return superpose(input, ws_ima, literal::red);
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::cag

} // end of namespace mln

#endif // ! MLN_CAG_SUPERPOSE_HH
