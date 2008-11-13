// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Olena Library.  This library is free
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

#ifndef MLN_LOGICAL_NOT_HH
# define MLN_LOGICAL_NOT_HH

/// \file mln/logical/not.hh
///
/// Point-wise "logical not" of a binary image.

# include <mln/logical/includes.hh>
# include <mln/fun/v2b/lnot.hh>


namespace mln
{

  namespace logical
  {

    /*! Point-wise "logical not" of image \p input.
     *
     * \param[in] input the input image.
     * \result The result image.
     *
     * \pre \p input.has_data
     */
    template <typename I>
    mln_concrete(I) not_(const Image<I>& input);


    /*! Point-wise in-place "logical not" of image \p input.
     *
     * \param[in,out] input The target image.
     *
     * It performs: \n
     *   for all p of input.domain \n
     *     input(p) = not input(p)
     *
     * \pre \p input.has_data
     */
    template <typename I>
    void not_inplace(Image<I>& input);



# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    inline
    mln_concrete(I) not_(const Image<I>& input)
    {
      trace::entering("logical::not_");

      mln_precondition(exact(input).has_data());

      fun::v2b::lnot<mln_value(I)> f;
      mln_concrete(I) output = level::transform(input, f);

      trace::exiting("logical::not_");
      return output;
    }

    template <typename I>
    inline
    void not_inplace(Image<I>& input)
    {
      trace::entering("logical::not_inplace");

      mln_precondition(exact(input).has_data());

      fun::v2b::lnot<mln_value(I)> f;
      level::transform_inplace(input, f);

      trace::exiting("logical::not_inplace");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::logical

} // end of namespace mln


#endif // ! MLN_LOGICAL_NOT_HH
