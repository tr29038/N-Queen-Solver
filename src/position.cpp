#include "position.h"

Position::Position(int _x, int _y) :
    x(_x),
    y(_y)
{}

bool Position::operator==(Position p) const
{
    return x == p.x && y == p.y;
}

std::ostream& operator<<(std::ostream& output_stream, Position p)
{
    output_stream << p.x << ',' << p.y;

    return output_stream;
}
