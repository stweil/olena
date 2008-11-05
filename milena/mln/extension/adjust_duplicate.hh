// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_EXTENSION_ADJUST_DUPLICATE_HH
# define MLN_CORE_EXTENSION_ADJUST_DUPLICATE_HH

/// \file mln/extension/adjust_duplicate.hh
///
/// Adjust the size of the domain extension then duplicate the image
/// inner boundary.
///
/// \todo Fix doc.

# include <mln/extension/adjust.hh>
# include <mln/extension/duplicate.hh>


namespace mln
{

  namespace extension
  {

    /// Adjust then duplicate.
    template <typename I, typename W>
    void adjust_duplicate(const Image<I>& ima,
			  const Window<W>& win);

    template <typename I, typename W>
    void adjust_duplicate(const Image<I>& ima,
			  const Weighted_Window<W>& wwin);

    template <typename I, typename N>
    void adjust_duplicate(const Image<I>& ima,
			  const Neighborhood<N>& nbh);

    template <typename I>
    void adjust_duplicate(const Image<I>& ima,
			  unsigned delta);



# ifndef MLN_INCLUDE_ONLY

    // Facades.

    template <typename I, typename W>
    void adjust_duplicate(const Image<I>& ima,
			  const Window<W>& win)
    {
      trace::entering("extension::adjust_duplicate");
      extension::adjust(ima, win);
      extension::duplicate(ima);
      trace::exiting("extension::adjust_duplicate");
    }

    template <typename I, typename W>
    void adjust_duplicate(const Image<I>& ima,
			  const Weighted_Window<W>& wwin)
    {
      trace::entering("extension::adjust_duplicate");
      extension::adjust(ima, wwin);
      extension::duplicate(ima);
      trace::exiting("extension::adjust_duplicate");
    }

    template <typename I, typename N>
    void adjust_duplicate(const Image<I>& ima,
			  const Neighborhood<N>& nbh)
    {
      trace::entering("extension::adjust_duplicate");
      extension::adjust(ima, nbh);
      extension::duplicate(ima);
      trace::exiting("extension::adjust_duplicate");
    }

    template <typename I>
    void adjust_duplicate(const Image<I>& ima,
			  unsigned delta)
    {
      trace::entering("extension::adjust_duplicate");
      extension::adjust(ima, delta);
      extension::duplicate(ima);
      trace::exiting("extension::adjust_duplicate");
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::extension

} // end of namespace mln


#endif // ! MLN_CORE_EXTENSION_DUPLICATE_HH
