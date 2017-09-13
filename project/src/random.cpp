#ifndef _random_cpp_
#define _random_cpp_

#include <cstdlib>
#include <ctime>
#include "random.hpp"

void Random::init_all() {
  srand (static_cast <unsigned> (time(0)));
}

float Random::real(float LO, float HI) {
  return LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
}


#endif
