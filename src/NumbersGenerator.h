#pragma once

#include <random>

#include "Generator.h"

class NumbersGenerator : public Generator
{
public:
    NumbersGenerator();

    int getInt(int low, int high) override;

private:
    std::mt19937 engine_;
};
