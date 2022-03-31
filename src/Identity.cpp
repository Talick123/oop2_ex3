#include "Identity.h"

#include <iostream>

Set Identity::compute(const std::vector<Set>& inputs) const
{
    return inputs.front();
}

void Identity::print(std::ostream& ostr, NameGenerator& gen) const
{
    ostr << gen.generate();
}

void Identity::print(std::ostream& ostr, const std::vector<Set>& inputs) const
{
    ostr << inputs.front();
}
