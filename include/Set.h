#pragma once

#include <vector>
#include <iosfwd>

// Represents a (mathematical) set of integers or tuple of integers
class Set
{
public:
    // Creates a set from vector of set items
    // Takes it by-value on purpose, because we copy it in anyway, so we want
    // to support move from r-value the easy way
    explicit Set(std::vector<int> items) : m_items(std::move(items))
    {
        makeSet();
    }

    // Reads the set directly from input
    // The first input must be the number of items to read
    explicit Set(std::istream& istr);

    // Gives read access to the internal set items
    // Used for print but also by various algorithms
    auto begin() const { return m_items.begin(); }
    auto end() const { return m_items.end(); }

private:
    void makeSet();

    std::vector<int> m_items;
};

std::ostream& operator<<(std::ostream& ostr, const Set& set);
