#include "BinaryOperation.h"

BinaryOperation::BinaryOperation(const std::shared_ptr<Operation>& first, const std::shared_ptr<Operation>& second)
    : m_first(first), m_second(second)
{
}
