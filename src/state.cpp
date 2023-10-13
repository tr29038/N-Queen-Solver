#include "state.h"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <random>

State::State(const std::vector<Position>& _queen_positions) :
    queen_positions(_queen_positions),
    neighbors(calculate_neighbors()),
    number_of_collisions(calculate_number_of_collisions()),
    hash_code(generate_hash())
{}

bool State::operator==(const State& s) const
{
    return queen_positions == s.queen_positions;
}

std::size_t State::get_board_size() const
{
    return queen_positions.size();
}

//! @brief Randomly generate queen positions, where there is only one queen per column and per row.
//! @param board_size The size of the grid (NxN).
//! @return The associated randomized state.
State State::get_initial_state(int board_size)
{
    std::vector<Position> queen_positions;
    std::random_device random_device;
    std::mt19937 rng(random_device());
    std::uniform_int_distribution<std::mt19937::result_type> randomizer(0, board_size - 1);

    for (int x = 0; x < board_size; ++x)
    {
        int y = randomizer(rng);
        queen_positions.emplace_back(x, y);
    }

    return {queen_positions};
}

std::vector<std::vector<Position>> State::get_neighbors() const
{
    return neighbors;
}

int State::get_number_of_collisions() const
{
    return number_of_collisions;
}

//! @brief Compute all of the neighbor states of this state.
//!        A state is a neighbor if a single queen's y coordinate is incremented or decremented.
//! @return All neighbor states.
std::vector<std::vector<Position>> State::calculate_neighbors() const
{
    std::vector<int> y_offsets;
    y_offsets.push_back(1);
    y_offsets.push_back(-1);

    std::vector<std::vector<Position>> neighbors;
    for (auto current_queen_position : queen_positions)
    {
        for (auto y_offset : y_offsets)
        {
            Position neighbor_position(current_queen_position.x, current_queen_position.y + y_offset);
            if (!in_bounds(neighbor_position) || position_is_occupied(neighbor_position))
                continue;

            std::vector<Position> queen_positions_copy(queen_positions.begin(), queen_positions.end());
            auto current_queen_position_iter = std::find(queen_positions_copy.begin(), queen_positions_copy.end(), current_queen_position);

            current_queen_position_iter->y += y_offset;
            neighbors.push_back(queen_positions_copy);
        }
    }

    return neighbors;
}

//! @brief Calculate the number of queens that can attack each other.
//!        A pair of queens within attacking range counts as 2 collisions here.
//! @return The number of potential attacks in the state.
int State::calculate_number_of_collisions() const
{
    int num_collisions = 0;
    for (auto current_queen_position : queen_positions)
    {
        num_collisions
            += get_horizontal_collisions(current_queen_position)
            + get_diagonal_collisions(current_queen_position);
    }

    return num_collisions;
}

std::size_t State::get_hash() const
{
    return hash_code;
}

void State::print_board(std::ostream& output_stream) const
{
    std::vector<std::vector<char>> board(queen_positions.size(), std::vector<char>(queen_positions.size(), ' '));
    for (auto queen_position : queen_positions)
        board[queen_position.y][queen_position.x] = 'Q';

    for (int y = 0; y < queen_positions.size(); ++y)
    {
        for (int x = 0; x < queen_positions.size(); ++x)
        {
            output_stream << board[y][x];
        }
        output_stream << '\n';
    }
}

//! @brief Create a child state from this state and parent_state.
//!        A parent's queen position to carry over to the child is randomly selected.
//!        A small chance exists that a new queen position is generated in the child state.
//! @param parent_state A parent state.
//! @return The generated child state.
//! @throw std::runtime_error If this->board_size() != parent_state.board_size().
State State::mate(const State& parent_state) const
{
    if (get_board_size() != parent_state.get_board_size())
        throw std::runtime_error("Boards with different sizes cannot mate");

    std::vector<Position> child_queen_positions;
    std::random_device random_device;
    std::mt19937 rng(random_device());
    std::uniform_int_distribution<std::mt19937::result_type> randomizer(1, 100);
    std::uniform_int_distribution<std::mt19937::result_type> positioner(0, get_board_size() - 1);

    for (int i = 0; i < get_board_size(); ++i)
    {
        auto probability = randomizer(rng);

        if (probability < 45)
            child_queen_positions.push_back(queen_positions[i]);
        else if (probability < 90)
            child_queen_positions.push_back(parent_state.queen_positions[i]);
        else
            child_queen_positions.emplace_back(i, positioner(rng));
    }

    return {child_queen_positions};
}

std::size_t State::generate_hash()
{
    std::hash<std::string> hasher;
    std::stringstream ss;
    ss << *this;
    return hasher(ss.str());
}

int State::get_horizontal_collisions(Position queen_position) const
{
    int num_collisions = 0;
    for (auto current_queen_position : queen_positions)
    {
        if (current_queen_position == queen_position)
            continue;

        if (current_queen_position.y == queen_position.y)
            ++num_collisions;
    }

    return num_collisions;
}

int State::get_diagonal_collisions(Position queen_position) const
{
    int num_collisions = 0;
    int board_size = queen_positions.size();
    int origin_x = queen_position.x;
    int origin_y = queen_position.y;
    for (auto current_queen_position : queen_positions)
    {
        if (current_queen_position == queen_position)
            continue;

        // Up, left
        Position current_position(origin_x, origin_y);
        while (in_bounds(current_position))
        {
            if (current_position == current_queen_position)
                ++num_collisions;
            --current_position.x; --current_position.y;
        }

        // Up, right
        current_position.x = origin_x; current_position.y = origin_y;
        while (in_bounds(current_position))
        {
            if (current_position == current_queen_position)
                ++num_collisions;
            ++current_position.x; --current_position.y;
        }

        // Down, left
        current_position.x = origin_x; current_position.y = origin_y;
        while (in_bounds(current_position))
        {
            if (current_position == current_queen_position)
                ++num_collisions;
            --current_position.x; ++current_position.y;
        }

        // Down, right
        current_position.x = origin_x; current_position.y = origin_y;
        while (in_bounds(current_position))
        {
            if (current_position == current_queen_position)
                ++num_collisions;
            ++current_position.x; ++current_position.y;
        }
    }

    return num_collisions;
}

bool State::in_bounds(Position p) const
{
    int board_size = queen_positions.size();
    return p.x >= 0 && p.y >= 0 && p.x < board_size && p.y < board_size;
}

bool State::position_is_occupied(Position p) const
{
    return std::find(queen_positions.begin(), queen_positions.end(), p) != queen_positions.end();
}

std::ostream& operator<<(std::ostream& output_stream, const State& state)
{
    for (auto queen_position : state.queen_positions)
        output_stream << queen_position << '\n';

    return output_stream;
}
