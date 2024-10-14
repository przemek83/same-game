#include "NumbersGenerator.h"

#include <chrono>

NumbersGenerator::NumbersGenerator()
    : engine_{static_cast<unsigned long>(
          std::chrono::system_clock::now().time_since_epoch().count())}
{
}

int NumbersGenerator::getInt(int low, int high)
{
    std::uniform_int_distribution dist{low, high};
    return dist(engine_);
}
