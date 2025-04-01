
#include "../include/board.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>

Board::Board(int rows, int cols, const std::vector<uint8_t>& tiles)
    : rows(rows), cols(cols), objective(generate_objective(rows, cols)) {
    if (tiles.size() != rows * cols) {
        throw std::invalid_argument("Invalid size for puzzle.");
    }
    
    this->tiles = tiles;
}

Board::Board(const Board& other) : rows(other.rows), cols(other.cols), objective(other.objective), tiles(other.tiles) {}

Board::Board(int rows, int cols): rows(rows), cols(cols), objective(generate_objective(rows, cols)), tiles(objective) {}

std::vector<uint8_t> Board::generate_objective(int rows, int cols) const {
    std::vector<uint8_t> obj(rows * cols);
    std::iota(obj.begin(), obj.end(), 0);
    return obj;
}

bool Board::is_objective() const {
    return tiles == objective;
}

unsigned int Board::get_rows() const {
    return rows;
}

unsigned int Board::get_cols() const {
    return cols;
}

std::vector<uint8_t> Board::get_tiles() const {
    return tiles;
}

std::vector<uint8_t> Board::get_objective() const {
    return objective;
}

std::vector<Board> Board::get_successors() const {
    std::vector<Board> successors;
    int zero_index = std::find(tiles.begin(), tiles.end(), 0) - tiles.begin(); // gets index of zero tile
    int zero_row = zero_index / 3;
    int zero_col = zero_index % 3;

    if (zero_row > 0) { // Moving up is possible
        Board successor(*this);
        std::swap(successor.tiles[zero_index], successor.tiles[zero_index - cols]); // Swap empty cell with cell above
        successors.push_back(successor);
    }

    if (zero_col > 0) { // Moving left is possible
        Board successor(*this);
        std::swap(successor.tiles[zero_index], successor.tiles[zero_index - 1]);
        successors.push_back(successor);
    }

    if (zero_col < 2) { // Moving right is possible
        Board successor(*this);
        std::swap(successor.tiles[zero_index], successor.tiles[zero_index + 1]);
        successors.push_back(successor);
    }

    if (zero_row < 2) { // Moving down is possible
        Board successor(*this);
        std::swap(successor.tiles[zero_index], successor.tiles[zero_index + cols]);
        successors.push_back(successor);
    }

    return successors;
}

// Sum of vertical and horizontal distances to goal for each (non-empty) tile
int Board::manhattan_distance() const {
    int distance = 0;
    for (int i = 0; i < tiles.size(); i++) {
        if (tiles[i] != 0) { // not counting the empty tile
            int target_row = tiles[i] / cols; // get rows and columns from index of tile in array
            int target_col = tiles[i] % cols;
            int current_row = i / cols;
            int current_col = i % cols;
            distance += std::abs(target_row - current_row) + std::abs(target_col - current_col);
        }
    }
    return distance;
}

// Number of (non-empty) tiles that are not in their goal position
int Board::hamming_distance() const {
    int distance = 0;
    for (int i = 0; i < tiles.size(); i++) {
        if (tiles[i] != 0 && tiles[i] != objective[i]) {
            ++distance;
        }
    }
    return distance;
}

void Board::print() const {
    for (int i = 0; i < tiles.size(); ++i) {
        std::cout << std::setw(2) << static_cast<int>(tiles[i]) << " ";
        if ((i + 1) % cols == 0) {
            std::cout << "\n";
        }
    }
    std::cout << std::endl;
}

bool Board::operator==(const Board& other) const {
    return rows == other.rows && cols == other.cols && tiles == other.tiles;
}

size_t std::hash<Board>::operator()(const Board& board) const {
    size_t hash = 0;
    size_t prime = 31;

    for (auto& n : board.get_tiles()) {
        hash = hash * prime + n;
    }

    return hash;
};