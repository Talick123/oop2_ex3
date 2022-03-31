#pragma once

#include "BinaryOperation.h"

#include <memory>

class Comp : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;

    int inputCount() const override { return first()->inputCount() + second()->inputCount() - 1; }

    Set compute(const std::vector<Set>& inputs) const override;
    void print(std::ostream& ostr, NameGenerator& gen) const override;
    void print(std::ostream& ostr, const std::vector<Set>& inputs) const override;

private:
    std::vector<Set> inputsForFirstOperation(const std::vector<Set>& inputs) const;
    std::vector<Set> inputsForSecondOperation(Set resultOfFirst, const std::vector<Set>& inputs) const;
};
