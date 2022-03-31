#include "Comp.h"

#include <iostream>

Set Comp::compute(const std::vector<Set>& inputs) const
{
    const auto resultOfFirst = first()->compute(inputsForFirstOperation(inputs));
    return second()->compute(inputsForSecondOperation(std::move(resultOfFirst), inputs));
}

void Comp::print(std::ostream& ostr, NameGenerator& gen) const
{
    first()->print(ostr, gen);
    ostr << " -> ";
    second()->print(ostr, gen);
}

void Comp::print(std::ostream& ostr, const std::vector<Set>& inputs) const
{
    const auto inputsForFirst = inputsForFirstOperation(inputs);
    first()->print(ostr, inputsForFirst);
    ostr << " -> ";
    second()->print(ostr, inputsForSecondOperation(first()->compute(inputsForFirst), inputs));
}

std::vector<Set> Comp::inputsForFirstOperation(const std::vector<Set>& inputs) const
{
    return std::vector<Set>(inputs.begin(), inputs.begin() + first()->inputCount());
}

std::vector<Set> Comp::inputsForSecondOperation(Set resultOfFirst, const std::vector<Set>& inputs) const
{
    auto result = std::vector<Set> { resultOfFirst };
    result.insert(result.end(), inputs.begin() + first()->inputCount(), inputs.end());
    return result;
}
