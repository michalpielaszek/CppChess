//
// Created by mpiel on 24.03.2025.
//

#ifndef KING_H
#define KING_H

#include "Piece.h"

class King : public Piece {
public:
    King(Color color, PieceName piece_name)
        : Piece(color, piece_name) {};
};



#endif //KING_H
