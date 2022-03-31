#include "NonCompBinaryOperation.h"

#include <iostream>

void NonCompBinaryOperation::print(std::ostream& ostr, NameGenerator& gen) const
{
    ostr << '(';
    first()->print(ostr, gen);
    ostr << ' ';
    printSymbol(ostr);
    ostr << ' ';
    second()->print(ostr, gen);
    ostr << ')';
}

void NonCompBinaryOperation::print(std::ostream& ostr, const std::vector<Set>& inputs) const
{
    ostr << '(';
    first()->print(ostr, inputsForFirstOperation(inputs));
    ostr << ' ';
    printSymbol(ostr);
    ostr << ' ';
    second()->print(ostr, inputsForSecondOperation(inputs));
    ostr << ')';
}

std::vector<Set> NonCompBinaryOperation::inputsForFirstOperation(const std::vector<Set>& inputs) const
{
    return std::vector<Set>(inputs.begin(), inputs.begin() + first()->inputCount());
}

std::vector<Set> NonCompBinaryOperation::inputsForSecondOperation(const std::vector<Set>& inputs) const
{
    return std::vector<Set>(inputs.begin() + first()->inputCount(), inputs.end());
}
