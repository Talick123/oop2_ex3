#pragma once

#include "BinaryOperation.h"

// Represents a binary operation that it isn't composition
// They have similar print format and same implementation for inputCount()
class NonCompBinaryOperation : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;

    int inputCount() const override
    {
        return first()->inputCount() + second()->inputCount();
    }

    virtual void printSymbol(std::ostream& ostr) const = 0;
    void print(std::ostream& ostr, NameGenerator& gen) const override;
    void print(std::ostream& ostr, const std::vector<Set>& inputs) const override;

protected:
    std::vector<Set> inputsForFirstOperation(const std::vector<Set>& inputs) const;
    std::vector<Set> inputsForSecondOperation(const std::vector<Set>& inputs) const;
};
