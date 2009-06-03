// Copyright (C) 2008 EPITA Research and Development Laboratory
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


#ifndef POINT_SET_BASE_HH
# define POINT_SET_BASE_HH

/*! \file sandbox/pellegrin/cond_inheritance/internal/point_set_base.hh
 *
 * \brief Definition of point_set_base class.
 */

# include "internal/uni_set.hh"
# include "internal/multi_set.hh"


namespace mln
{
  // fwd declaration
  template <typename E>
  class p_set;
  template <typename E>
  class p_array;

  namespace internal
  {
    /*! \brief Point set base class.
    */
    template <typename T, typename E>
    class point_set_base;
    
    template <typename E>
    class point_set_base<p_set<E>, E>: public uni_set<E>
    {
    protected:
      point_set_base ();
    };

    template <typename E>
    class point_set_base<p_array<E>, E>: public multi_set<E>
    {
    protected:
      point_set_base ();
    };

# ifndef MLN_INCLUDE_ONLY

    template <typename E>
    inline
    point_set_base<p_set<E>, E>::point_set_base ()
    {
      std::cout << "    " << __FILE__
		<< ": constructeur mln::internal::point_set_base<p_set<E>, E>"
		<< std::endl;
    }

    template <typename E>
    inline
    point_set_base<p_array<E>, E>::point_set_base ()
    {
      std::cout << "    " << __FILE__
		<< ": constructeur "
		"mln::internal::point_set_base<p_array<E>, E>"
		<< std::endl;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln

#endif // ! POINT_SET_BASE_HH
