#include "board.hpp"
#include <chrono>


struct Results {
    int nodes_expanded;
    int depth;
    double time;
    double average_heuristic;
    double heuristic_at_start;
};

struct Node {
    Board state;
    int depth;

    Node(const Board& state) : state(state), depth(0) {}

    Node(const Board& state, int depth) : state(state), depth(depth) {}
};

Results depth_first_search(const Board& start);