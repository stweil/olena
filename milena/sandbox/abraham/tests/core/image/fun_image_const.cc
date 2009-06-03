// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

# include <mln/core/image/image2d.hh>
# include <mln/core/image/dmorph/fun_image.hh>
# include <mln/fun/v2w2v/cos.hh>

int main ()
{

  using namespace mln;

  typedef const image2d<double>  I;

  double vs[6][5] = {

    { 12, -3, 8, -4, 6 },
    { -2, 22, -1, 45, -1 },
    { -1, -4, -4, -4, -1 },
    { -1, -4, -3, -4, -1 },
    { -1, -4, -5, -3, -1 },
    { -1, -1, -1, -1, -1 }

  };

  I ima(make::image(vs));
  fun_image<mln::fun::v2w2v::cos<double>, I > out(ima);

  double i = 0;

  box_fwd_piter_<point2d> p(ima.domain());
  for_all (p)
  {
    out(p) = i;
    i += 1./40.;
  }

  for_all (p)
    std::cout << out(p) << std::endl;
}
