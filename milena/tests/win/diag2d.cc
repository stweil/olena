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

/// \file tests/win/diag2d.cc
///
/// Tests on mln::win::diag2d.

#include <cmath>

#include <mln/win/diag2d.hh>
#include <mln/convert/to_image.hh>
#include <mln/debug/println.hh>


int main()
{
  using namespace mln;

  const unsigned l = 5;
  win::diag2d diag(l);

  mln_assertion(diag.delta() == 2);

  for (def::coord x = -5; x <= 5; ++x)
    for (def::coord y = -5; y <= 5; ++y)
    {
      mln_assertion(((abs(x) <= 2) && (abs(y) <= 2)) ||
		    !diag.has(dpoint2d(x, y)));
      std::cout << "x = "
		<< x
		<< ", y = "
		<< y
		<< " : "
		<< ((diag.has(dpoint2d(x, y))) ? "True" : "False")
		<< std::endl;
      mln_assertion((x == -y) ==
		    (diag.has(dpoint2d(x, y))) || abs(x) > 2 || abs(y) > 2);
    }

  debug::println(convert::to_image(diag));
}

