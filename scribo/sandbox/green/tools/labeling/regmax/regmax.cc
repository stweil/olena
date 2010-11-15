// Copyright (C) 2007, 2008, 2009, 2010 EPITA LRDE
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
///
/// \brief This tool aimed at segmenting the histogram.
///
/// The current segmentation of the histogram is done by finding the
/// regional maxima. Regional maxima are isolated small regions inside
/// the background (label 0).

#include <iostream>
#include <fstream>
#include <boost/format.hpp>

#include <mln/accu/stat/histo3d_rgb.hh>
#include <mln/accu/stat/mean.hh>

#include <mln/algebra/vec.hh>

#include <mln/core/macros.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/data/compute.hh>

#include <mln/debug/println.hh>
#include <mln/display/display_histo.hh>

#include <mln/io/dump/load.hh>
#include <mln/io/dump/save.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/labeling/regional_maxima.hh>
#include <mln/labeling/compute.hh>
#include <mln/labeling/mean_values.hh>

#include <mln/literal/colors.hh>

#include <mln/morpho/opening/volume.hh>

#include <mln/value/label_8.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

#include <mln/util/array.hh>

/// \ brief Compute the 2d labeled image.
///
///  Compute the 2d labeled image with the 2d labeled histogram and
///  the input image.
///
/// \fixme Need to put it in mln library. Nothing to do here.
template <unsigned n>
struct t_labeling_rgbn : mln::Function_v2v< t_labeling_rgbn<n> >
{
  typedef mln::value::rgb<n>    t_rgbn;
  typedef mln::value::label_8   t_lbl8;
  typedef t_rgbn                argument;
  typedef t_lbl8                result;
  typedef mln::image3d<t_lbl8>  t_label;

  const t_label& _label;

  t_labeling_rgbn(const t_label& label) : _label(label) {}

  result operator()(const argument& c) const
  {
    t_lbl8 tmp = mln::opt::at(_label, c.blue(), c.red(), c.green());

    return tmp;
  }
};


/// \brief Compute and dump stats.
///
/// \param[in] i_input_rgb8 the 2d rgb input image.
/// \param[in] l_input_lbl8 the 2d labeled image.
/// \param[in] h_histo_rgbn the quantified histogram.
/// \param[in] l_histo_lbl8 the labeled quantified histogram.
/// \param[in] n_labels the number of labels (number of regions).
/// \param[in] the name of the logging file for the stats.
///
/// Every segmented color has got it's own label. For each label, we
/// compute the mean in the r/g/b channels, the total of pixels, the
/// percentage of pixels in the image and the percentage without counting
/// the background (label 0).
void compute_stats(const mln::image2d<mln::value::rgb8>&    i_input_rgb8,
		   const mln::image2d<mln::value::label_8>& l_input_lbl8,
		   const mln::image3d<unsigned>&            h_histo_rgbn,
		   const mln::image3d<mln::value::label_8>& l_histo_lbl8,
		   const mln::value::label_8&               n_labels,
		   const std::string&                       log)
{
  typedef mln::algebra::vec<3,float>                        t_vec3f;
  typedef mln::accu::math::sum<unsigned,unsigned>           t_sum;
  typedef mln::accu::stat::mean<t_vec3f,t_vec3f,t_vec3f>    t_mean;
  typedef mln::util::array<unsigned>                        t_count_array;
  typedef mln::util::array<t_vec3f>                         t_mean_array;

  mln::util::array<float>  abs((unsigned)(n_labels)+1);
  mln::util::array<float>  rel((unsigned)(n_labels)+1);
  unsigned                 nb = 0;

  for (unsigned i = 0; i <= n_labels; ++i)
  {
    abs[i] = 0.0;
    rel[i] = 0.0;
  }

  // COMPUTE THE SUM
  t_count_array count = mln::labeling::compute(t_sum(),
					       h_histo_rgbn,
					       l_histo_lbl8,
					       n_labels);

  // COMPUTE THE TOTAL
  for (unsigned i = 0; i <= n_labels; ++i)
  {
    unsigned c = count[i];
    nb += c;
  }

  // COMPUTE THE PERCENTAGES
  for (unsigned i = 0; i <= n_labels; ++i)
    if (0 < count[i])
    {
      abs[i] = ((float)count[i] / nb)*100.0;
      rel[i] = ((float)count[i] / (nb - count[0]))*100.0;
    }

  // COMPUTE THE MEAN

  t_mean_array mean = mln::labeling::compute(t_mean(),
					     i_input_rgb8,
					     l_input_lbl8,
					     n_labels);

  // CORRECT 0 LABEL STATS
  rel[0] = 0;
  mean[0][0] = 255.0;
  mean[0][1] = 255.0;
  mean[0][2] = 0.0;

  // PRINT STATS
  std::ofstream log_stream(log.c_str());

  for (unsigned i = 0; i <= n_labels; ++i)
  {
    const t_vec3f& mean_v = mean[i];

    log_stream << boost::format("%2i|"
				"r = %6.2f, g = %6.2f, b = %6.2f |"
				"c = %7i, %%i = %5.2f, %%c = %5.2f")
      % i
      % mean_v[0]
      % mean_v[1]
      % mean_v[2]
      % count[i]
      % abs[i]
      % rel[i]
	      << std::endl;
  }

  log_stream << std::endl << std::endl;
  log_stream.flush();
  log_stream.close();
}



/// \brief Dump the colormap.
///
/// \param[in] m2_label the internal representation of the colormap.
/// \param[in] colormap the name of the colormap in the filesystem.
///
/// Take an dynamic array of 3d vector which represents the colormap.
/// Use boost formatting to dump it properly.
void save(mln::util::array< mln::algebra::vec<3,float> >& m2_label,
	  const char                                      *colormap)
{
  typedef mln::algebra::vec<3,float> t_vec3f;
  typedef mln::util::array<t_vec3f>  t_mean_array;

  std::ofstream stream(colormap);

  for (unsigned i = 0; i < m2_label.size(); ++i)
  {
    const t_vec3f& mean_v = m2_label[i];

    stream << boost::format("%2i | r = %6.2f, g = %6.2f, b = %6.2f")
      % i
      % mean_v[0]
      % mean_v[1]
      % mean_v[2]
	   << std::endl;
  }

  stream.flush();
  stream.close();
}


/// \brief The regmax image processing chain.
///
/// \param[in] input  the name of the RGB input image.
/// \param[in] quant  the name of quantified RGB input image.
/// \param[in] histo  the name of the 3d RGB histogram to filter.
/// \param[in] opened the name of 3d histogram.
/// \param[in] nbh    the neighbouring (c6|c18|c26).
/// \param[out] labeled  the 3d labeled histogram.
/// \param[out] proj     the red/green projection of the segmented histogram.
/// \param[out] colormap the name of the colormap produced.
/// \param[out] mean     the name of the mean rebuilded image.
/// \param[out] stats    the name of the statistics (optional).
///
/// This is the core routine of this file. First every input file
/// (images or histograms) are loaded. Then regional labeling is
/// done. And finally, every colormap dumping, stats saving,
/// projection is done.
template <unsigned n>
void mk_regmax(const std::string&   input,   // in
	       const std::string&   quant,   // in
	       const std::string&   histo,   // in
	       const std::string&   opened,  // in
	       const mln::neighb3d& nbh,     // in
	       const std::string&   labeled, // out
	       const std::string&   proj,    // out
	       const std::string&   colormap,// out
	       const std::string&   mean,    // out
	       const std::string&   stats)   // [out]
{
  typedef mln::value::label_8                         t_lbl8;
  typedef mln::value::rgb8                            t_rgb8;
  typedef mln::value::rgb<n>                          t_rgbn;
  typedef mln::value::int_u8                          t_int_u8;
  typedef mln::value::int_u<n>                        t_int_un;
  typedef mln::algebra::vec<3,float>                  t_v3f;
  typedef mln::image2d<t_int_u8>                      t_image2d_int_u8;
  typedef mln::image2d<t_int_un>                      t_image2d_int_un;
  typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
  typedef mln::image2d<t_rgbn>                        t_image2d_rgbn;
  typedef mln::image3d<t_lbl8>                        t_image3d_lbl8;
  typedef mln::image2d<t_lbl8>                        t_image2d_lbl8;
  typedef mln::image3d<unsigned>                      t_histo3d;
  typedef mln::accu::meta::stat::histo3d_rgb          t_histo3d_fun;
  typedef mln::accu::stat::mean<t_v3f,t_v3f,t_v3f>    t_mean;
  typedef mln::util::array<t_v3f>                     t_mean_array;

  t_image2d_rgb8                                      i0_input; // input img
  t_image2d_rgbn                                      i1_input; // quant img
  t_histo3d                                           h1_input; // input histo
  t_histo3d                                           h2_input; // opened histo
  t_image2d_rgb8                                      p2_label; // histo proj
  t_image3d_lbl8                                      l2_histo; // label histo
  t_image2d_lbl8                                      l2_input; // label input
  t_mean_array                                        m2_label; // colormap
  t_image2d_rgb8                                      i2_mean;  // reconstructed

  t_lbl8                                              n_lbl;    // nb class
  t_rgb8                                              red(mln::literal::red);


  // BEGIN LOADING
  mln::io::ppm::load(i0_input, input.c_str());
  mln::io::ppm::load(i1_input, quant.c_str());
  mln::io::dump::load(h1_input, histo.c_str());
  mln::io::dump::load(h2_input, opened.c_str());
  // END LOADING

  // BEGIN IMAGE PROCESSING
  l2_histo  = mln::labeling::regional_maxima(h2_input, nbh, n_lbl);
  // END IMAGE PROCESSING

  // BEGIN SAVING
  mln::io::dump::save(l2_histo, labeled.c_str());

  l2_input = mln::data::transform(i1_input, t_labeling_rgbn<n>(l2_histo));
  i2_mean  = mln::labeling::mean_values(i0_input, l2_input, n_lbl);
  m2_label = mln::labeling::compute(t_mean(), i0_input, l2_input, n_lbl);

  // CORRECT 0 LABEL STATS
  m2_label[0][0] = 255.0;
  m2_label[0][1] = 255.0;
  m2_label[0][2] = 0.0;

  p2_label =mln::display::display3_histo3d_unsigned<n>(h1_input,
						       l2_histo,
						       m2_label,
						       red);

  mln::io::ppm::save(p2_label, proj.c_str());
  save(m2_label, colormap.c_str());
  mln::io::ppm::save(i2_mean, mean.c_str());

  if (0 < stats.size())
    compute_stats(i0_input, l2_input, h1_input, l2_histo, n_lbl, stats);

  // END SAVING
}


/// \brief The online documentation of the tool.
///
/// Print the documentation of the tool with the meaning of the
/// different parameters.
void usage()
{
  std::cout                                                       << std::endl;
  std::cout << "regmax input.ppm q quant.ppm histo.dump"
	    << " opened.dump nbh labeled.dump proj.ppm"
	    << " colormap.txt mean.ppm [stats.txt]"               << std::endl;
  std::cout << std::endl;
  std::cout << "where :"                                          << std::endl;
  std::cout << "* [ in] input.ppm is the 8 bits color ppm image"  << std::endl;
  std::cout << "* [ in] q is the degree of quantification"
	    << " {2,3,4,5,6,7,8}"                                 << std::endl;
  std::cout << "* [ in] quant.ppm is the q bits quantified input"
	    << " image"                                           << std::endl;
  std::cout << "* [ in] histo.dump is the quantified color"
	    << " histogram"                                       << std::endl;
  std::cout << "* [ in] opened.dump is the filtered histogram"    << std::endl;
  std::cout << "* [ in] nbh is the 3d neighbourhood {6,18,26}"    << std::endl;
  std::cout << "* [out] labeled.dump is the labeled 3d histogram" << std::endl;
  std::cout << "* [out] proj.ppm is the r/g projection of the"
	    << " histogram with maxima label plot on"             << std::endl;
  std::cout << "* [out] colormap.txt is the colormap for labels"  << std::endl;
  std::cout << "* [out] mean.ppm is the mean reconstructed image" << std::endl;
  std::cout << "* [out] stats.txt is the statistical label report"<< std::endl;
  std::cout << std::endl;
}


/// \brief The main entry of the software.
///
/// \param[in] input.ppm the name of the RGB input image.
/// \param[in] q the quantification previously used.
/// \param[in] quant.ppm the name of quantified RGB input image.
/// \param[in] histo.dump the name of the 3d RGB histogram to filter.
/// \param[in] opened.dump the name of 3d histogram.
/// \param[in] nbh the code for the neighbouring (6|18|26).
/// \param[out] labeled.dump the 3d labeled histogram.
/// \param[out] proj.ppm the red/green projection of the segmented histogram.
/// \param[out] colormap.txt the name of the colormap produced.
/// \param[out] mean.ppm the name of the mean rebuilded image.
/// \param[out] stats.txt the name of the statistics file (optional).
///
/// This is the front hand for the image processing routine. The goal of this
/// chain is to segment the 3d histogram.
int main(int argc, char* args[])
{
  if (11 == argc || 12 == argc)
  {
    const std::string input(args[1]);   // in
    const char        q = args[2][0];   // in
    const std::string quant(args[3]);   // in
    const std::string histo(args[4]);   // in
    const std::string opened(args[5]);  // in
    const char        nbh = args[6][0]; // in
    const std::string labeled(args[7]); // out
    const std::string proj(args[8]);    // out
    const std::string colormap(args[9]);// out
    const std::string mean(args[10]);    // out
    const std::string stats(12 == argc? args[11] : "");   // [out]


    mln::neighb3d neighbourhood;

    switch (nbh)
    {
      case '6': neighbourhood = mln::c6();  break;
      case '1': neighbourhood = mln::c18(); break;
      case '2': neighbourhood = mln::c26(); break;
      default:  usage(); return 0; // force usage and quit
    }

    switch (q)
    {

      case '2': mk_regmax<2>(input,quant,histo,opened,neighbourhood,
			     labeled,proj,colormap,mean,stats); break;
      case '3': mk_regmax<3>(input,quant,histo,opened,neighbourhood,
			     labeled,proj,colormap,mean,stats); break;
      case '4': mk_regmax<4>(input,quant,histo,opened,neighbourhood,
			     labeled,proj,colormap,mean,stats); break;
      case '5': mk_regmax<5>(input,quant,histo,opened,neighbourhood,
			     labeled,proj,colormap,mean,stats); break;
      case '6': mk_regmax<6>(input,quant,histo,opened,neighbourhood,
			     labeled,proj,colormap,mean,stats); break;
      case '7': mk_regmax<7>(input,quant,histo,opened,neighbourhood,
			     labeled,proj,colormap,mean,stats); break;
      case '8': mk_regmax<8>(input,quant,histo,opened,neighbourhood,
			     labeled,proj,colormap,mean,stats); break;
      default: usage(); break;
    }
  }
  else
    usage();

  return 0;
}