#ifndef PIECE_H
#define PIECE_H

enum Color {
    WHITE,
    BLACK
};

enum PieceName {
    KING_WHITE,
    KING_BLACK,
    QUEEN_WHITE,
    QUEEN_BLACK,
    KNIGHT_WHITE,
    KNIGHT_BLACK,
    ROOK_WHITE,
    ROOK_BLACK,
    PAWN_WHITE,
    PAWN_BLACK,
    BISHOP_WHITE,
    BISHOP_BLACK,
};

class Piece {
public:
    Color color_ = WHITE;
    PieceName name_ = KING_WHITE;
    Piece(const Color color, const PieceName name) : color_(color), name_(name) {}
    Piece() : color_(WHITE), name_(KING_WHITE) {}

    const static char* pieceNameToString(PieceName name) {
        switch (name) {
            case KING_WHITE: return "KING_WHITE";
            case KING_BLACK: return "KING_BLACK";
            case QUEEN_WHITE: return "QUEEN_WHITE";
            case QUEEN_BLACK: return "QUEEN_BLACK";
            case ROOK_WHITE: return "ROOK_WHITE";
            case ROOK_BLACK: return "ROOK_BLACK";
            case BISHOP_WHITE: return "BISHOP_WHITE";
            case BISHOP_BLACK: return "BISHOP_BLACK";
            case KNIGHT_WHITE: return "KNIGHT_WHITE";
            case KNIGHT_BLACK: return "KNIGHT_BLACK";
            case PAWN_WHITE: return "PAWN_WHITE";
            case PAWN_BLACK: return "PAWN_BLACK";
            default: return "UNKNOWN";
        }
    }
};



#endif //PIECE_H
