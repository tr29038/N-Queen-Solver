#include "hill_climbing_searcher.h"

#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <queue>
#include <functional>

//! @brief Solve the n queens problem using the hill climbing solution.
//! @param initial_state The initial state to solve from.
//! @return The solution state if a solution was found, else std::nullopt.
std::optional<State> Hill_Climbing_Searcher::solve(const State& initial_state)
{
    auto current_state = initial_state;
    while (true)
    {
        if (current_state.get_number_of_collisions() == 0)
            return current_state;

        auto neighbors = current_state.get_neighbors();
        std::random_shuffle(neighbors.begin(), neighbors.end());
        auto neighbor_iter = std::find_if(neighbors.begin(), neighbors.end(), [current_state](const State& s)
        {
            return s.get_number_of_collisions() < current_state.get_number_of_collisions();
        });

        if (neighbor_iter == neighbors.end())
            break;

        current_state = *neighbor_iter;
    }

    return std::nullopt;
}
