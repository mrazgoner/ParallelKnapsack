#pragma once
#include "DynamicKnapsackCalculator.h"
class ParallelKnapsackCalculator :
    public DynamicKnapsackCalculator
{
   public:
	   using DynamicKnapsackCalculator::DynamicKnapsackCalculator;
	   void traverse(const std::function<void(int, int)>& worker) override;
};

