#include "NumbersGenerator.h"
#include "Generator.h"

NumbersGenerator::NumbersGenerator(long seed)
    : engine_{static_cast<unsigned long>(seed)}
{
}

int NumbersGenerator::getInt(int low, int high)
{
    std::uniform_int_distribution dist{low, high};
    return dist(engine_);
}
