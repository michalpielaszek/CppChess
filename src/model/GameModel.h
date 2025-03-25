//
// Created by mpiel on 19.03.2025.
//

#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "Piece.h"
#include "vector"
#include "array"

class Controller;

class GameModel {

    //variables
public:
    Controller* controller_;
private:
    std::array<std::array<Piece*, 8>, 8> piece_board_map_;
    std::vector<Piece*> pieces_;

    //methods
public:
    GameModel() : controller_(nullptr) {

    }

    void setController(Controller* controller) {
        controller_ = controller;
    }

    void populate_piece_board_map();
    void populate_pieces();

    std::array<std::array<Piece*, 8>, 8> getBoardMap();
    std::vector<Piece*> getPieces();
};



#endif //GAMEMODEL_H
