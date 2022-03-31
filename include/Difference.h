#pragma once

#include "NonCompBinaryOperation.h"

#include <memory>

class Difference : public NonCompBinaryOperation
{
public:
    using NonCompBinaryOperation::NonCompBinaryOperation;

    Set compute(const std::vector<Set>& inputs) const override;
    void printSymbol(std::ostream& ostr) const override;
};
