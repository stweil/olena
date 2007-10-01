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

/*! \file tests/level_fill.cc
 *
 * \brief Tests on mln::level::fill
 */

#include <mln/core/image2d_b.hh>
#include <mln/core/sub_image.hh>
#include <mln/level/fill.hh>

#include <mln/debug/println.hh>
#include <mln/value/props.hh>


int main()
{
  using namespace mln;


  unsigned u = 300;
  unsigned char uc = u;
  mln_assertion(uc == 44);

//   {
//     const unsigned size = 3;
//     image2d_b<unsigned> ima(size, size);
//     level::fill(ima, u);
//     debug::println(ima);
//   }

  {
    const unsigned size = 10000;
    image2d_b<unsigned char> ima(size, size);
    for (unsigned i = 0; i < 5; ++i)
      level::fill(ima, uc);
  }

//   {
//     // do *not* compile so that's great since ima is not mutable
//     sub_image< const image2d_b<int>, box2d > ima;
//     level::fill(ima, 0);
//   }

}
