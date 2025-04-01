#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <iostream>

class Board {

private:
    const unsigned int rows, cols;
    const std::vector<uint8_t> objective;
    std::vector<uint8_t> tiles;

    std::vector<uint8_t> generate_objective(int rows, int cols) const;
    
public:
    Board(int rows, int cols, const std::vector<uint8_t>& tiles);
    Board(const Board& other);
    Board(int rows, int cols);

    bool is_objective() const;

    unsigned int get_rows() const;
    unsigned int get_cols() const;
    std::vector<uint8_t> get_tiles() const;
    std::vector<uint8_t> get_objective() const;

    std::vector<Board> get_successors() const;
    int manhattan_distance() const;
    int hamming_distance() const;

    void print() const;
    
    bool operator==(const Board& other) const;
};

namespace std {
    template <>
    struct hash<Board> {
        size_t operator()(const Board& board) const;
    };
}
