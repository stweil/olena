#include <mln/core/image/image2d.hh>
#include <scribo/canvas/integral_browsing.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/util/couple.hh>
#include <mln/util/timer.hh>
#include <scribo/binarization/internal/sauvola_formula.hh>

namespace mln
{

  template <typename I>
  struct sauvola_fast_functor
  {

    const I& input;
    typedef mln_ch_value(I,bool) bin_t;
    bin_t bin;

    image2d<double> th_;

    const mln_value(I)* pi;
    bool* po;

    double K_;
    double R_;

    scribo::binarization::internal::sauvola_formula formula_;

    int step_;
    unsigned next_line1;
    unsigned next_line2;
    unsigned next_line3;
    unsigned offset1;
    unsigned offset2;

    sauvola_fast_functor(const I& input_, mln_ch_value(I,bool)& bin_, double K, double R)
      : input(input_),
	bin(bin_),
	pi(&input(input.domain().pmin())),
	po(&bin(bin.domain().pmin())),
	K_(K),
	R_(R)
    {
      step_ = 3;
      next_line1 = 2 * input.border();
      next_line2 = input.delta_index(dpoint2d(+1,0)) + next_line1;
      next_line3 = input.delta_index(dpoint2d(+2,0)) + next_line1;

      offset1 = input.delta_index(dpoint2d(+1,0));
      offset2 = input.delta_index(dpoint2d(+2,0));
    }

    void exec(double mean, double stddev)
    {
      static point2d p(0,0);

      double th = formula_(p, mean, stddev, K_, R_);

      for (int i = 0; i < step_; ++i, ++po, ++pi)
      {
	*po = (*pi <= th);
	*(po + offset1) = (*(pi + offset1) <= th);
	*(po + offset2) = (*(pi + offset2) <= th);
      }
    }

    void end_of_row(int)
    {
      po += next_line3;
      pi += next_line3;
    }


    void finalize()
    {
    }

  };




  template <typename I, typename J>
  void
  init_integral(const Image<I>& input_,
  		Image<J>& integral_sum_sum_2_)
  {
    trace::entering("subsampling::impl::integral_3");

    const I& input = exact(input_);
    J& integral_sum_sum_2 = exact(integral_sum_sum_2_);

    mln_precondition(input.is_valid());
    mln_precondition(input.domain().pmin() == literal::origin);

    typedef mln_value(I) V;
    typedef mln_sum(V) S;
    typedef mln_value(J) V2;
    typedef mln_site(I) P;

    initialize(integral_sum_sum_2, input);
    V2* p_integ = integral_sum_sum_2.buffer();

    const int up = input.delta_index(dpoint2d(-1, 0));

    const unsigned nrows = input.domain().nrows();
    const unsigned ncols = input.domain().ncols();

    unsigned row = 0;

    unsigned b_offset = input.delta_index(dpoint2d(input.border(),
  						   input.border()));
    p_integ += b_offset;
    {
      S h_sum = 0, h_sum_2 = 0;
      const V* ptr1 = & input.at_(row, 0);
      for (unsigned col = 0; col < ncols; ++col)
      {
  	V v = *ptr1++;

  	h_sum   += v;
  	h_sum_2 += v * v;

  	// exception
  	p_integ->first() = h_sum;
  	p_integ->second() = h_sum_2;

  	++p_integ;
      }
    }

    unsigned b_next = 2 * input.border();

    p_integ += b_next;

    for (row += 1; row < nrows; ++row)
    {
      S h_sum = 0, h_sum_2 = 0;
      const V* ptr1 = & input.at_(row, 0);
      for (unsigned col = 0; col < ncols; ++col)
      {
  	V v = *ptr1++;

  	h_sum   += v;
  	h_sum_2 += v * v;

  	p_integ->first() = h_sum + (p_integ + up)->first();
  	p_integ->second() = h_sum_2 + (p_integ + up)->second();

  	++p_integ;
      }

      p_integ += b_next;
    }

    trace::exiting("subsampling::impl::integral_3");
  }



  image2d<bool>
  sauvola_fast(const image2d<value::int_u8>& input, unsigned win)
  {
    util::timer t;
    t.start();
    image2d<util::couple<double,double> > integral;
    init_integral(input, integral);
    t.stop();
    std::cout << "image integrale - " << t << std::endl;

    t.restart();
    image2d<bool> output;
    initialize(output, input);
    sauvola_fast_functor<image2d<value::int_u8> > f(input, output, SCRIBO_DEFAULT_SAUVOLA_K, SCRIBO_DEFAULT_SAUVOLA_R);
    scribo::canvas::integral_browsing(integral, 3, win, win, 1, f);
    t.stop();
    std::cout << "Binarization - " << t << std::endl;

    return f.bin;
  }

}


int main(int argc, char *argv[])
{
  using namespace mln;

  image2d<value::int_u8> input;
  io::pgm::load(input, argv[1]);

  unsigned win = atoi(argv[2]);

  util::timer t;
  t.start();
  image2d<bool> output = sauvola_fast(input, win);
  t.stop();
  std::cout << t << std::endl;

  io::pbm::save(output, argv[3]);
}
