#include "util.h"

template<>
int Util::random(int range_from, int range_to) {
	std::random_device                  rand_dev;
	std::mt19937                        generator(rand_dev());
	std::uniform_int_distribution<int>    distr(range_from, range_to);
	return distr(generator);
}
