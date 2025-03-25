//
// Created by mpiel on 24.03.2025.
//

#ifndef KNIGHT_H
#define KNIGHT_H
#include "Piece.h"


class Knight : public Piece {
public:
    Knight(Color color, PieceName piece_name)
        : Piece(color, piece_name) {};
};



#endif //KNIGHT_H
