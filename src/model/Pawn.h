//
// Created by mpiel on 24.03.2025.
//

#ifndef PAWN_H
#define PAWN_H
#include "Piece.h"


class Pawn : public Piece {
public:
    Pawn(Color color, PieceName piece_name)
        : Piece(color, piece_name) {};
};



#endif //PAWN_H
