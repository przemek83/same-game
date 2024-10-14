#pragma once

#include <random>

#include "Generator.h"

class NumbersGenerator : public Generator
{
public:
    explicit NumbersGenerator(long seed);

    int getInt(int low, int high) override;

private:
    std::mt19937 engine_;
};
