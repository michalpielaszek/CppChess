#include "Controller.h"

#include <GameModel.h>
#include <View.h>
#include <filesystem>
#include <Piece.h>

#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/image.h>
#include <iostream>


/*void Controller::initialize_pieces_images() {

    piecesPiecesMap_[KING_WHITE] = wxImage("../images/pieces/white/Chess_klt60.png", wxBITMAP_TYPE_PNG);
    piecesPiecesMap_[KING_BLACK] = wxImage("../images/pieces/black/Chess_kdt60.png", wxBITMAP_TYPE_PNG);
    piecesPiecesMap_[QUEEN_WHITE] = wxImage("../images/pieces/white/Chess_qlt60.png", wxBITMAP_TYPE_PNG);
    piecesPiecesMap_[QUEEN_BLACK] = wxImage("../images/pieces/black/Chess_qdt60.png", wxBITMAP_TYPE_PNG);
    piecesPiecesMap_[KNIGHT_WHITE] = wxImage("../images/pieces/white/Chess_nlt60.png", wxBITMAP_TYPE_PNG);
    piecesPiecesMap_[KNIGHT_BLACK] = wxImage("../images/pieces/black/Chess_pdt60.png", wxBITMAP_TYPE_PNG);
    piecesPiecesMap_[ROOK_WHITE] = wxImage("../images/pieces/white/Chess_rlt60.png", wxBITMAP_TYPE_PNG);
    piecesPiecesMap_[ROOK_BLACK] = wxImage("../images/pieces/black/Chess_rdt60.png", wxBITMAP_TYPE_PNG);
    piecesPiecesMap_[PAWN_WHITE] = wxImage("../images/pieces/white/Chess_plt60.png", wxBITMAP_TYPE_PNG);
    piecesPiecesMap_[PAWN_BLACK] = wxImage("../images/pieces/black/Chess_pdt60.png", wxBITMAP_TYPE_PNG);
    piecesPiecesMap_[BISHOP_WHITE] = wxImage("../images/pieces/white/Chess_blt60.png", wxBITMAP_TYPE_PNG);
    piecesPiecesMap_[BISHOP_BLACK] = wxImage("../images/pieces/black/Chess_bdt60.png", wxBITMAP_TYPE_PNG);


    wxImage image1("../images/pieces/white/Chess_klt60.png", wxBITMAP_TYPE_PNG);
    if (!image1.IsOk()) std::cout << "Failed to load Chess_klt60.png\n";
    piecesPiecesMap_[KING_WHITE] = image1;

    wxImage image2("../images/pieces/black/Chess_kdt60.png", wxBITMAP_TYPE_PNG);
    if (!image2.IsOk()) std::cout << "Failed to load Chess_kdt60.png\n";
    piecesPiecesMap_[KING_BLACK] = image2;

    wxImage image3("../images/pieces/white/Chess_qlt60.png", wxBITMAP_TYPE_PNG);
    if (!image3.IsOk()) std::cout << "Failed to load Chess_qlt60.png\n";
    piecesPiecesMap_[QUEEN_WHITE] = image3;

    wxImage image4("../images/pieces/black/Chess_qdt60.png", wxBITMAP_TYPE_PNG);
    if (!image4.IsOk()) std::cout << "Failed to load Chess_qdt60.png\n";
    piecesPiecesMap_[QUEEN_BLACK] = image4;

    wxImage image5("../images/pieces/white/Chess_nlt60.png", wxBITMAP_TYPE_PNG);
    if (!image5.IsOk()) std::cout << "Failed to load Chess_nlt60.png\n";
    piecesPiecesMap_[KNIGHT_WHITE] = image5;

    wxImage image6("../images/pieces/black/Chess_pdt60.png", wxBITMAP_TYPE_PNG);
    if (!image6.IsOk()) std::cout << "Failed to load Chess_pdt60.png\n";
    piecesPiecesMap_[KNIGHT_BLACK] = image6;

    wxImage image7("../images/pieces/white/Chess_rlt60.png", wxBITMAP_TYPE_PNG);
    if (!image7.IsOk()) std::cout << "Failed to load Chess_rlt60.png\n";
    piecesPiecesMap_[ROOK_WHITE] = image7;

    wxImage image8("../images/pieces/black/Chess_rdt60.png", wxBITMAP_TYPE_PNG);
    if (!image8.IsOk()) std::cout << "Failed to load Chess_rdt60.png\n";
    piecesPiecesMap_[ROOK_BLACK] = image8;

    wxImage image9("../images/pieces/white/Chess_plt60.png", wxBITMAP_TYPE_PNG);
    if (!image9.IsOk()) std::cout << "Failed to load Chess_plt60.png\n";
    piecesPiecesMap_[PAWN_WHITE] = image9;

    wxImage image10("../images/pieces/black/Chess_pdt60.png", wxBITMAP_TYPE_PNG);
    if (!image10.IsOk()) std::cout << "Failed to load Chess_pdt60.png\n";
    piecesPiecesMap_[PAWN_BLACK] = image10;

    wxImage image11("../images/pieces/white/Chess_blt60.png", wxBITMAP_TYPE_PNG);
    if (!image11.IsOk()) std::cout << "Failed to load Chess_blt60.png\n";
    piecesPiecesMap_[BISHOP_WHITE] = image11;

    wxImage image12("../images/pieces/black/Chess_bdt60.png", wxBITMAP_TYPE_PNG);
    if (!image12.IsOk()) std::cout << "Failed to load Chess_bdt60.png\n";
    piecesPiecesMap_[BISHOP_BLACK] = image12;
}
*/

#include <filesystem>     // Needed for std::filesystem
#include <wx/filename.h>  // Needed for wxFileName
#include <wx/stdpaths.h>  // Needed for wxStandardPaths


void Controller::initialize_pieces_images() {
    wxInitAllImageHandlers();  // 🔧 Required to avoid handler errors

    // Get the directory where the executable resides
    wxString execPath = wxStandardPaths::Get().GetExecutablePath();
    wxString baseDir = wxFileName(execPath).GetPath(); // cmake-build-debug
    wxString projectRoot = wxFileName(baseDir + "/..").GetAbsolutePath();

    wxString whitePath = projectRoot + "/images/pieces/white/";
    wxString blackPath = projectRoot + "/images/pieces/black/";

    auto loadImage = [](const wxString& path) {
        wxImage img(path, wxBITMAP_TYPE_PNG);
        if (!img.IsOk()) {
            std::cout << "Failed to load image: " << std::string(path.mb_str()) << std::endl;
        }
        return img;
    };

    piecesPiecesMap_[KING_WHITE]    = loadImage(whitePath + "Chess_klt60.png");
    piecesPiecesMap_[KING_BLACK]    = loadImage(blackPath + "Chess_kdt60.png");
    piecesPiecesMap_[QUEEN_WHITE]   = loadImage(whitePath + "Chess_qlt60.png");
    piecesPiecesMap_[QUEEN_BLACK]   = loadImage(blackPath + "Chess_qdt60.png");
    piecesPiecesMap_[KNIGHT_WHITE]  = loadImage(whitePath + "Chess_nlt60.png");
    piecesPiecesMap_[KNIGHT_BLACK]  = loadImage(blackPath + "Chess_ndt60.png");
    piecesPiecesMap_[ROOK_WHITE]    = loadImage(whitePath + "Chess_rlt60.png");
    piecesPiecesMap_[ROOK_BLACK]    = loadImage(blackPath + "Chess_rdt60.png");
    piecesPiecesMap_[PAWN_WHITE]    = loadImage(whitePath + "Chess_plt60.png");
    piecesPiecesMap_[PAWN_BLACK]    = loadImage(blackPath + "Chess_pdt60.png");
    piecesPiecesMap_[BISHOP_WHITE]  = loadImage(whitePath + "Chess_blt60.png");
    piecesPiecesMap_[BISHOP_BLACK]  = loadImage(blackPath + "Chess_bdt60.png");
}

void Controller::initialize_controller() {
    wxInitAllImageHandlers();
    model_->populate_pieces();             // must come first
    model_->populate_piece_board_map();
    initialize_pieces_images();
}

void Controller::map_all_pieces_to_tiles() {
    auto boardMap = model_->getBoardMap();

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* piece = boardMap[i][j];
            if (!piece) continue;

            auto it = piecesPiecesMap_.find(piece->name_);
            if (it != piecesPiecesMap_.end()) {
                view_->gameWindow_->board_->piecesBoardMap_[{i, j}] = it->second;
            }
        }
    }
}


