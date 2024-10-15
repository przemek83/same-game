#pragma once

#include <chrono>
#include <random>

#include "Generator.h"

class NumbersGenerator : public Generator
{
public:
    int getInt(int low, int high) override;

private:
    std::mt19937 engine_{static_cast<unsigned long>(
        std::chrono::system_clock::now().time_since_epoch().count())};
};
