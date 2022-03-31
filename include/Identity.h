#pragma once

#include "Operation.h"

#include <memory>

// Represents the identity operation
// Returns the same set that it gets as input
// Used as the leaf in every operation tree
class Identity : public Operation
{
public:
    Set compute(const std::vector<Set>& inputs) const override;
    void print(std::ostream& ostr, NameGenerator& gen) const override;
    void print(std::ostream& ostr, const std::vector<Set>& inputs) const override;
    int inputCount() const override { return 1; }

};
