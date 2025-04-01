#include "../include/solvers.hpp"
#include <deque>
#include <chrono>
#include <unordered_set>

Results depth_first_search(const Board& start) {
    Results results = {};
    results.heuristic_at_start = start.manhattan_distance();

    std::deque<Node> open;
    open.push_back(start);

    std::unordered_set<Board> closed;
    closed.insert(start);

    bool done = start.is_objective();

    auto start_time = std::chrono::high_resolution_clock::now();

    while (!open.empty() && !done) {
        Node current = open.front();
        open.pop_front();

        results.nodes_expanded += 1;

        const auto successors = current.state.get_successors();
        for (const auto& s : successors) {
            if (s.is_objective()) { // If objective found among successors
                results.depth = current.depth + 1;
                done = true;
                break;
            }
            if (closed.find(s) == closed.end()) { // If state not checked before
                closed.insert(s);
                open.push_back(Node(s, current.depth + 1));
            }
        }
    }

    results.average_heuristic = 0.0F;
    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration<double>(end_time - start_time); 
    results.time = duration.count();  

    return results;
}