#include "Union.h"

#include <algorithm>
#include <iostream>
#include <iterator>

Set Union::compute(const std::vector<Set>& inputs) const
{
    const auto a = first()->compute(inputsForFirstOperation(inputs));
    const auto b = second()->compute(inputsForSecondOperation(inputs));

    auto result = std::vector<int>();
    std::ranges::set_union(a, b, std::back_inserter(result));
    return Set(std::move(result));
}

void Union::printSymbol(std::ostream& ostr) const
{
    ostr << 'U';
}
