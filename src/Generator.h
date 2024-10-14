#pragma once

class Generator
{
public:
    Generator() = default;
    virtual ~Generator() = default;

    Generator(const Generator&) = delete;
    Generator(Generator&&) = delete;
    Generator& operator=(const Generator&) = delete;
    Generator& operator=(Generator&&) = delete;

    virtual int getInt(int low, int high) = 0;
};
