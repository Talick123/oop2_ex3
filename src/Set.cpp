#include "Set.h"

#include <algorithm>
#include <iostream>

namespace rng = std::ranges;

Set::Set(std::istream& istr)
{
    auto n = 0;
    istr >> n;
    for (auto i = 0; i < n; ++i)
    {
        auto num = 0;
        istr >> num;
        m_items.push_back(num);
    }
    makeSet();
}

void Set::makeSet()
{
    // The result of product isn't necessarily sorted
    // e.g. {1, 2} * {1, 2, 3} = {1, 2, 3, 2, 4, 6}
    // and so is the input from the user
    rng::sort(m_items);

    // The same example shows it isn't necessarily only unique values
    // unique() algorithm takes a sorted range and makes sure only unique values left
    // at its start. It returns the point of the new end of the range, and from there
    // we have to erase the tail of the range ourselves
    auto [newEnd, end] = rng::unique(m_items);
    m_items.erase(newEnd, end);

}

std::ostream& operator<<(std::ostream& ostr, const Set& set)
{
    auto first = true;

    ostr << '{';
    for (const auto& item : set)
    {
        if (!first)
        {
            ostr << ',';
        }
        first = false;
        ostr << ' ' << item;
    }
    ostr << " }";

    return ostr;
}
