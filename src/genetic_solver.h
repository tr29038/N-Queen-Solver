#pragma once

#include <optional>

class Genetic_Solver
{
    public:
        Genetic_Solver() = delete;

        static std::optional<int> solve(int);
};
