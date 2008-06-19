// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_ACCU_INTERNAL_BASE_HH
# define MLN_ACCU_INTERNAL_BASE_HH

/*! \file mln/accu/internal/base.hh
 *
 * \brief Define a base class for implementation of accumulator
 * classes.
 *
 * \todo Cf. mln/core/pseudo_site_base.hh we get some impl from
 * site_impl; we want here the same effect except that the subject is
 * not always a site.  Maybe we actually need a more generic mechanism
 * s.a. subject_impl...
 */

# include <mln/core/concept/accumulator.hh>
# include <mln/metal/unqualif.hh>


namespace mln
{

  namespace accu
  {

    namespace internal
    {

      /*! \internal Generic min accumulator class.
       * Base class for implementation of accumulator classes.
       */
      template <typename R, typename E>
      class base_ : public Accumulator<E>,
		    public mln::internal::proxy_impl< mlc_unqualif(R), E >
      {
	typedef mlc_unconst(R)  tmp_;
	typedef mlc_unref(tmp_) result_;
      public:

	// As a proxy:
	typedef R               q_subject;
	typedef mlc_unqualif(R)   subject;
	R unproxy() const;

	// As an accumulator:
	typedef R result;

      protected:
	base_();
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename R, typename E>
      inline
      base_<R,E>::base_()
      {
      }

      template <typename R, typename E>
      inline
      R
      base_<R,E>::unproxy() const
      {
	return exact(this)->to_result();
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::accu::internal

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_INTERNAL_BASE_HH
