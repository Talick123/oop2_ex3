#include "Intersection.h"

#include <algorithm>
#include <iostream>
#include <iterator>

Set Intersection::compute(const std::vector<Set>& inputs) const
{
    const auto a = first()->compute(inputsForFirstOperation(inputs));
    const auto b = second()->compute(inputsForSecondOperation(inputs));

    auto result = std::vector<int>();
    std::ranges::set_intersection(a, b, std::back_inserter(result));
    return Set(std::move(result));
}

void Intersection::printSymbol(std::ostream& ostr) const
{
    ostr << '^';
}
