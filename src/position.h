#pragma once

#include <ostream>

struct Position
{
    Position() = delete;
    Position(int, int);

    bool operator==(Position) const;

    int x, y;
};

std::ostream& operator<<(std::ostream&, Position);
