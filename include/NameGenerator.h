#pragma once

#include <utility>

// Generates set names, A-Z
const char FIRST = 'A';
class NameGenerator
{
public:
    // Generates the next name
    char generate()
    {
        m_current = ((m_current - FIRST) % 26) + FIRST;
        return m_current++;
    }

private:
    char m_current = FIRST;
};
