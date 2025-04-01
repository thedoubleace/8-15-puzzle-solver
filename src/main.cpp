#include "../include/solvers.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>

void print_usage(const std::string& program_name);
std::vector<Board> extract_puzzles(int argc, char* argv[]);
bool validate_puzzles(const std::vector<Board>& boards, const std::string& algorithm);

int main(int argc, char* argv[]) {
    if (argc <= 2) {
        print_usage(argv[0]);
        return 1;
    }

    std::string command = argv[1];
    if(command == "-bfs" || command == "-idfs" || command == "-astar" || command == "-idastar" || command == "-gbfs") {
        std::vector<Board> puzzles = extract_puzzles(argc, argv); // Get puzzles from command line arguments
        if (puzzles.empty()) {
            std::cerr << "Error: No valid puzzles provided." << std::endl;
            return 1;
        }
        if (!validate_puzzles(puzzles, command)) { // Check whether every puzzle is valid
            return 1;
        }
        for (const auto& puzzle : puzzles) { // Solve each provided puzzle
            if (command == "-bfs") {
                Results result = depth_first_search(puzzle);
                std::cout << result.nodes_expanded << "," << result.depth << "," << result.time << "," 
                          << result.average_heuristic << "," << result.heuristic_at_start << std::endl;
            } else if (command == "-idfs") {
                
            } else if (command == "-astar" && puzzle.get_rows() * puzzle.get_cols() == 9) {
                
            } else if (command == "-astar" && puzzle.get_rows() * puzzle.get_cols() == 16) {
                
            } else if (command == "-idastar") {
                
            } else if (command == "-gbfs") {
                
            } else {
                std::cerr << "Error: Invalid algorithm-size combination." << std::endl;
                return 1;
            }
        }
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }
    
}

std::vector<Board> extract_puzzles(int argc, char* argv[]) {
    std::vector<Board> boards;
    std::vector<uint8_t> puzzle;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];

        std::istringstream arg_stream(arg); // Using stream to parse number from string
        int num;

        if (arg_stream >> num) {
            puzzle.push_back(static_cast<uint8_t>(num));
        }

        if (arg_stream.peek() == ',') { // If arg stream ends in ',', end of puzzle
            if (puzzle.size() == 9) {
                boards.push_back(Board(3, 3, puzzle));
            } else if (puzzle.size() == 16) {
                boards.push_back(Board(4, 4, puzzle));
            } else {
                std::cerr << "Invalid puzzle size. Expected 8-puzzle (9 tiles) or 15-puzzle (16 tiles).\n";
            }
            
            puzzle.clear();
        }
    }

    if (puzzle.size() == 9) {// Add last puzzle not followed by a comma
        boards.push_back(Board(3, 3, puzzle));
    } else if (puzzle.size() == 16) {
        boards.push_back(Board(4, 4, puzzle));
    } else {
        std::cerr << "Invalid puzzle size. Expected 8-puzzle (9 tiles) or 15-puzzle (16 tiles).\n";
    }
    

    return boards;
}

bool validate_puzzles(const std::vector<Board>& boards, const std::string& algorithm) {
    for (size_t i = 0; i < boards.size(); i++){ // Checking whether every puzzle is valid
        const auto& puzzle = boards[i].get_tiles();
        size_t n = puzzle.size();
        
        if (n == 16 && algorithm != "-astar") {
            std::cerr << "Error on puzzle " << i + 1 << ": 15-puzzle is only supported with the A* algorithm.\n";
            return false;
        }

        std::vector<bool> seen(n + 1, false);
        int max_tile = (n == 9) ? 8 : 15;

        for (const auto& tile : puzzle) {
            if (tile > max_tile) {
                std::cerr << "Error on puzzle " << i + 1 << ": Invalid tile value. Expected a digit between 0 and " << max_tile << ".\n";
                return false;
            }
            if (seen[tile]) {
                std::cerr << "Error on puzzle " << i + 1 << ": Duplicate tile value found: " << static_cast<int>(tile) << "\n";
                return false;
            }
            seen[tile] = true;
        }
    }

    return true;
}

void print_usage(const std::string& program_name) {
    std::cout << "Usage:\n" << program_name << " <algorithm> <comma-separated list of puzzles>\n\n";
    std::cout << std::setw(10) << "Flag" << std::setw(50) << "Description" << std::setw(30) << "Supported Puzzles\n";
    
    std::cout << std::setw(10) << "-bfs"     << std::setw(50) << "Breadth-First Search" << std::setw(30) <<  "[8-puzzle]\n";
    std::cout << std::setw(10) << "-idfs"    << std::setw(50) << "Iterative Deepening Depth-First Search" << std::setw(30) <<  "[8-puzzle]\n";
    std::cout << std::setw(10) << "-astar"   << std::setw(50) <<"A*" << std::setw(30) <<  "[8-puzzle, 15-puzzle]\n";
    std::cout << std::setw(10) << "-idastar" << std::setw(50) << "Iterative Deepening A*" << std::setw(30) <<  "[8-puzzle]\n";
    std::cout << std::setw(10) << "-gbfs"    << std::setw(50) << "Greedy Best-First Search" << std::setw(30) <<  "[8-puzzle]\n";

    std::cout << "\nExamples:\n";
    std::cout << "  " << program_name << " -bfs 1 2 3 4 5 6 7 8 0\n";
    std::cout << "  " << program_name << " -idfs 1 2 3 4 5 6 7 8 0, 0 9 8 7 6 5 4 3 2 1\n";
    std::cout << "  " << program_name << " -astar 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15\n";

    std::cout << "Output format:\n<no. of expanded nodes>,<solution depth>,<time elapsed>,<avg. heuristic value>,<heuristic at start>" << std::endl;
}
