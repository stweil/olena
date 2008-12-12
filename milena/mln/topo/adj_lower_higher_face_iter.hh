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
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_TOPO_ADJ_LOWER_HIGHER_FACE_ITER_HH
# define MLN_TOPO_ADJ_LOWER_HIGHER_FACE_ITER_HH

/// \file mln/topo/adj_lower_higher_face_iter.hh
///
/// Definition of forward and backward iterators on all the
/// adjacent (n-1)-faces and (n+1)-faces of a (reference) n-face in a
/// complex.

# include <mln/topo/internal/complex_relative_iterator_sequence.hh>
# include <mln/topo/adj_lower_face_iter.hh>
# include <mln/topo/adj_higher_face_iter.hh>


namespace mln
{

  namespace topo
  {

    // Forward declaration.
    template <unsigned D> class complex;

    namespace internal
    {

      template <typename I1, typename I2, typename E>
      class complex_relative_iterator_sequence;

    } // end of namespace mln::topo::internal


    /*------------------------------------------.
    | topo::adj_lower_higher_face_fwd_iter<D>.  |
    `------------------------------------------*/

    /// Forward iterator on all the adjacent (n-1)-faces and
    /// (n+1)-faces of the n-face of an mln::complex<D>.
    ///
    /// \arg \p D The dimension of the complex this iterator belongs to.
    template <unsigned D>
    class adj_lower_higher_face_fwd_iter
      : public internal::complex_relative_iterator_sequence<adj_lower_face_fwd_iter<D>,
							    adj_higher_face_fwd_iter<D>,
							    adj_lower_higher_face_fwd_iter<D> >
    {
      typedef adj_lower_higher_face_fwd_iter<D> self_;
      typedef adj_lower_face_fwd_iter<D> iter1_;
      typedef adj_higher_face_fwd_iter<D> iter2_;
      typedef internal::complex_relative_iterator_sequence< iter1_,
							    iter2_,
							    self_ > super_;

    public:
      /// Construction.
      /// \{
      adj_lower_higher_face_fwd_iter();
      template <typename Fref>
      adj_lower_higher_face_fwd_iter(const Fref& f_ref);
      /// \}
    };


    /*------------------------------------------.
    | topo::adj_lower_higher_face_bkd_iter<D>.  |
    `------------------------------------------*/

    /// Forward iterator on all the adjacent (n-1)-faces and
    /// (n+1)-faces of the n-face of an mln::complex<D>.
    ///
    /// \arg \p D The dimension of the complex this iterator belongs to.
    template <unsigned D>
    class adj_lower_higher_face_bkd_iter
      : public internal::complex_relative_iterator_sequence<adj_higher_face_bkd_iter<D>,
							    adj_lower_face_bkd_iter<D>,
							    adj_lower_higher_face_bkd_iter<D> >
    {
      typedef adj_lower_higher_face_bkd_iter<D> self_;
      typedef adj_higher_face_bkd_iter<D> iter1_;
      typedef adj_lower_face_bkd_iter<D> iter2_;
      typedef internal::complex_relative_iterator_sequence< iter1_,
							    iter2_,
							    self_ > super_;

    public:
      /// Construction.
      /// \{
      adj_lower_higher_face_bkd_iter();
      template <typename Fref>
      adj_lower_higher_face_bkd_iter(const Fref& f_ref);
      /// \}
    };



# ifndef MLN_INCLUDE_ONLY

    /*------------------------------------------.
    | topo::adj_lower_higher_face_fwd_iter<D>.  |
    `------------------------------------------*/

    template <unsigned D>
    inline
    adj_lower_higher_face_fwd_iter<D>::adj_lower_higher_face_fwd_iter()
    {
    }

    template <unsigned D>
    template <typename Fref>
    inline
    adj_lower_higher_face_fwd_iter<D>::adj_lower_higher_face_fwd_iter(const Fref& f_ref)
      : super_(f_ref)
    {
    }


    /*------------------------------------------.
    | topo::adj_lower_higher_face_bkd_iter<D>.  |
    `------------------------------------------*/

    template <unsigned D>
    inline
    adj_lower_higher_face_bkd_iter<D>::adj_lower_higher_face_bkd_iter()
    {
    }

    template <unsigned D>
    template <typename Fref>
    inline
    adj_lower_higher_face_bkd_iter<D>::adj_lower_higher_face_bkd_iter(const Fref& f_ref)
      : super_(f_ref)
    {
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_ADJ_LOWER_HIGHER_FACE_ITER_HH
