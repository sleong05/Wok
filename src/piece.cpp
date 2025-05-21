#include "piece.hpp"

Piece::Piece(int x, int y, int color) : position({x, y}), color(color) {}

std::pair<int, int> Piece::getPosition() { return position; }

int Piece::getColor() { return color; }

Piece::~Piece() {}