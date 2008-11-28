// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_MORPHO_ELEMENTARY_GRADIENT_HH
# define MLN_MORPHO_ELEMENTARY_GRADIENT_HH

/// \file mln/morpho/elementary/gradient.hh
///
/// \todo Add fastest versions (be careful about extension values...)

# include <mln/morpho/includes.hh>
# include <mln/accu/min_max.hh>


namespace mln
{

  namespace morpho
  {

    namespace elementary
    {


      template <typename I, typename N>
      mln_concrete(I)
      gradient(const Image<I>& input, const Neighborhood<N>& nbh);


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	template <typename I, typename N>
	void
	gradient_tests(const Image<I>& input, const Neighborhood<N>& nbh)
	{
	  mln_precondition(exact(input).has_data());
	  // mln_precondition(exact(nbh).is_valid());
	  (void) input;
	  (void) nbh;
	}

      } // end of namespace mln::morpho::elementary::tests


      namespace impl
      {

	template <typename I, typename N>
	mln_concrete(I)
	gradient_on_function(const Image<I>& input_, const Neighborhood<N>& nbh_)
	{
	  trace::entering("morpho::elementary::impl::gradient_on_function");

	  const I& input = exact(input_);
	  const N& nbh   = exact(nbh_);
	  internal::gradient_tests(input, nbh);

	  accu::min_max<mln_value(I)> a;

	  mln_concrete(I) output;
	  initialize(output, input);

	  mln_piter(I) p(input.domain());
	  mln_niter(N) n(nbh, p);
	  for_all(p)
	    {
	      a.init();
	      for_all(n) if (input.domain().has(n))
		a.take(input(n));
	      output(p) = a.second() - a.first();
	    }

	  trace::exiting("morpho::elementary::impl::gradient_on_function");
	  return output;
	}

	template <typename I, typename N>
	mln_concrete(I)
	gradient_on_set(const Image<I>& input_, const Neighborhood<N>& nbh_)
	{
	  trace::entering("morpho::elementary::impl::gradient_on_set");

	  const I& input = exact(input_);
	  const N& nbh   = exact(nbh_);
	  internal::gradient_tests(input, nbh);

	  mln_concrete(I) output;
	  initialize(output, input);
	  level::fill(output, false);

	  mln_piter(I) p(input.domain());
	  mln_niter(N) n(nbh, p);
	  for_all(p)
	    if (input(p) == true)
	      {
		for_all(n) if (input.domain().has(n))
		  if (input(n) == false)
		    {
		      output(p) = true;
		      break;
		    }
	      }
	    else // input(p) == false
	      {
		for_all(n) if (input.domain().has(n))
		  if (input(n) == true)
		    {
		      output(p) = true;
		      break;
		    }
	      }

	  trace::exiting("morpho::elementary::impl::gradient_on_set");
	  return output;
	}

      } // end of namespace mln::morpho::elementary::impl


      namespace internal
      {

	// Dispatch.

	template <typename I, typename N>
	mln_concrete(I)
	gradient_dispatch(trait::image::kind::any,
			  trait::image::speed::any,
			  const Image<I>& input, const Neighborhood<N>& nbh)
	{
	  return impl::gradient_on_function(input, nbh);
	}

	template <typename I, typename N>
	mln_concrete(I)
	gradient_dispatch(trait::image::kind::logic,
			  trait::image::speed::any,
			  const Image<I>& input, const Neighborhood<N>& nbh)
	{
	  return impl::gradient_on_set(input, nbh);
	}

	template <typename I, typename N>
	mln_concrete(I)
	gradient_dispatch(const Image<I>& input, const Neighborhood<N>& nbh)
	{
	  return gradient_dispatch(mln_trait_image_kind(I)(),
				   mln_trait_image_speed(I)(),
				   input, nbh);
	}

      } // end of namespace mln::morpho::elementary::internal


      // Facade.

      template <typename I, typename N>
      mln_concrete(I)
	gradient(const Image<I>& input, const Neighborhood<N>& nbh)
      {
	trace::entering("morpho::elementary::gradient");

	internal::gradient_tests(input, nbh);
	mln_concrete(I) output = internal::gradient_dispatch(input, nbh);
	
	trace::exiting("morpho::elementary::gradient");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::elementary

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_ELEMENTARY_GRADIENT_HH
