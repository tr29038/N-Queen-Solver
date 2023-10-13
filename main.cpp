#include <iostream>
#include <fstream>
#include <string>
#include <optional>

#include "hill_climbing_searcher.h"
#include "genetic_solver.h"

void output_results(const std::string& output_file_path, const State& start_state, const std::optional<State>& final_state)
{
    std::ofstream file_writer(output_file_path);

    file_writer << "Start:\n";
    start_state.print_board(file_writer);
    file_writer << '\n';

    if (!final_state.has_value())
    {
        file_writer << "Fail.";
    }
    else
    {
        file_writer << "Final:\n";
        final_state.value().print_board(file_writer);
        file_writer << '\n';
        file_writer << "Success.";
    }

    file_writer.close();
}

void output_results(const std::string& output_file_path, std::optional<int> number_of_generations)
{
    std::ofstream file_writer(output_file_path);

    file_writer
        << "Number of generations to solve: "
        << (number_of_generations.has_value() ? std::to_string(number_of_generations.value()) : std::string("Fail"))
        << '\n';

    file_writer.close();
}

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cout
            << "Correct usage: "
            << argv[0] << " <size of board> <genetic|hill-climbing> <output file path>\n";

        return 1;
    }

    int board_size;
    try
    {
        board_size = std::stoi(argv[1]);
    }
    catch (const std::exception& e)
    {
        std::cout << "Invalid command line argument: " << e.what() << '\n';
        return 1;
    }

    std::string algorithm = argv[2];
    const std::string output_file_path = argv[3];
    if (algorithm == "hill-climbing")
    {
        auto initial_state = State::get_initial_state(board_size);
        std::optional<State> final_state;
        final_state = Hill_Climbing_Searcher::solve(initial_state);

        try
        {
            output_results(output_file_path, initial_state, final_state);
        }
        catch (const std::exception& e)
        {
            std::cout << "Failed to write to file: " << e.what() << '\n';
            return 1;
        }
    }
    else if (algorithm == "genetic")
    {
        auto generations_to_solve = Genetic_Solver::solve(board_size);

        try
        {
            output_results(output_file_path, generations_to_solve);
        }
        catch (const std::exception& e)
        {
            std::cout << "Failed to write to file: " << e.what() << '\n';
            return 1;
        }
    }
    else
    {
        throw std::runtime_error("Invalid algorithm provided. Use hill-climbing or genetic.");
    }

    return 0;
}
