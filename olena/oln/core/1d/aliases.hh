// Copyright (C) 2001, 2003, 2004, 2005, 2006 EPITA Research and
// Development Laboratory
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

#ifndef OLN_CORE_1D_ALIASES_HH
# define OLN_CORE_1D_ALIASES_HH


namespace oln
{

  /// \{
  /// Forward declarations.
  template <typename C> class point1d_;
  template <typename C> class dpoint1d_;
  template <typename D> class neighb_;
  template <typename P> class bbox_;
  template <typename P> class topo_lbbox_;
  template <typename T> class fwd_piter_bbox_;
  template <typename T> class bkd_piter_bbox_;
  class grid1d;
  /// \}


  /// \{
  /// Alliases.
  typedef  point1d_<int>  point1d;
  typedef dpoint1d_<int> dpoint1d;

  typedef neighb_<dpoint1d> neighb1d;

  typedef bbox_<point1d> bbox1d;
  typedef topo_lbbox_<point1d> topo1d;
  typedef fwd_piter_bbox_<topo1d> fwd_piter1d;
  typedef bkd_piter_bbox_<topo1d> bkd_piter1d;

  typedef  point1d_<float>  point1df;
  typedef dpoint1d_<float> dpoint1df;
  /// \}


} // end of namespace oln


#endif // ! OLN_CORE_1D_ALIASES_HH
