//
// Created by mpiel on 24.03.2025.
//

#ifndef ROOK_H
#define ROOK_H
#include "Piece.h"


class Rook : public Piece {
public:
    Rook(Color color, PieceName piece_name)
        : Piece(color, piece_name) {};
};

#endif //ROOK_H
