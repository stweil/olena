#include <mln/core/image/image2d.hh>
#include <mln/core/image/disk_ima.hh>

#include <mln/io/ppm/load.hh>
#include <mln/literal/colors.hh>
#include <mln/value/rgb8.hh>

#include <mln/debug/quiet.hh>

#include "bench.hh"


using namespace mln;
using value::rgb8;


int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " input" << std::endl;
    return 1;
  }

  disk_ima<rgb8> ima;

  io::ppm::load(ima, argv[1]);

  bench_disk_perf(ima);

  return 0;
}