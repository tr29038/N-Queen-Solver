#pragma once

#include "state.h"

#include <optional>

class Hill_Climbing_Searcher
{
    public:
        Hill_Climbing_Searcher() = delete;

        static std::optional<State> solve(const State&);
};
