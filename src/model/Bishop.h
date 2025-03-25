//
// Created by mpiel on 24.03.2025.
//

#ifndef BISHOP_H
#define BISHOP_H
#include "Piece.h"


class Bishop : public Piece {
public:
    Bishop(Color color, PieceName piece_name)
        : Piece(color, piece_name) {};
};



#endif //BISHOP_H
