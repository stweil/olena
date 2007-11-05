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


#ifndef MLN_FIXME_FLLT_HH
# define MLN_FIXME_FLLT_HH

/*! \file fllt.hh
 *
 * \brief Fast level line transform of an image.
 *
 */

# include <mln/core/image2d.hh>
# include <mln/core/set_p.hh>
# include <mln/core/inplace.hh>
# include <mln/core/neighb2d.hh>
# include <mln/core/pset_if_piter.hh>
# include <mln/core/pset_if.hh>
# include <mln/core/sub_image.hh>
# include <mln/core/image_if.hh>
# include <mln/core/clone.hh>
# include <mln/core/a_point_of.hh>

# include <mln/debug/println.hh>
# include <mln/debug/println_with_border.hh>

# include <mln/convert/to_image.hh>

# include <mln/border/fill.hh>

# include <mln/level/compute.hh>
# include <mln/level/fill.hh>
# include <mln/accu/min.hh>
# include <mln/accu/max.hh>

# include <mln/set/uni.hh>
# include <mln/set/diff.hh>
# include <mln/set/inter.hh>
# include <mln/set/is_subset_of.hh>

# include <mln/util/tree.hh>
# include <mln/util/branch_iter.hh>

# include <mln/labeling/regional_minima.hh>
# include <mln/labeling/regional_maxima.hh>
# include <mln/labeling/level.hh>

# include <mln/fun/ops.hh>
# include <mln/pw/value.hh>
# include <mln/pw/cst.hh>

# include <mln/util/tree_to_image.hh>
# include <mln/value/int_u8.hh>
# include <mln/level/stretch.hh>
# include <mln/level/compare.hh>
# include <mln/io/pgm/save.hh>

namespace mln
{
  namespace fllt
  {

    template <typename P, typename V>
    struct fllt_node_elt
    {
      V	value;
      set_p<P> points;
      set_p<P> holes;
      /// Tell if his parent if brighter or not.  Nb : if the parent
      /// if brighter, the node come from the lower level set
      bool brighter;
    };

# define fllt_tree(P, V)  util::tree< fllt_node_elt<P, V> >
# define fllt_node(P, V)  util::node< fllt_node_elt<P, V> >
# define fllt_branch(P, V)  util::branch< fllt_node_elt<P, V> >
# define fllt_branch_iter(P, V)  util::branch_iter< fllt_node_elt<P, V> >

    //    # define fllt_node(P, V)  typename fllt_tree(P, V)::node_t



    //   LOWER LEVEL SET : region = c4, border = c8
    //   UPPER LEVEL SET : region = c8, border = c4

    // 1)
    // x0 <- a not tagged local mininum of ima.
    // g <- u(x0)

    // 2)
    // A <- {x0}
    // R <- {}
    // N <- {}

    // 3)
    // N <- N union {x neighbor of a pixel in a}
    // gn <- min u(x) x belongs to N.
    // R <- R union A
    // tag the pixels of A.

    // 4)
    // IF g < gn
    //    IF number of conected components of the border > 1
    //       follow each border to find which is the exterior border
    //       and which are the holes. Keep one pixel of each holes.
    //
    //       Remove from N border of holes.
    //       Recompute gn <- min u(x) x belongs to A
    //
    //	  g <- gn
    //    A <- {x belongs to N / u(x) == g}
    //    N <- N\{x belongs to N / u(x) == g}
    //    GO TO 3)
    // IF g == gn
    //    A <- {x belongs to N / u(x) == g}
    //    N <- N\{x belongs to N / u(x) == g}
    //    GO TO 3)
    // IF g > gn
    //    set the gray-level of the pixels of R to g.
    //    GO TO 1)

    template <typename V>
    void step1 (const image2d<V>& ima,
		point2d p,
		V& g,
		point2d& x0)
    {
      //std::cout << "entering step 1" << std::endl;
      // x0 <- a not tagged local mininum of ima.
      //std::cout << std::endl << "x0 = " << p << std::endl;
      x0 = p;
      // g <- u(x0)
      g = ima(x0);
      //std::cout << "g = " << g << std::endl;
      //std::cout << "exiting step 1" << std::endl;
    }

    template <typename P>
    void step2 (set_p<P>& A,
		set_p<P>& R,
		set_p<P>& N,
		point2d& x0)
    {
      //std::cout << "entering step 2" << std::endl;
      // A <- {x0}
      A.clear();
      A.insert(x0);
      // R <- {}
      R.clear();
      // N <- {}
      N.clear();
      //std::cout << "exiting step 2" << std::endl;
    }


    template <typename V, typename P, typename F>
    void step3 (const image2d<V>& u,
		image2d<bool>& tagged,
		set_p<P>& A,
		set_p<P>& R,
		set_p<P>& N,
		V& gn)
    {
      static bool finished = false;
      //std::cout << "entering step 3" << std::endl;

      // Stop the algorithm.
      if (finished)
      { finished = false; gn -= 2 * F::inc; return; }

      // N <- N union {x neighbor of a pixel in a\R}
      mln_piter(set_p<P>) qa(A);
      for_all(qa)
	{
	  mln_niter(neighb2d) n(F::reg_nbh(), qa);
	  for_all (n)
	    if (!R.has (n))
	      N.insert (n);
	}

      //      debug::println(u);

      //       //std::cout << "A :" << std::endl;
      //       if (A.npoints())
      // 	//debug::println(u | A);
      //       //std::cout << "N :" << std::endl;
      //       if (N.npoints())
      // 	//debug::println(u | N);
      //       //std::cout << "R :" << std::endl;
      //       if (R.npoints())
      // 	//debug::println(u | R);

      // gn <- min u(x) x belongs to N.
      if ((u | set::inter(N, u.domain())).npoints() > 0)
	gn = level::compute< typename F::accu_for_gn >(u | set::inter(N, u.domain()));
      else
      {
	finished = true;
 	gn += F::inc;
      }
      //std::cout << std::endl << "gN = " << gn << std::endl;
      // R <- R union A
      // tag the pixels of A.

      for_all(qa)
	{
	  R.insert(qa);
	  tagged(qa) = true;
	}
      //std::cout << "exiting step 3" << std::endl;
    }


    /// IF g < gn.
    template <typename V, typename P, typename F>
    void step4_1 (image2d<V>& u,
		  set_p<P>& A,
		  set_p<P>& R,
		  set_p<P>& N,
		  V& g,
		  V& gn,
		  fllt_node(P, V)*& current_region,
		  image2d<fllt_node(P, V)*>& regions
		  )
    {
      //std::cout << "entering step 4_1" << std::endl;

      // If the region is bounded
      // Create a new conected component.
      // FIXME : we can make it faster.

      if ((R.bbox() < u.domain()) || (R.npoints() == u.domain().npoints()))
      {
	mln_piter(set_p<P>) p(R);
	current_region = new fllt_node(P, V)();
	current_region->elt().brighter = F::parent_is_brighter;
	current_region->elt().value = g;
	for_all(p)
	  {
	    current_region->elt().points.insert(p);
	    if (regions(p) == 0)
	      regions(p) = current_region;
	    else
	    {
	      if (regions(p)->parent() == 0)
		regions(p)->set_parent(current_region);
	    }
	  }


	// Count the number of conected components of the border of R.
	static image2d<int>  tmp(u.domain().to_larger(1));
	static image2d<bool> border_ima(tmp.domain());
	level::fill(border_ima, false);

	//       level::fill(inplace(border_ima | N), true);
	//       std::cout << "tmp border = " << tmp.border () << std::endl;
	//       std::cout << "ima border = " << border_ima.border () << std::endl;
	mln_piter(set_p<P>) z(N);
	for_all(z)
	  {
	    mln_assertion(border_ima.owns_(z));
	    border_ima(z) = true;
	  }
	unsigned n;
	labeling::level(border_ima, true, F::bdr_nbh(), tmp, n);

	//     debug::println(border_ima);
	//std::cout << "nb composantes :" << n << std::endl;
	//      debug::println(tmp);
	if (n > 1)
	{

	  //   IF number of conected components of the border > 1
	  for (int i = 2; i <= n; i++)
	  {
	    //       follow each border to find which is the exterior border
	    //       and which are the holes. Keep one pixel of each holes.

	    // WARNING : We trust labeling::level to label the exterior border with 1.
	    current_region->elt().holes.insert(a_point_of(tmp | pw::value(tmp) == pw::cst(n)));

	    //       FIXME : [optimisation] Remove from N border of holes???.
	    //       Recompute gn <- min u(x) x belongs to A
	  }
	}

      }
      g = gn;
      //    A <- {x belongs to N / u(x) == g}
      A.clear();
      A = set::uni(A, set::inter(N, u.domain()) | pw::value(u) == pw::cst(g));
      //    N <- N\{x belongs to N / u(x) == g}
      N = set::diff(N, set::inter(N, u.domain()) | pw::value(u) == pw::cst(g));

      //       std::cout << "A :" << std::endl;
      //       if (A.npoints())
      // 	debug::println(u | A);
      //       std::cout << "N :" << std::endl;
      //       if (N.npoints())
      // 	debug::println(u | N);

      //std::cout << "exiting step 4_1" << std::endl;
    }


    /// IF g == gn.
    template <typename V, typename P>
    void step4_2 (const image2d<V>& u,
		  set_p<P>& A,
		  set_p<P>& N,
		  V& g,
		  fllt_node(P, V)* current_region,
		  image2d<fllt_node(P, V)*>& regions
		  )
    {
      //std::cout << "entering step 4_2" << std::endl;

      //    A <- {x belongs to N / u(x) == g}
      A = set::uni(A, set::inter(N, u.domain()) | pw::value(u) == pw::cst(g));
      //    N <- N\{x belongs to N / u(x) == g}
      N = set::diff(N, set::inter(N, u.domain()) | pw::value(u) == pw::cst(g));

      //       std::cout << "A :" << std::endl;
      //       if (A.npoints())
      // 	debug::println(u | A);
      //       std::cout << "N :" << std::endl;
      //       if (N.npoints())
      // 	debug::println(u | N);

      //std::cout << "exiting step 4_2" << std::endl;
    }

    /// IF g > gn.
    template <typename V, typename P>
    void step4_3 (image2d<V>& u,
		  const image2d<bool>& tagged,
		  const set_p<P>& R,
		  const V& g)
    {
      //std::cout << "entering step 4_3" << std::endl;

      //    set the gray-level of the pixels of R to g.
      mln_piter(set_p<P>) p(R);
      for_all(p)
	{
	  mln_assertion (tagged(p));
	  u (p) = g;
	}

      //std::cout << "exiting step 4_3" << std::endl;

    }


    template <typename V, typename F>
    fllt_tree(point2d, V)&
    compute_level_set(const image2d<V>& ima,
		      image2d< fllt_node(point2d, V)* >& regions)
    {
      typedef point2d P;
      typedef image2d<V> I;

      // FIXME: not nice.
      typedef     mln::image_if<
	mln::image2d<V>,
	mln::fun::greater_p2b_expr_<mln::pw::value_<mln::image2d<V> >,
	mln::pw::cst_<int> >
	> I_IF;

      // Check
      mln_assertion(ima.domain() == regions.domain());

      // Declarations.
      set_p<P> R, N, A;
      V g, gn;
      point2d x0;
      image2d<V> min_locals(ima.domain());
      image2d<V> u = clone(ima);
      border::fill(u, 0);

      //std::cout << "image U:" << std::endl;
      //      debug::println_with_border(u);
      image2d<bool> tagged(ima.domain());
      fllt_node(P, V)* current_region;

      // INIT
      R.clear();
      N.clear();
      A.clear();
      g=  0;
      gn = 0;
      current_region = 0;

      level::fill(regions, 0);
      level::fill(tagged, false);

      // Get the locals extremums
      unsigned nlabels;
      F::regional_extremum(ima, F::reg_nbh(), min_locals, nlabels);

      //       debug::println(min_locals);
      //       debug::println(min_locals | (pw::value(min_locals) > pw::cst(0)));

      /// Algorithm.
      {
	// For all locals extremums
	//void* x = min_locals | (pw::value(min_locals) > pw::cst(0));
	I_IF min_locals_list(min_locals | (pw::value(min_locals) > pw::cst(0)));
	mln_piter(I_IF) p(min_locals_list.domain());
	for_all(p)
	  {
	    if (tagged(p))
	      continue;

	    step1(ima, p, g, x0);
	    step2(A, R, N, x0);
	    while (1)
	    {
	      //std::cout << "g = " << g << std::endl;
	      step3<V, P, F>(u, tagged, A, R, N, gn);
	      /// step4.
	      if (F::compare(g, gn))
	      {
		step4_1<V, P, F>(u, A, R, N, g, gn, current_region, regions);
		// GO TO 3)
		continue;
	      }


	      if (g == gn)
	      {
		step4_2(u, A, N, g, current_region, regions);
		// GO TO 3)
		continue;
	      }


	      if (!F::compare(g, gn))
	      {
		step4_3(u, tagged, R, g);
		// GO TO 1)
		break;
	      }
	    }
	    //std::cout << "current_region = " << current_region << std::endl;
	  }
      } // end of Algorithm
      std::cout << "END OF ALGORITHM" << std::endl;

      image2d<value::int_u8> output (ima.domain ());
      fllt_tree(P, V)& tree = *new fllt_tree(P, V)(current_region);
      util::tree_to_image (tree, output);

      util::display_tree(ima, tree);

      //       debug::println(output);
      //       std::cout << std::endl;
      //       debug::println(ima);

      //       if (output != ima)
      //       {
      // 	std::cerr << "BUG!!!" << std::endl;
      // 	abort();
      //       }

      io::pgm::save(output, "out.pgm");
      std::cout << "out.pgm generate"
		<< std::endl;


      //      debug::println(regions);
      //debug::println(ima | regions(make:defined reference to `mln::fllt::lower<mln::value::int_u<8u> >::inc':point2d(-4,-1))->elt().points);

      return (tree);

    } // end of compute_level_set

    //   LOWER LEVEL SET : region = c4, border = c8
    template <typename V>
    struct lower
    {
      static bool
      compare(const V& u, const V& v)
      {
	return u < v;
      }

      template <typename I, typename N, typename O>
      static bool
      regional_extremum(const Image<I>& input, const Neighborhood<N>& nbh,
			Image<O>& output, unsigned& nlabels)
      {
	return labeling::regional_minima(input, nbh,
					 output, nlabels);
      }

      static const int inc = 1;
      static const bool parent_is_brighter = true;
      typedef accu::min accu_for_gn;

      static const neighb2d& bdr_nbh() { return c8(); }
      static const neighb2d& reg_nbh() { return c4(); }
    };



    //   UPPER LEVEL SET : region = c8, border = c4
    template <typename V>
    struct upper
    {
      static bool
      compare(const V& u, const V& v)
      {
	return u > v;
      }

      template <typename I, typename N, typename O>
      static bool
      regional_extremum(const Image<I>& input, const Neighborhood<N>& nbh,
			Image<O>& output, unsigned& nlabels)
      {
	return labeling::regional_maxima(input, nbh,
					 output, nlabels);
      }

      static const int inc = -1;
      static const bool parent_is_brighter = false;
      typedef accu::max accu_for_gn;

      static const neighb2d& bdr_nbh() { return c4(); }
      static const neighb2d& reg_nbh() { return c8(); }
    };

    template <typename P, typename V>
    void
    move_shape(fllt_node(P, V)& node,
	       fllt_node(P, V)& hole,
	       fllt_tree(P, V)& tree,
	       const image2d<fllt_node(P, V)*>& low_reg,
	       const image2d<fllt_node(P, V)*>& upp_reg)
    {
      //     node.elt().points = set::uni(hole.elt().points, node.elt().points);
      node.add_child(&hole);
      fill_a_shape(hole, tree, low_reg, upp_reg);
    }

    template <typename P, typename V>
    fllt_node(P, V)*
    find_the_hole(fllt_node(P, V)& node,
		  const P p,
		  const image2d<fllt_node(P, V)*>& other_reg)
    {
      fllt_node(P, V)* s = other_reg(p);

      mln_assertion(s);
      while (s->parent() && (s->parent()->elt().value < node.elt().value))
      {
	mln_assertion(s);
	s = s->parent();
	mln_assertion(s);
      }
      return s;
    }

    template <typename P, typename V>
    void
    fill_a_shape(fllt_node(P, V)& node,
		 fllt_tree(P, V)& tree,
		 const image2d<fllt_node(P, V)*>& low_reg,
		 const image2d<fllt_node(P, V)*>& upp_reg)
    {
      mln_piter(set_p<P>) p(node.elt().holes);
      for_all(p)
	{
	  std::cout << "OK start loop" << std::endl;
	  bool h = true;

	  fllt_node(P, V)* hole;
	  if (node.elt().brighter)
	    hole = find_the_hole(node, point2d(p), upp_reg);
	  else
	    hole = find_the_hole(node, point2d(p), low_reg);

	  typename fllt_node(P, V)::children_t::iterator it;
	  for (it = node.children().begin();
	       it != node.children().end();
	       it++)
	  {
	    if (set::is_subset_of(hole->elt().points,
 				  (*it)->elt().points))
	    {
	      h = false;
	      break;
	    }
	  }
	  if (h)
	  {
	    move_shape(node, *hole, tree, low_reg, upp_reg);
	    std::cout << "OK" << std::endl;
	  }

	}
    }

    template <typename P, typename V>
    fllt_tree(P, V)
    merge_trees(fllt_tree(P, V)& lower,
		fllt_tree(P, V)& upper,
		const image2d<fllt_node(P, V)*>& low_reg,
		const image2d<fllt_node(P, V)*>& upp_reg,
		const image2d<V>& ima)
    {

      //   In order to merge the trees, we only have to find for each shape S
      //   with a hole H in it whether one of its children has a hole H�
      //   containing H. If it is the case, we do nothing. Otherwise, we
      //   put the shape of the hole H (and all its descendants) as child of
      //   the shape .

      fllt_branch_iter(P, V) p(lower.main_branch());
      for_all(p)
	{
 	  fllt_node(P, V)& n(p);
 	  fill_a_shape(n, lower, low_reg, upp_reg);
	  mln_assertion(lower.check_consistency());
	  mln_assertion(upper.check_consistency());
	}


      {
	fllt_branch_iter(P, V) p(upper.main_branch());
	for_all(p)
	  {
	    fllt_node(P, V)& n(p);
	    fill_a_shape(n, upper, low_reg, upp_reg);
	    mln_assertion(lower.check_consistency());
	    mln_assertion(upper.check_consistency());
	  }
      }


      typename fllt_node(P, V)::children_t::iterator it;
      for (it = upper.root()->children().begin();
      	   it != upper.root()->children().end(); )
      {
      	lower.root()->add_child(*it);
      }
      mln_assertion(lower.check_consistency());
    }


    template <typename P, typename V>
    void
    visualize_deepness(image2d<value::int_u8>& output,
		       fllt_tree(P, V)& tree)
    {
      fllt_branch_iter(P, V) p(tree.main_branch());
      level::fill(output, 0);
      for_all(p)
	{
	  //std::cout << (&*p) << ":" << p.deepness() << std::endl;
	  mln_piter(set_p<point2d>) q((*p).elt().points);
	  for_all(q)
	    {
	      if (output(q) < p.deepness())
		output(q) = p.deepness();
	    }
	}
    }


    template <typename P, typename V>
    void
    visualize_bounds(image2d<value::int_u8>& output,
		     fllt_tree(P, V)& tree,
		     unsigned limit)
    {
      fllt_branch_iter(P, V) p(tree.main_branch());
      level::fill(output, 255);
      for_all(p)
	{
	  if ((*p).elt().points.npoints() > limit)
	  {
	    mln_piter(set_p<point2d>) q((*p).elt().points);
	    for_all(q)
	      {
		mln_niter(neighb2d) n(c4(), q);
		bool is_border = false;
		for_all (n)
		  if (!((*p).elt().points).has (n))
		    is_border = true;
		if (is_border)
		  output(q) = 0;
	      }
	  }
	}
    }

    template <typename P, typename V>
    void
    draw_tree(const image2d<V>& ima,
	      fllt_tree(P, V)& tree)
    {
      fllt_branch_iter(P, V) p(tree.main_branch());
      for_all(p)
      	{
      	  std::cout << "region mere : " << (*p).parent() << std::endl;
      	  std::cout << "               ^" << std::endl;
      	  std::cout << "               |" << std::endl;
      	  std::cout << "region : " << &*p
		    << " value = " << (*p).elt().value << std::endl
		    << " holes : "
		    << (*p).elt().holes.npoints() << std::endl;

      	  debug::println(ima | (*p).elt().points);
      	  std::cout << std::endl;
      	}
    }

    template <typename V>
    // Fixme : return type
    void
    fllt(const image2d<V>& ima)
    {
      typedef point2d P;

      fllt_tree(P, V) upper_tree;
      fllt_tree(P, V) lower_tree;
      image2d<fllt_node(P, V)*> low_reg(ima.domain());
      image2d<fllt_node(P, V)*> upp_reg(ima.domain());

      std::cout << "1/ Compute the lower level set." << std::endl;
      lower_tree = compute_level_set<V, lower<V> >(ima, low_reg);
      draw_tree(ima, lower_tree);
      std::cout << "2/ Compute the upper level set." << std::endl;
      upper_tree = compute_level_set<V, upper<V> >(ima, upp_reg);

      draw_tree(ima, upper_tree);

      std::cout << "3/ Merge the two trees." << std::endl;
      fllt_tree(P, V) result_tree = merge_trees(lower_tree, upper_tree, low_reg, upp_reg, ima);


      std::cout << "4/ Generate outputs." << std::endl;

      image2d<value::int_u8> output (ima.domain ());
      util::tree_to_image (lower_tree, output);

//             if (output != ima)
//             {
//       	std::cerr << "BUG!!!" << std::endl;
//       	abort();
//             }


      //       io::pgm::save(output, "out_final.pgm");
      //       std::cout << "out_final.pgm generate"
      // 		<< std::endl;


      //      util::display_tree(ima, lower_tree);
      draw_tree(ima, lower_tree);
      //       image2d<value::int_u8> viz(ima.domain());
      //       image2d<value::int_u8> viz2(ima.domain());

      //       visualize_deepness(viz, lower_tree);
      //       level::stretch(viz, viz2);
      //       debug::println(viz);
      //       debug::println(viz2);
      //       io::pgm::save(viz2, "fllt.pgm");

      //       visualize_bounds(viz, lower_tree, 2000);
      //       debug::println(viz);
      //       io::pgm::save(viz, "fllt_bounds.pgm");
    }

  } // end of namespace mln::fllt

} // end of namespace mln



#endif // ! MLN_FIXME_FLLT_HH
