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

#ifndef MLN_WIN_DISK2D_HH
# define MLN_WIN_DISK2D_HH

/// \file mln/win/disk2d.hh
///
/// Definition of the mln::win::disk2d window.

# include <mln/core/internal/classical_window_base.hh>
# include <mln/core/alias/dpoint2d.hh>


namespace mln
{

  mln_internal_add_classical_window_trait(disk2d);


  namespace win
  {
 
    /*! \brief Disk window defined on the 2D square grid.
     *
     * A disk2d is centered and symmetric.
     *
     */
    struct disk2d : public internal::classical_window_base< dpoint2d, disk2d >
    {
      /*! \brief Constructor.
       *
       * \param[in] length Length, thus diameter.
       *
       */
      disk2d(unsigned length);

      /*! \brief Give the disk diameter.
       */
      unsigned length() const;

      /*! \brief Give the maximum coordinate gap between the window
       * center and a window point.
       */
      unsigned delta_() const;

      void print_(std::ostream& ostr) const;

    protected:
      unsigned length_;
    };

 

# ifndef MLN_INCLUDE_ONLY

    inline
    disk2d::disk2d(unsigned length)
      : length_(length)
    {
      mln_precondition(length % 2 == 1);
      const def::coord
	r  = static_cast<def::coord>(length / 2),
	minus_r = static_cast<def::coord>(-r),
	r2 = static_cast<def::coord>(r * r);
      for (def::coord a = minus_r; a <= r; ++a)
	for (def::coord b = minus_r; b <= r; ++b)
	  if (a * a + b * b <= r2)
	    insert(dpoint2d(a, b));
    }

    inline
    unsigned disk2d::length() const
    {
      return length_;
    }

    inline
    unsigned disk2d::delta_() const
    {
      return length_ / 2;
    }

    inline
    void disk2d::print_(std::ostream& ostr) const
    {
      ostr << "[disk2d: length=" << length_ << ']';
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::win

} // end of namespace mln



#endif // ! MLN_WIN_DISK2D_HH
