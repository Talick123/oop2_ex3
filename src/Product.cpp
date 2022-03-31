#include "Product.h"

#include <iostream>

Set Product::compute(const std::vector<Set>& inputs) const
{
    const auto a = first()->compute(inputsForFirstOperation(inputs));
    const auto b = second()->compute(inputsForSecondOperation(inputs));

    auto result = std::vector<int>();
    for (const auto& itemA : a)
    {
        for (const auto& itemB : b)
        {
            result.push_back(itemA * itemB);
        }
    }

    return Set(std::move(result));
}

void Product::printSymbol(std::ostream& ostr) const
{
    ostr << '*';
}
