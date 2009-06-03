// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_INTERNAL_FORCE_EXACT_HH
# define MLN_CORE_INTERNAL_FORCE_EXACT_HH

/*! \file mln/core/internal/force_exact.hh
 *
 * \brief Definition of a violent cast for internal use only.
 *
 */



namespace mln
{

  namespace internal
  {

    /*! \internal Violent cast.
     * This cast is an alternative to the mln::exact cast.
     * It is used for objects that do not derive from
     * mln::Object.
     * Warning Do not to use this cast!
     * see mln::exact
     */
    template <typename E, typename T>
    E& force_exact(const T& ref)
    {
      /*
	static const E exact_obj;
	static const Type& exact_obj_ref = exact_obj;
	static const int exact_offset =
	(const char*)(void*)(&exact_obj_ref)
	- (const char*)(void*)(&exact_obj);
	return *(E*)((char*)(this_) - exact_offset);
      */
      static const E* exact_obj;
      static const T& exact_obj_ref = *exact_obj;
      static const int exact_offset =
	(const char*)(void*)(&exact_obj_ref)
	- (const char*)(void*)( exact_obj);

      return *(E*)((char*)(&ref) - exact_offset);
    }

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_FORCE_EXACT_HH
