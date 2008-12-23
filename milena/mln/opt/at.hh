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

#ifndef MLN_OPT_AT_HH
# define MLN_OPT_AT_HH

/// \file mln/opt/at.hh
///
/// FIXME

# include <mln/core/concept/image.hh>
# include <mln/trait/images.hh>
# include <mln/trace/all.hh>

# include <mln/core/alias/point1d.hh>
# include <mln/core/alias/point2d.hh>
# include <mln/core/alias/point3d.hh>

namespace mln
{

  namespace opt
  {

    /// One dimension
    template <typename I>
    mln_rvalue(I) at(const Image<I>& ima, int ind);

    template <typename I>
    mln_lvalue(I) at(Image<I>& ima, int ind);


    /// Two dimensions
    template <typename I>
    mln_rvalue(I) at(const Image<I>& ima, int row, int col);

    template <typename I>
    mln_lvalue(I) at(Image<I>& ima, int row, int col);

    /// Three dimensions
    template <typename I>
    mln_rvalue(I) at(const Image<I>& ima, int sli, int row, int col);

    template <typename I>
    mln_lvalue(I) at(Image<I>& ima, int sli, int row, int col);


# ifndef MLN_INCLUDE_ONLY


    /// One dimension
    namespace impl
    {

      template <typename I>
      inline
      mln_rvalue(I) at_1d_impl(trait::image::category::domain_morpher,
                               const Image<I>& ima, int ind)
      {
        point1d p(ind);
        return exact(ima)(p);
      }

      template <typename I>
      inline
      mln_rvalue(I) at_1d_impl(trait::image::category::morpher,
                               const Image<I>& ima, int ind)
      {
        // FIXME: what about morpher that modify the image value?
        // (through a function for instance)
        return at(*exact(ima).delegatee_(), ind);
      }

      template <typename I>
      inline
      mln_rvalue(I) at_1d_impl(trait::image::category::primary,
                               const Image<I>& ima, int ind)
      {
        return exact(ima).at(ind);
      }


      template <typename I>
      inline
      mln_lvalue(I) at_1d_impl(trait::image::category::domain_morpher,
                               Image<I>& ima, int ind)
      {
        mlc_is(mln_trait_image_pw_io(I),
               trait::image::pw_io::read_write)::check();

        point1d p(ind);
        return exact(ima)(p);
      }

      template <typename I>
      inline
      mln_lvalue(I) at_1d_impl(trait::image::category::morpher,
                               Image<I>& ima, int ind)
      {
        // FIXME: what about morpher that modify the image value?
        // (through a function for instance)
        return at(*exact(ima).delegatee_(), ind);
      }

      template <typename I>
      inline
      mln_lvalue(I) at_1d_impl(trait::image::category::primary,
                               Image<I>& ima, int ind)
      {
        return exact(ima).at(ind);
      }

    } // end of namespace mln::opt::impl

    template <typename I>
    inline
    mln_rvalue(I) at(const Image<I>& ima, int ind)
    {
      mlc_is(mln_trait_image_dimension(I),
             trait::image::dimension::one_d)::check();

      return impl::at_1d_impl(mln_trait_image_category(I)(), ima, ind);
    }

    template <typename I>
    mln_lvalue(I) at(Image<I>& ima, int ind)
    {
      mlc_is(mln_trait_image_dimension(I),
             trait::image::dimension::one_d)::check();

      return impl::at_1d_impl(mln_trait_image_category(I)(), ima, ind);
    }



    /// Two dimensions
    namespace impl
    {

      template <typename I>
      inline
      mln_rvalue(I) at_2d_impl(trait::image::category::domain_morpher,
                               const Image<I>& ima, int row, int col)
      {
        point2d p(row, col);
        return exact(ima)(p);
      }

      template <typename I>
      inline
      mln_rvalue(I) at_2d_impl(trait::image::category::morpher,
                               const Image<I>& ima, int row, int col)
      {
        // FIXME: what about morpher that modify the image value?
        // (through a function for instance)
        return at(*exact(ima).delegatee_(), row, col);
      }

      template <typename I>
      inline
      mln_rvalue(I) at_2d_impl(trait::image::category::primary,
                               const Image<I>& ima, int row, int col)
      {
        return exact(ima).at(row, col);
      }


      template <typename I>
      inline
      mln_lvalue(I) at_2d_impl(trait::image::category::domain_morpher,
                               Image<I>& ima, int row, int col)
      {
        mlc_is(mln_trait_image_pw_io(I),
               trait::image::pw_io::read_write)::check();

        point2d p(row, col);
        return exact(ima)(p);
      }

      template <typename I>
      inline
      mln_lvalue(I) at_2d_impl(trait::image::category::morpher,
                               Image<I>& ima, int row, int col)
      {
        // FIXME: what about morpher that modify the image value?
        // (through a function for instance)
        return at(*exact(ima).delegatee_(), row, col);
      }

      template <typename I>
      inline
      mln_lvalue(I) at_2d_impl(trait::image::category::primary,
                               Image<I>& ima, int row, int col)
      {
        return exact(ima).at(row, col);
      }

    } // end of namespace mln::opt::impl

    template <typename I>
    inline
    mln_rvalue(I) at(const Image<I>& ima, int row, int col)
    {
      mlc_is(mln_trait_image_dimension(I),
             trait::image::dimension::two_d)::check();

      return impl::at_2d_impl(mln_trait_image_category(I)(), ima, row, col);
    }

    template <typename I>
    mln_lvalue(I) at(Image<I>& ima, int row, int col)
    {
      mlc_is(mln_trait_image_dimension(I),
             trait::image::dimension::two_d)::check();

      return impl::at_2d_impl(mln_trait_image_category(I)(), ima, row, col);
    }


    /// Three dimensions
    namespace impl
    {

      template <typename I>
      inline
      mln_rvalue(I) at_3d_impl(trait::image::category::domain_morpher,
                               const Image<I>& ima, int sli, int row, int col)
      {
        point3d p(sli, row, col);
        return exact(ima)(p);
      }

      template <typename I>
      inline
      mln_rvalue(I) at_3d_impl(trait::image::category::morpher,
                               const Image<I>& ima, int sli, int row, int col)
      {
        // FIXME: what about morpher that modify the image value?
        // (through a function for instance)
        return at(*exact(ima).delegatee_(), sli, row, col);
      }

      template <typename I>
      inline
      mln_rvalue(I) at_3d_impl(trait::image::category::primary,
                               const Image<I>& ima, int sli, int row, int col)
      {
        return exact(ima).at(sli, row, col);
      }


      template <typename I>
      inline
      mln_lvalue(I) at_3d_impl(trait::image::category::domain_morpher,
                               Image<I>& ima, int sli, int row, int col)
      {
        mlc_is(mln_trait_image_pw_io(I),
               trait::image::pw_io::read_write)::check();

        point3d p(sli, row, col);
        return exact(ima)(p);
      }

      template <typename I>
      inline
      mln_lvalue(I) at_3d_impl(trait::image::category::morpher,
                               Image<I>& ima, int sli, int row, int col)
      {
        // FIXME: what about morpher that modify the image value?
        // (through a function for instance)
        return at(*exact(ima).delegatee_(), sli, row, col);
      }

      template <typename I>
      inline
      mln_lvalue(I) at_3d_impl(trait::image::category::primary,
                               Image<I>& ima, int sli, int row, int col)
      {
        return exact(ima).at(sli, row, col);
      }

    } // end of namespace mln::opt::impl

    template <typename I>
    inline
    mln_rvalue(I) at(const Image<I>& ima, int sli, int row, int col)
    {
      mlc_is(mln_trait_image_dimension(I),
             trait::image::dimension::three_d)::check();

      return impl::at_3d_impl(mln_trait_image_category(I)(),
                              ima, sli, row, col);
    }

    template <typename I>
    mln_lvalue(I) at(Image<I>& ima, int sli, int row, int col)
    {
      mlc_is(mln_trait_image_dimension(I),
             trait::image::dimension::three_d)::check();

      return impl::at_3d_impl(mln_trait_image_category(I)(),
                              ima, sli, row, col);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::opt

} // end of namespace mln


#endif // ! MLN_OPT_AT_HH