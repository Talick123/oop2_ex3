#pragma once

#include <utility>

// Generates set names, A-Z
class NameGenerator
{
public:
    // Generates the next name
    char generate()
    {
        return m_current++;
    }

private:
    char m_current = 'A';
};
