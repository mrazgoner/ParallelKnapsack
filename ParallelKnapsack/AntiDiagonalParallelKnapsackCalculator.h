#pragma once

#include "DynamicKnapsackCalculator.h"
#include <functional>

class AntiDiagonalParallelKnapsackCalculator : public DynamicKnapsackCalculator
{
public:
    using DynamicKnapsackCalculator::DynamicKnapsackCalculator;
    ~AntiDiagonalParallelKnapsackCalculator() = default;

    void traverse(const std::function<void(int, int)>& worker) override;
};