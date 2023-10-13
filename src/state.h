#pragma once

#include "position.h"

#include <ostream>
#include <vector>

class State
{
    public:
        State() = delete;
        State(const std::vector<Position>&);

        bool operator==(const State&) const;

        static State get_initial_state(int);

        friend std::ostream& operator<<(std::ostream&, const State&);

        std::size_t get_board_size() const;
        std::vector<std::vector<Position>> get_neighbors() const;
        int get_number_of_collisions() const;
        std::size_t get_hash() const;
        void print_board(std::ostream&) const;
        State mate(const State&) const;

    private:
        std::vector<std::vector<Position>> calculate_neighbors() const;
        int calculate_number_of_collisions() const;
        std::size_t generate_hash();
        int get_horizontal_collisions(Position) const;
        int get_diagonal_collisions(Position) const;
        bool in_bounds(Position) const;
        bool position_is_occupied(Position) const;

        std::vector<Position> queen_positions;
        std::vector<std::vector<Position>> neighbors;
        int number_of_collisions;
        std::size_t hash_code;
};
