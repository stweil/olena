// Copyright (C) 2006, 2009 EPITA Research and Development Laboratory
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

#ifndef MLN_UTIL_TRACKED_PTR_HH
# define MLN_UTIL_TRACKED_PTR_HH

/// \file mln/util/tracked_ptr.hh
///
/// Definition of a smart pointer for shared data with tracking.
///
/// \todo Split defs from decls.

# include <set>
# include <iostream>

# include <mln/core/contract.hh>



namespace mln
{

  namespace util
  {

    template <typename T>
    struct tracked_ptr
    {
      typedef tracked_ptr<T> self_t;
      typedef std::set<self_t*> holders_t;

      T* ptr_;
      holders_t* holders_;

      /// Coercion towards Boolean (for arithmetical tests).
      operator bool() const;

      /// Negation (for arithmetical tests).
      bool operator !() const;

      /// Mimics the behavior of op-> for a pointer in the const case.
      ///
      /// \invariant Pointer proxy exists.
      const T* operator->() const;

      /// Mimics the behavior of op-> for a pointer in the mutable case.
      ///
      /// \invariant Pointer proxy exists.
      T* operator->();

      /// Ctor.
      tracked_ptr();

      /// Ctor.
      tracked_ptr(T* ptr);

      /// Cpy ctor.
      tracked_ptr(const tracked_ptr<T>& rhs);

      /// Assignment.
      tracked_ptr<T>& operator=(const tracked_ptr<T>& rhs);

      /// Assignment.
      tracked_ptr<T>& operator=(T* ptr);

      /// Dtor.
      ~tracked_ptr();

      bool run_() const;

      void clean_();

    };




# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    tracked_ptr<T>::operator bool() const
    {
      mln_invariant(run_());
      return ptr_ != 0;
    }

    template <typename T>
    /// Negation (for arithmetical tests).
    inline
    bool tracked_ptr<T>::operator !() const
    {
      mln_invariant(run_());
      return ! bool(*this);
    }

    template <typename T>
    /// Mimics the behavior of op-> for a pointer in the const case.
    ///
    /// \invariant Pointer proxy exists.
    inline
    const T* tracked_ptr<T>::operator->() const
    {
      mln_invariant(run_());
      mln_precondition(ptr_ != 0);
      return ptr_;
    }

    template <typename T>
    /// \brief Mimics the behavior of op-> for a pointer in the mutable case.
    ///
    /// \invariant Pointer proxy exists.
    inline
    T* tracked_ptr<T>::operator->()
    {
      mln_invariant(run_());
      mln_precondition(ptr_ != 0);
      return ptr_;
    }

    template <typename T>
    /// Ctor.
    inline
    tracked_ptr<T>::tracked_ptr() :
      ptr_(0),
      holders_(0)
    {
      mln_invariant(run_());
    }

    template <typename T>
    /// Ctor.
    inline
    tracked_ptr<T>::tracked_ptr(T* ptr) :
      ptr_(ptr)
    {
      typedef std::set<tracked_ptr<T>*> holders_t;

      if (ptr == 0)
	holders_ = 0;
      else
	{
	  holders_ = new holders_t;
	  holders_->insert(this);
	}
      mln_invariant(run_());
    }

    template <typename T>
    /// Cpy ctor.
    inline
    tracked_ptr<T>::tracked_ptr(const tracked_ptr<T>& rhs) :
      ptr_(rhs.ptr_),
      holders_(rhs.holders_)
    {
      mln_invariant(rhs.run_());
      if (ptr_ != 0)
	holders_->insert(this);
      mln_invariant(run_());
    }

    template <typename T>
    /// Assignment.
    inline
    tracked_ptr<T>& tracked_ptr<T>::operator=(const tracked_ptr<T>& rhs)
    {
      mln_invariant(run_());
      mln_invariant(rhs.run_());
      if (&rhs == this || rhs.ptr_ == ptr_)
	// no-op
	return *this;
      clean_();
      ptr_ = rhs.ptr_;
      holders_ = rhs.holders_;
      // If ptr == 0, holders_ == 0 so we cannot insert anything in it.
      if (holders_ != 0)
        holders_->insert(this);
      return *this;
    }

    template <typename T>
    /// Assignment.
    inline
    tracked_ptr<T>& tracked_ptr<T>::operator=(T* ptr)
    {
      typedef std::set<tracked_ptr<T>*> holders_t;

      mln_invariant(run_());
      if (ptr == ptr_)
	// no-op
	return *this;
      clean_();
      ptr_ = ptr;
      if (ptr == 0)
	holders_ = 0;
      else
	{
	  holders_ = new holders_t;
	  holders_->insert(this);
	}
      return *this;
    }

    /// Dtor.
    template <typename T>
    inline
    tracked_ptr<T>::~tracked_ptr()
    {
      clean_();
    }

    template <typename T>
    inline
    bool tracked_ptr<T>::run_() const
    {
      typedef std::set<tracked_ptr<T>*> holders_t;

      mln_invariant((ptr_ && holders_) || (! ptr_ && ! holders_));
      if (ptr_ == 0)
	return true;
      mln_invariant(holders_->size() > 0);
      tracked_ptr<T>* this_ = const_cast<tracked_ptr<T>*>(this);
      mln_invariant(holders_->find(this_) != holders_->end());
      this_ = 0;
      typename holders_t::const_iterator i;
      for (i = holders_->begin(); i != holders_->end(); ++i)
	mln_invariant((*i)->ptr_ == ptr_);
      return true;
    }

    template <typename T>
    inline
    void tracked_ptr<T>::clean_()
    {
      mln_invariant(run_());
      if (ptr_ == 0)
	// no-op
	return;
      if (holders_->size() == 1)
	{
	  delete ptr_;
	  delete holders_;
	}
      else
	holders_->erase(this);
      ptr_ = 0;
      holders_ = 0;
      mln_invariant(run_());
    }

    template <typename T>
    inline
    std::ostream& operator<<(std::ostream& ostr, const tracked_ptr<T>& tp)
    {
      typedef std::set<tracked_ptr<T>*> holders_t;

      ostr << "tracked_ptr @ " << (&tp)
	   << " { ptr = " << tp.ptr_
	   << " / holders = ";
      if (tp.holders_ == 0)
	ostr << "0";
      else
	{
	  typename holders_t::const_iterator i;
	  for (i = tp.holders_->begin(); i != tp.holders_->end(); ++i)
	    ostr << (*i) << ' ';
	}
      ostr << " }";
      return ostr;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln


#endif // ! MLN_UTIL_TRACKED_PTR_HH
