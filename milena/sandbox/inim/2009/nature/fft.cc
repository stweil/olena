//                                                                  -*- c++ -*-
// Copyright (C) 2004  EPITA Research and Development Laboratory
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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#include <mln/transform/fft.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

using namespace mln;
using namespace transform;
using namespace value;

int main(int argc, const char * argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " in.pgm [other_files.pgm]" << std::endl;
    return 1;
  }

  for (int i = 1; i < argc; ++i)
    {
      image2d<int_u8> ima;
      io::pgm::load(ima, argv[i]);

      fft<double> fourier(ima);

      fourier.transform();

      std::string name(argv[i]);
      name.erase(name.length() - 4);
      io::pgm::save(fourier.transformed_image_log_magn<int_u8>(true), name.append("_fft.pgm"));
    }
}
