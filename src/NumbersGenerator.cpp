#include "NumbersGenerator.h"

int NumbersGenerator::getInt(int low, int high)
{
    std::uniform_int_distribution dist{low, high};
    return dist(engine_);
}
