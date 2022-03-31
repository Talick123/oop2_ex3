#pragma once

#include "Operation.h"

#include <memory>

class BinaryOperation : public Operation
{
public:
    BinaryOperation(const std::shared_ptr<Operation>& arg1, const std::shared_ptr<Operation>& arg2);

protected:
    const std::shared_ptr<Operation>& first() const { return m_first; }
    const std::shared_ptr<Operation>& second() const { return m_second; }

private:
    const std::shared_ptr<Operation> m_first;
    const std::shared_ptr<Operation> m_second;
};
