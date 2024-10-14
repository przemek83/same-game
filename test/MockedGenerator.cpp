#include "MockedGenerator.h"

#include <cmath>

int MockedGenerator::getInt(int low, int high)
{
    // Following linear congruential generator algorithm described in
    // https://thompsonsed.co.uk/random-number-generators-for-c-performance-tested

    x_ = ((multiplier_ * x_) + increment_) % modulus_;
    return low + static_cast<int>(x_ % (high - low + 1));
}
