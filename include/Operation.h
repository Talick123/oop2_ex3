#pragma once

#include "NameGenerator.h"
#include "Set.h"

#include <iosfwd>

// Represents an operation on sets
class Operation
{
public:
    virtual ~Operation() = default;

    // Return the number of inputs (the range size) expected by compute()
    virtual int inputCount() const = 0;

    // Computes the resulted set
    // Assumes the size of the input range is the same as returned by inputCount()
    virtual Set compute(const std::vector<Set>& inputs) const = 0;

    // Prints the operation with generic name for the sets or with the actual input arguments
    virtual void print(std::ostream& ostr, NameGenerator& gen) const = 0;
    virtual void print(std::ostream& ostr, const std::vector<Set>& inputs) const = 0;
};
