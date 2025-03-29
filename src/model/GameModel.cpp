//
// Created by mpiel on 19.03.2025.
//

#include "GameModel.h"
#include "Piece.h"
#include "Rook.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"
#include "Pawn.h"
#include "Knight.h"
#include <iostream>



void GameModel::populate_piece_board_map() {
    int index = 0;

    // Czarny tylny rząd (u góry)
    for (int x = 0; x < 8; ++x)
        piece_board_map_[x][7] = pieces_[index++];

    // Czarne pionki
    for (int x = 0; x < 8; ++x)
        piece_board_map_[x][6] = pieces_[index++];

    // Puste pola w środku
    for (int y = 2; y < 6; ++y)
        for (int x = 0; x < 8; ++x)
            piece_board_map_[x][y] = nullptr;

    // Białe pionki
    for (int x = 0; x < 8; ++x)
        piece_board_map_[x][1] = pieces_[index++];

    // Biały tylny rząd (na dole)
    for (int x = 0; x < 8; ++x)
        piece_board_map_[x][0] = pieces_[index++];
}

void GameModel::populate_pieces() {
    // White pieces
    pieces_.push_back(new Rook(WHITE, ROOK_WHITE));
    pieces_.push_back(new Knight(WHITE, KNIGHT_WHITE));
    pieces_.push_back(new Bishop(WHITE, BISHOP_WHITE));
    pieces_.push_back(new Queen(WHITE, QUEEN_WHITE));
    pieces_.push_back(new King(WHITE, KING_WHITE));
    pieces_.push_back(new Bishop(WHITE, BISHOP_WHITE));
    pieces_.push_back(new Knight(WHITE, KNIGHT_WHITE));
    pieces_.push_back(new Rook(WHITE, ROOK_WHITE));
    for (int i = 0; i < 8; ++i)
        pieces_.push_back(new Pawn(WHITE, PAWN_WHITE));

    // Black pieces
    pieces_.push_back(new Rook(BLACK, ROOK_BLACK));
    pieces_.push_back(new Knight(BLACK, KNIGHT_BLACK));
    pieces_.push_back(new Bishop(BLACK, BISHOP_BLACK));
    pieces_.push_back(new Queen(BLACK, QUEEN_BLACK));
    pieces_.push_back(new King(BLACK, KING_BLACK));
    pieces_.push_back(new Bishop(BLACK, BISHOP_BLACK));
    pieces_.push_back(new Knight(BLACK, KNIGHT_BLACK));
    pieces_.push_back(new Rook(BLACK, ROOK_BLACK));
    for (int i = 0; i < 8; ++i)
        pieces_.push_back(new Pawn(BLACK, PAWN_BLACK));
}

std::vector<Piece*> GameModel::getPieces() {
    for (auto piece : pieces_) {
        std::cout<< Piece::pieceNameToString(piece->name_);
    }
    return pieces_;
}

std::array<std::array<Piece*, 8>, 8> GameModel::getBoardMap() {
    return piece_board_map_;
}