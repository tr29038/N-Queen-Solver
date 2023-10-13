#include "genetic_solver.h"
#include "state.h"

#include <iostream>
#include <algorithm>
#include <random>

//! @brief Use a genetic algorithm to solve the n queens problem.
//! @param board_size Size of the board/number of queens.
//! @return If solved, the number of generations it took, else std::nullopt.
std::optional<int> Genetic_Solver::solve(int board_size)
{
    // Population size is 10 times the number of queens, as recommended.
    int population_size = 10 * board_size;
    // The percentage of the best from the original population to move to the new generation.
    int elitism_percentage = 10;
    // Generation tracker.
    int current_generation = 1;
    // Maximum generations to consider before aborting.
    int max_generations = population_size * board_size * 100;

    // Fill initial population with random start states.
    std::vector<State> population;
    for (int i = 0; i < population_size; ++i)
        population.push_back(State::get_initial_state(board_size));

    // Randomizer stuff.
    std::random_device random_device;
    std::mt19937 rng(random_device());
    std::uniform_int_distribution<std::mt19937::result_type> randomizer(0, population_size / 3);

    while (current_generation <= max_generations)
    {
        // Sort population so the state with the best fitness is first, and the worst is last.
        std::sort(population.begin(), population.end(), [](const State& s1, const State& s2){ return s1.get_number_of_collisions() < s2.get_number_of_collisions(); });

        // Solved.
        if (population[0].get_number_of_collisions() <= 0)
            return current_generation;

        std::vector<State> new_generation;

        // States with best fitness scores carry on to the next generation.
        int elitism_number = elitism_percentage / 100 * population_size;
        for (int i = 0; i < elitism_number; ++i)
            new_generation.push_back(population[i]);

        // The rest of the new generation is bred from randomly selected parents in the top 1/3 of fitness scores.
        for (int i = 0; i < population_size - elitism_number; ++i)
        {
            auto parent_1 = population[randomizer(rng)];
            auto parent_2 = population[randomizer(rng)];
            auto child = parent_1.mate(parent_2);
            new_generation.push_back(child);
        }

        population = new_generation;
        ++current_generation;
    }

    return std::nullopt;
}
