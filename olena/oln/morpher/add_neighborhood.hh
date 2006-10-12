// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_MORPHER_ADD_NEIGHBORHOOD
# define OLN_MORPHER_ADD_NEIGHBORHOOD

# include <oln/morpher/internal/image_extension.hh>
# include <oln/core/gen/topo_add_nbh.hh>
# include <oln/core/iterator_vtypes.hh>
# include <oln/core/gen/fwd_niter_neighb.hh>


namespace oln
{

  namespace morpher
  {
    // Forward declaration.
    template <typename Image, typename Neighb> struct add_neighborhood;

  } // end of namespace oln::morpher


  /// Super type.
  template <typename Image, typename Neighb>
  struct set_super_type< morpher::add_neighborhood<Image, Neighb> >
  {
    typedef morpher::add_neighborhood<Image, Neighb> self_t;
    typedef morpher::internal::image_extension<Image, self_t> ret;
  };

  template <typename Image, typename Neighb>
  struct vtypes< morpher::add_neighborhood<Image, Neighb> >
  {
    // Topology type.
    typedef topo_add_nbh< oln_type_of(Image, topo), Neighb > topo_type;

    // Neighborhood type.
    typedef Neighb neighborhood_type;
  };


  namespace morpher
  {
    /// Neighborhood addition morpher.
    template <typename Image, typename Neighb>
    class add_neighborhood : public morpher::internal::image_extension< Image,
								        add_neighborhood<Image, Neighb> >
    // FIXME: Uncomment. stc_get_supers(mlc_comma_1(add_neighborhood<Image, Neighb>))
    {
    private:

      typedef add_neighborhood<Image, Neighb> self_t;
      typedef stc_get_nth_super(self_t, 1) super_t;
      typedef oln_type_of(self_t, topo) topo_t;

    public:

      // FIXME: Handle the constness.

      //       template <typename I, typename N>
      //       add_neighborhood(const abstract::image<I>& image, const N& nbh)
      // 	: super_t(image)
      //       {
      //       }

      add_neighborhood(const Image& image, const Neighb& nbh);
      const topo_t& impl_topo() const;

    protected:
      topo_t topo_;
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename Image, typename Neighb>
    add_neighborhood<Image, Neighb>::add_neighborhood(const Image& image, const Neighb& nbh) :
      super_t(image),
      topo_(image.topo(), nbh)
    {
      mlc::assert_equal_<oln_type_of(Image, grid), oln_type_of(Neighb, grid)>::check();
      // FIXME: check that Image is without a nbh
    }

    template <typename Image, typename Neighb>
    const typename add_neighborhood<Image, Neighb>::topo_t&
    add_neighborhood<Image, Neighb>::impl_topo() const
    {
      return topo_;
    }

# endif

  } // end of namespace oln::morpher



  template <typename I, typename N>
  morpher::add_neighborhood<I, N>
  operator + (const abstract::image<I>& image, const abstract::neighborhood<N>& nbh);


# ifndef OLN_INCLUDE_ONLY

  template <typename I, typename N>
  morpher::add_neighborhood<I, N>
  operator + (const abstract::image<I>& image,
	      const abstract::neighborhood<N>& nbh)
  {
    mlc::assert_equal_<oln_type_of(I, grid), oln_type_of(N, grid)>::check();
    // FIXME: check that Image is without a nbh
    morpher::add_neighborhood<I, N> tmp(image.exact(), nbh.exact());
    return tmp;
  }

# endif

} // end of namespace oln


#endif // ! OLN_MORPHER_ADD_NEIGHBORHOOD
