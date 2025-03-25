//
// Created by mpiel on 24.03.2025.
//

#ifndef QUEEN_H
#define QUEEN_H
#include "Piece.h"


class Queen : public Piece {
public:
    Queen(Color color, PieceName piece_name)
        : Piece(color, piece_name) {};
};



#endif //QUEEN_H
