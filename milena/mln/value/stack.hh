// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_VALUE_STACK_HH
# define MLN_VALUE_STACK_HH

/*! \file mln/value/stack.hh
 *
 * \brief Definition of an image class FIXME
 */

# include <mln/core/internal/image_base.hh>
# include <mln/metal/vec.hh>
# include <mln/value/set.hh>
# include <mln/value/proxy.hh>


namespace mln
{

  namespace value
  {

    // Fwd decl.
    template <unsigned n, typename I> struct stack_image;


    namespace internal
    {

      template <unsigned n, typename I>
      struct helper_stack_image_lvalue_
      {
	typedef value::proxy< stack_image<n,I> > ret;
	static ret make(stack_image<n,I>& ima, const mln_psite(I)& p)
	{
	  ret tmp(ima, p);
	  return tmp;
	}
      };

      template <unsigned n, typename I>
      struct helper_stack_image_lvalue_< n, const I >
      {
	typedef metal::vec<n, mln_value(I)> ret;
	static ret make(stack_image<n, const I>& ima, const mln_psite(I)& p)
	{
	  return ima.read_(p);
	}
      };

    } // end of namespace mln::value::internal


    /*! \brief FIXME
     *
     */
    template <unsigned n, typename I>
    struct stack_image : public mln::internal::image_base_< mln_pset(I), stack_image<n,I> >
    {
      /// Point_Site associated type.
      typedef mln_psite(I) psite;

      /// Point_Set associated type.
      typedef mln_pset(I) pset;

      /// Value associated type.
      typedef metal::vec<n, mln_value(I)> value;

      /// Return type of read-only access.
      typedef value rvalue;

      /// Return type of read-write access.
      typedef typename internal::helper_stack_image_lvalue_<n,I>::ret lvalue;

      /// Value set associated type.
      typedef mln::value::set<value> vset;


      /// Skeleton.
      typedef stack_image< tag::unsigned_<n>, tag::image<I> > skeleton;


      /// Constructor.
      stack_image(const metal::vec<n,I*>& imas);


      /// Test if this image has been initialized.
      bool has_data() const;

      /// Test if a pixel value is accessible at \p p.
      bool owns_(const psite& p) const;

      /// Give the definition domain.
      const mln_pset(I)& domain() const;

      /// Read-only access of pixel value at point site \p p.
      rvalue operator()(const psite& p) const;
      value read_(const psite& p) const;

      /// Read-write access of pixel value at point site \p p.
      lvalue operator()(const psite&);
      void write_(const psite& p, const value& v);

      /// Give the set of values of the image.
      const vset& values() const;

    protected:
      metal::vec<n,I*> imas_;
    };



    template <typename I>
    stack_image<2, const I>
    stack(const Image<I>& ima1, const Image<I>& ima2);
  

    template <typename I>
    stack_image<2, I>
    stack(Image<I>& ima1, Image<I>& ima2);



# ifndef MLN_INCLUDE_ONLY

    // stack_image<n, I>

    template <unsigned n, typename I>
    stack_image<n,I>::stack_image(const metal::vec<n,I*>& imas)
      : imas_(imas)
    {
      for (unsigned i = 0; i < n; ++i)
	{
	  mln_precondition(imas[i] != 0);
	  mln_precondition(imas[i]->has_data());
	}
    }

    template <unsigned n, typename I>
    bool stack_image<n,I>::has_data() const
    {
      for (unsigned i = 0; i < n; ++i)
	mln_invariant(imas_[i]->has_data());
      return true;
    }

    template <unsigned n, typename I>
    bool stack_image<n,I>::owns_(const psite& p) const
    {
      for (unsigned i = 0; i < n; ++i)
	if (! imas_[i]->owns_(p))
	  return false;
      return true;
    }

    template <unsigned n, typename I>
    const mln_pset(I)&
    stack_image<n,I>::domain() const
    {
      return imas_[0]->domain();
    }

    template <unsigned n, typename I>
    metal::vec<n, mln_value(I)>
    stack_image<n,I>::read_(const psite& p) const
    {
      mln_precondition(this->owns_(p));
      metal::vec<n, mln_value(I)> tmp;
      for (unsigned i = 0; i < n; ++i)
	tmp[i] = imas_[i]->operator()(p);
      return tmp;
    }

    template <unsigned n, typename I>
    metal::vec<n, mln_value(I)>
    stack_image<n,I>::operator()(const psite& p) const
    {
      return read_(p);
    }

    template <unsigned n, typename I>
    void
    stack_image<n,I>::write_(const psite& p, const value& v)
    {
      mln_precondition(this->owns_(p));
      // FIXME!!!
      for (unsigned i = 0; i < n; ++i)
	imas_[i]->operator()(p) = v[i];
    }

    template <unsigned n, typename I>
    typename stack_image<n,I>::lvalue
    stack_image<n,I>::operator()(const psite& p)
    {
      return internal::helper_stack_image_lvalue_<n,I>::make(*this, p);
    }

    template <unsigned n, typename I>
    const mln::value::set< metal::vec<n, mln_value(I)> >&
    stack_image<n,I>::values() const
    {
      return vset::the();
    }
  
    // stack(..)

    template <typename I>
    stack_image<2, const I>
    stack(const Image<I>& ima1, const Image<I>& ima2)
    {
      mln_precondition(exact(ima1).domain() == exact(ima2).domain());
      metal::vec<2, const I*> imas;
      imas[0] = & exact(ima1);
      imas[1] = & exact(ima2);
      stack_image<2, const I> tmp(imas);
      return tmp;
    }

    template <typename I>
    stack_image<2, I>
    stack(Image<I>& ima1, Image<I>& ima2)
    {
      mln_precondition(exact(ima1).domain() == exact(ima2).domain());
      metal::vec<2, I*> imas;
      imas[0] = & exact(ima1);
      imas[1] = & exact(ima2);
      stack_image<2, I> tmp(imas);
      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::value

} // end of namespace mln


#endif // ! MLN_VALUE_STACK_HH
