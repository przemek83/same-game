#pragma once

#include <cmath>
#include <cstdint>

#include <src/Generator.h>

class MockedGenerator : public Generator
{
public:
    int getInt(int low, int high) override;

private:
    static constexpr uint64_t modulus_{4'294'967'296};
    static constexpr uint64_t multiplier_{1'664'525};
    static constexpr uint64_t increment_{1'013'904'223};

    unsigned long int x_{12};
};
