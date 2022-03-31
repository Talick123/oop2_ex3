#include "Difference.h"

#include <algorithm>
#include <iostream>
#include <iterator>

Set Difference::compute(const std::vector<Set>& inputs) const
{
    const auto a = first()->compute(inputsForFirstOperation(inputs));
    const auto b = second()->compute(inputsForSecondOperation(inputs));

    auto result = std::vector<int>();
    std::ranges::set_difference(a, b, std::back_inserter(result));
    return Set(std::move(result));
}

void Difference::printSymbol(std::ostream& ostr) const
{
    ostr << '-';
}
