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

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/labeling/colorize.hh>
#include <mln/value/label_16.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/literal/colors.hh>
#include <mln/util/array.hh>

#include <scribo/primitive/extract/objects.hh>
#include <scribo/primitive/group/apply.hh>
#include <scribo/primitive/link/with_several_left_links.hh>
#include <scribo/primitive/link/with_several_right_links.hh>
#include <scribo/debug/save_linked_bboxes_image.hh>
#include <scribo/primitive/group/from_double_link.hh>
#include <scribo/filter/objects_small.hh>

#include <scribo/debug/save_bboxes_image.hh>
#include <scribo/make/debug_filename.hh>

int usage(const char *name)
{
  std::cout << "Usage: " << name << " <input.pbm> " << std::endl;
  return 1;
}


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc < 1)
    return usage(argv[0]);

  scribo::make::internal::debug_filename_prefix = "group_with_double_several_links";

  image2d<bool> input;
  io::pbm::load(input, argv[1]);


  value::label_16 nbboxes;
  typedef image2d<value::label_16> L;
  std::cout << "extract bboxes" << std::endl;
  typedef object_image(L) text_t;
  text_t text = scribo::primitive::extract::objects(input, c8(), nbboxes);


  std::cout << "Remove small components" << std::endl;
  text = filter::objects_small(text,4);

  std::cout << "Group with left link" << std::endl;
  object_links<L> left_link
	= primitive::link::with_several_left_links(text, 30);

  std::cout << "Group with right link" << std::endl;
  object_links<L> right_link
	= primitive::link::with_several_right_links(text, 30);

  std::cout << "BEFORE - nbboxes = " << nbboxes << std::endl;

  scribo::debug::save_linked_bboxes_image(input,
					  text, left_link, right_link,
					  literal::red, literal::cyan, literal::yellow,
					  literal::green,
					  scribo::make::debug_filename("links.ppm"));

  // With validation.
  std::cout << "Group from double link" << std::endl;

  object_groups<L> groups
    = primitive::group::from_double_link(text, left_link, right_link);

  text_t grouped_text = primitive::group::apply(text, groups);

  io::ppm::save(mln::labeling::colorize(value::rgb8(),
				     grouped_text,
				     grouped_text.nlabels()),
		scribo::make::debug_filename("label_color.ppm"));

  std::cout << "AFTER double grouping - nbboxes = " << grouped_text.bboxes().nelements() << std::endl;

  scribo::debug::save_bboxes_image(input, grouped_text.bboxes(),
				   literal::red,
				   scribo::make::debug_filename("bboxes.ppm"));

}
