// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TRANSFORM_INTERNAL_CLOSEST_POINT_FUNCTOR_HH
# define MLN_TRANSFORM_INTERNAL_CLOSEST_POINT_FUNCTOR_HH

/// \file mln/transform/internal/closest_point_functor.hh
///
/// Closest point functor.

# include <mln/core/macros.hh>

namespace mln
{

  namespace transform
  {

    namespace internal
    {

      template <typename I>
      struct closest_point_functor
      {
	typedef mln_value(I) V;
	typedef mln_site(I)  P;

	void init(const I&);
	bool inqueue_p_wrt_input_p(const V& input_p);
	void init_p(const P&);
	bool inqueue_p_wrt_input_n(const V& input_n);
	void process(const P&, const P&);

	mln_ch_value(I,P) cp_ima;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      void
      closest_point_functor<I>::init(const I& input)
      {
	initialize(cp_ima, input);
      }

      template <typename I>
      inline
      bool
      closest_point_functor<I>::inqueue_p_wrt_input_p(const V& input_p)
      {
	return input_p == true;
      }

      template <typename I>
      inline
      void
      closest_point_functor<I>::init_p(const P& p)
      {
	cp_ima(p) = p;
      }

      template <typename I>
      inline
      bool
      closest_point_functor<I>::inqueue_p_wrt_input_n(const V& input_n)
      {
	return input_n == false;
      }

      template <typename I>
      inline
      void closest_point_functor<I>::process(const P& p, const P& n)
      {
	cp_ima(n) = p;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::transform::internal

  } // end of namespace mln::transform

} // end of namespace mln


#endif // ! MLN_TRANSFORM_INTERNAL_CLOSEST_POINT_FUNCTOR_HH