// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

/// \file
/// \brief Morphological operators in graph space implemented on
/// mln::image2d-based cubical complexes.

#include <mln/io/pbm/load.hh>

#include <mln/debug/println.hh>

#include "apps/graph-morpho/morpho.hh"

#include "apps/graph-morpho/io.hh"
#include "apps/data.hh"


int main()
{
  using namespace mln;

  // Remove borders so as to avoid side effects.
  border::thickness = 0;

  /* Binary graph-based image with vertices aligned on a discrete 2D grid.

     Of course, it would have been better, simpler and faster to use a
     cubical 1-complex here, but they are not yet available (as of
     2009-09-10).  */
  typedef mln::bin_1complex_image2d ima_t;

  // ------------------------ //
  // Dilations and erosions.  //
  // ------------------------ //

  /* Create an image corresponding to the graph X of the ISMM 2009
     paper from Jean Cousty et al.  */
  image2d<bool> x = io::pbm::load(MLN_APPS_DIR "/graph-morpho/x.pbm");
  debug::println("x:", x);

  debug::println("dilation_e2v(x):", dilation_e2v(x)) ;
  debug::println("erosion_v2e(x):",  erosion_v2e(x));

  debug::println("erosion_e2v(x):",  erosion_e2v(x));
  debug::println("dilation_v2e(x):", dilation_v2e(x));

  debug::println("dilation_graph(x):", dilation_graph(x));
  debug::println("erosion_graph(x):",  erosion_graph(x));

  debug::println("alpha3(x):", alpha3(x));
  debug::println("beta3(x):",  beta3(x));

  // --------- //
  // Filters.  //
  // --------- //

  // Create an image corresponding to the graph Y.
  image2d<bool> y = io::pbm::load(MLN_APPS_DIR "/graph-morpho/y.pbm");
  debug::println("y:", y);

  debug::println("opening_graph(y):",      opening_graph(y));
  debug::println("half_opening_graph(y):", half_opening_graph(y));
  debug::println("beta3(alpha3(y)):",      beta3(alpha3(y)));

  // Create an image corresponding to the graph Z.
  image2d<bool> z = io::pbm::load(MLN_APPS_DIR "/graph-morpho/z.pbm");
  debug::println("z:", z);

  debug::println("closing_graph(z):",      closing_graph(z));
  debug::println("half_closing_graph(z):", half_closing_graph(z));
  debug::println("alpha3(beta3(z)):",      alpha3(beta3(z)));
}
