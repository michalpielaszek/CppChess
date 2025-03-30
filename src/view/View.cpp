#include "View.h"
#include "Controller.h"

#include <wx/image.h>
#include <iostream>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/dcbuffer.h>

const int TILE_SIZE = 60;

int View::start(int argc, char** argv, View& view) {
    return wxEntry(argc, argv);
}

wxBitmap View::ConvertToBitmap(const wxImage& image) {
    return wxBitmap(image);
}

wxBitmap View::CombineImagesToBitmap(const wxImage& tileImage, const wxImage& pieceImage) {
    wxImage tileCopy = tileImage.Copy();
    wxImage pieceCopy = pieceImage;

    // Skalowanie figur (ważne!)
    if (tileCopy.GetSize() != pieceCopy.GetSize()) {
        pieceCopy = pieceCopy.Scale(tileCopy.GetWidth(), tileCopy.GetHeight(), wxIMAGE_QUALITY_HIGH);
    }

    // Zapewnij kanał alfa
    if (!tileCopy.HasAlpha()) tileCopy.InitAlpha();
    if (!pieceCopy.HasAlpha()) pieceCopy.InitAlpha();

    /*
     *  Precisely what is going on here, since this library struggles with alpha channels.
     *  Approach taken here is as follows, the program iterates through all pixels of tile image
     *  as image is already resized to match tile size, and handles alpha proportions for each pixel.
     *  Each value of RGB is being blended with alpha proportion being value in [0, 1]
     *  0 - full transparency
     *  1 - full visibility
     *
     */
    for (int x = 0; x < tileCopy.GetWidth(); ++x) {
        for (int y = 0; y < tileCopy.GetHeight(); ++y) {
            const float alpha = pieceCopy.GetAlpha(x, y) / 255.f;

            if (alpha > 0.f) {
                const unsigned char tileR = tileCopy.GetRed(x, y);
                const unsigned char tileG = tileCopy.GetGreen(x, y);
                const unsigned char tileB = tileCopy.GetBlue(x, y);

                const unsigned char pieceR = pieceCopy.GetRed(x, y);
                const unsigned char pieceG = pieceCopy.GetGreen(x, y);
                const unsigned char pieceB = pieceCopy.GetBlue(x, y);

                // Color mix
                const unsigned char finalR = tileR * (1 - alpha) + pieceR * alpha;
                const unsigned char finalG = tileG * (1 - alpha) + pieceG * alpha;
                const unsigned char finalB = tileB * (1 - alpha) + pieceB * alpha;

                tileCopy.SetRGB(x, y, finalR, finalG, finalB);
                tileCopy.SetAlpha(x, y, 255);
            }
        }
    }

    return wxBitmap(tileCopy);
}


// ===========================
//           VIEW
// ===========================

bool View::OnInit() {
    wxLog::SetActiveTarget(nullptr);
    wxInitAllImageHandlers();

    std::cout << "Has PNG handler: " << wxImage::FindHandler(wxBITMAP_TYPE_PNG) << "\n";

    menu_ = new Menu(this);
    settings_ = new Settings(this);
    gameWindow_ = new GameWindow(this);

    board_ = new Board(gameWindow_);
    gameWindow_->board_ = board_;

    menu_->initialize_frame();
    settings_->initialize_frame();

    if (controller_) {
        controller_->initialize_controller();  // ← Controller musi być już dostępny tutaj
    } else {
        std::cerr << "Controller not initialized in View::OnInit()\n";
    }

    board_->populate_maps();  // ← DOPIERO TUTAJ, PO CONTROLLERZE (ważne!)
    gameWindow_->initialize_frame();  // ← GameWindow musi się utworzyć PO mapach

    ShowMenu();

    //Handlers must be initiated after all windows and classes are ready and initialized
    //TODO
    //handlers for other classes
    return true;
}

void View::ShowMenu() {
    settings_->settingsFrame_->Hide();
    gameWindow_->gameFrame_->Hide();
    auto pos = settings_->settingsFrame_->GetPosition();
    menu_->menuFrame_->SetPosition(pos);
    menu_->menuFrame_->Show(true);
}

void View::ShowSettings() {
    menu_->menuFrame_->Hide();
    auto pos = menu_->menuFrame_->GetPosition();
    settings_->settingsFrame_->SetPosition(pos);
    settings_->settingsFrame_->Show(true);
}

void View::ShowGameWindow() {
    menu_->menuFrame_->Hide();
    auto pos = menu_->menuFrame_->GetPosition();
    gameWindow_->gameFrame_->SetPosition(pos);
    gameWindow_->gameFrame_->Show(true);
    gameWindow_->initialize_handlers();
    gameWindow_->resizeReady_ = true;
}

// ===========================
//           MENU
// ===========================

void View::Menu::initialize_frame() {
    menuFrame_ = new wxFrame(nullptr, wxID_ANY, "Menu", wxDefaultPosition, wxSize(400, 400));
    menuPanel_ = new wxPanel(menuFrame_, wxID_ANY);

    mainSizerV_ = new wxBoxSizer(wxVERTICAL);
    secondarySizerH_ = new wxBoxSizer(wxVERTICAL);

    title_ = new wxStaticText(menuPanel_, wxID_ANY, "Chess Game", wxDefaultPosition, wxSize(200, 40), wxALIGN_CENTER);
    title_->SetFont(wxFont(18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    startGameButton_ = new wxButton(menuPanel_, wxID_ANY, "Start", wxDefaultPosition, wxSize(200, 40));
    settingsButton_ = new wxButton(menuPanel_, wxID_ANY, "Settings", wxDefaultPosition, wxSize(200, 40));
    scoreboardButton_ = new wxButton(menuPanel_, wxID_ANY, "Scoreboard", wxDefaultPosition, wxSize(200, 40));
    exitButton_ = new wxButton(menuPanel_, wxID_EXIT, "Exit", wxDefaultPosition, wxSize(200, 40));

    secondarySizerH_->Add(title_, 0, wxALL, 10);
    secondarySizerH_->Add(startGameButton_, 0, wxALL, 10);
    secondarySizerH_->Add(settingsButton_, 0, wxALL, 10);
    secondarySizerH_->Add(scoreboardButton_, 0, wxALL, 10);
    secondarySizerH_->Add(exitButton_, 0, wxALL, 10);

    mainSizerV_->AddStretchSpacer();
    mainSizerV_->Add(secondarySizerH_, 0, wxALIGN_CENTER);
    mainSizerV_->AddStretchSpacer();

    menuPanel_->SetSizer(mainSizerV_);

    startGameButton_->Bind(wxEVT_BUTTON, &View::Menu::OnStartGamePressed, this);
    settingsButton_->Bind(wxEVT_BUTTON, &View::Menu::OnSettingsPressed, this);
    exitButton_->Bind(wxEVT_BUTTON, &View::Menu::OnExitPressed, this);
}

void View::Menu::OnStartGamePressed(wxCommandEvent& event) {
    parent_->ShowGameWindow();
}

void View::Menu::OnSettingsPressed(wxCommandEvent& event) {
    parent_->ShowSettings();
}

void View::Menu::OnExitPressed(wxCommandEvent& event) {
    menuFrame_->Close();
}

// ===========================
//          SETTINGS
// ===========================

void View::Settings::initialize_frame() {
    settingsFrame_ = new wxFrame(nullptr, wxID_ANY, "Settings", wxDefaultPosition, wxSize(400, 400));
    settingsPanel_ = new wxPanel(settingsFrame_, wxID_ANY);

    mainSizerV_ = new wxBoxSizer(wxVERTICAL);
    secondarySizerH_ = new wxBoxSizer(wxVERTICAL);

    title_ = new wxStaticText(settingsPanel_, wxID_ANY, "Settings", wxDefaultPosition, wxSize(200, 40), wxALIGN_CENTER);
    title_->SetFont(wxFont(18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    leaveSettingsButton_ = new wxButton(settingsPanel_, wxID_ANY, "Back", wxDefaultPosition, wxSize(200, 40));

    secondarySizerH_->Add(title_, 0, wxALL, 10);
    secondarySizerH_->Add(leaveSettingsButton_, 0, wxALL, 10);

    mainSizerV_->AddStretchSpacer();
    mainSizerV_->Add(secondarySizerH_, 0, wxALIGN_CENTER);
    mainSizerV_->AddStretchSpacer();

    settingsPanel_->SetSizer(mainSizerV_);

    leaveSettingsButton_->Bind(wxEVT_BUTTON, &View::Settings::OnLeaveSettingsButtonClick, this);

    settingsFrame_->Show(false);
}

void View::Settings::OnLeaveSettingsButtonClick(wxCommandEvent& event) {
    parent_->ShowMenu();
}

// ===========================
//         GAME WINDOW
// ===========================

void View::GameWindow::initialize_frame() {
    gameFrame_ = new wxFrame(nullptr, wxID_ANY, "Game", wxDefaultPosition, wxSize(TILE_SIZE * 8 + 220, TILE_SIZE * 8 + 50));
    gamePanel_ = new wxPanel(gameFrame_, wxID_ANY);
    frontLayout_ = new wxBoxSizer(wxHORIZONTAL);
    gamePanel_->SetSizer(frontLayout_);




    Board* board = parent_->board_;
    if (!board) {
        std::cerr << "[ERROR] board_ is null!\n";
        return;
    }

    board->populate_maps();

    // Left-side board
    //boardPanel_ = new wxPanel(gamePanel_, wxID_ANY, wxDefaultPosition, wxSize(TILE_SIZE * 8, TILE_SIZE * 8));
    boardPanel_ = new BoardPanel(gamePanel_, board);

    boardPanel_->SetMinSize(wxSize(TILE_SIZE * 8, TILE_SIZE * 8));

    // StretchSpacer is just an element that sets proportions how much left and right should be left as free space
    // as the frontLayout_ aligns its children to center
    frontLayout_->AddStretchSpacer(1);
    frontLayout_->Add(boardPanel_, 0, wxALIGN_CENTER, 5);
    frontLayout_->AddStretchSpacer(1);

    // Right-side info panel
    infoPanel_ = new wxPanel(gamePanel_, wxID_ANY);
    infoLayout_ = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* infoWrapper = new wxBoxSizer(wxVERTICAL);
    infoWrapper->AddStretchSpacer();
    infoWrapper->Add(infoLayout_, 1, wxEXPAND | wxALL, 0);
    infoWrapper->AddStretchSpacer();
    infoPanel_->SetSizer(infoWrapper);

    player_1_info_   = new wxStaticText(infoPanel_, wxID_ANY, "Player 1");
    player_1_resign_ = new wxButton(infoPanel_, wxID_ANY, "Resign");
    player_1_undo    = new wxButton(infoPanel_, wxID_ANY, "Undo");
    timer_           = new wxStaticText(infoPanel_, wxID_ANY, "Timer");
    player_2_resign_ = new wxButton(infoPanel_, wxID_ANY, "Resign");
    player_2_undo    = new wxButton(infoPanel_, wxID_ANY, "Undo");
    player_2_info_   = new wxStaticText(infoPanel_, wxID_ANY, "Player 2");
    backButton_      = new wxButton(infoPanel_, wxID_ANY, "Back");

    infoLayout_->Add(player_1_info_,   1, wxEXPAND | wxALL, 10);
    infoLayout_->Add(player_1_resign_, 1, wxEXPAND | wxALL, 10);
    infoLayout_->Add(player_1_undo,    1, wxEXPAND | wxALL, 10);
    infoLayout_->Add(timer_,           1, wxEXPAND | wxALL, 10);
    infoLayout_->Add(player_2_resign_, 1, wxEXPAND | wxALL, 10);
    infoLayout_->Add(player_2_undo,    1, wxEXPAND | wxALL, 10);
    infoLayout_->Add(player_2_info_,   1, wxEXPAND | wxALL, 10);
    infoLayout_->Add(backButton_,      1, wxEXPAND | wxALL, 10);

    frontLayout_->Add(infoPanel_, 1, wxEXPAND | wxALL, 5);

    gameFrame_->Show(false);
}

void View::GameWindow::OnResize(wxSizeEvent& event) {
    if (resizeReady_) {
        wxSize panelSize = gamePanel_->GetClientSize();

        // Zakładamy, że frontLayout_ ma boardPanel_ i infoPanel_
        int infoPanelWidth = infoPanel_->GetSize().GetWidth();

        if (panelSize.GetWidth() <= 0 || panelSize.GetHeight() <= 0 || infoPanelWidth <= 0) {
            std::cerr << "Zbyt wczesny resize – infoPanel/gamePanel nie gotowe\n";
            event.Skip();
            return;
        }

        int availableWidth = panelSize.GetWidth() - infoPanelWidth;
        int availableHeight = panelSize.GetHeight();

        int size = std::min(availableWidth, availableHeight);

        boardPanel_->SetMinSize(wxSize(size, size));
        boardPanel_->SetSize(wxSize(size, size));

        board_->setSize(size);
        std::cout << "Size: " << size << ", Size/8: " << size/8 <<  std::endl;

        gamePanel_->Layout();
        gamePanel_->Refresh();

        //for now handling is just ignored
    }
    event.Skip();
}

/*void View::GameWindow::size_tiles_update(int x, int y) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            std::pair<int, int> pos = {i, j};
            auto tile_img = board_->boardTilesImagesMap_.at(pos).Scale(x, y, wxIMAGE_QUALITY_HIGH);
            wxBitmap bitmap;

            // Jeśli jest figura na polu i ma prawidłowy obraz
            if (board_->piecesBoardMap_.contains(pos) && board_->piecesBoardMap_.at(pos).IsOk()) {
                auto piece_img = board_->piecesBoardMap_.at(pos).Scale(x, y, wxIMAGE_QUALITY_HIGH);
                bitmap = View::CombineImagesToBitmap(tile_img, piece_img);
            } else {
                // Puste pole
                bitmap = wxBitmap(tile_img);
            }

            board_->piece_tile_images_[pos] = bitmap;
            board_->boardDisplayReadyMap_[pos] = bitmap;
        }
    }
}*/

/*void View::GameWindow::refresh_board_display() {
    gameGrid_->Clear(true);  // usuwa stare bitmapy

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            std::pair<int, int> pos = {x, y};
            wxStaticBitmap *img = new wxStaticBitmap(boardPanel_, wxID_ANY, board_->boardDisplayReadyMap_[pos]);
            gameGrid_->Add(img, 0, wxALIGN_CENTER);
        }
    }
    boardPanel_->Layout();
}*/

void View::GameWindow::OnLeaveGameButtonClick(wxCommandEvent& event) {
    parent_->ShowMenu();
}

void View::Board::populate_maps() {
    wxInitAllImageHandlers();  // Ensure image handlers are active

    // Resolve absolute tile image paths
    wxString execPath = wxStandardPaths::Get().GetExecutablePath();
    wxString baseDir = wxFileName(execPath).GetPath(); // e.g., cmake-build-debug
    wxString projectRoot = wxFileName(baseDir + "/..").GetAbsolutePath();

    wxString lightPath = projectRoot + "/images/board/tiles/set1/light_tile.png";
    wxString darkPath  = projectRoot + "/images/board/tiles/set1/dark_tile.png";

    wxImage lightTile(lightPath, wxBITMAP_TYPE_PNG);
    wxImage darkTile(darkPath, wxBITMAP_TYPE_PNG);

    //Downscaling to 40x40 pixel sizes
    lightTile = lightTile.Scale(TILE_SIZE, TILE_SIZE, wxIMAGE_QUALITY_HIGH);
    darkTile = darkTile.Scale(TILE_SIZE, TILE_SIZE, wxIMAGE_QUALITY_HIGH);

    if (!lightTile.IsOk()) std::cerr << "Failed to load: " << std::string(lightPath.mb_str()) << "\n";
    if (!darkTile.IsOk())  std::cerr << "Failed to load: " << std::string(darkPath.mb_str()) << "\n";

    boardTilesImagesMap_.clear();
    piecesBoardMap_.clear();
    piece_tile_images_.clear();
    boardDisplayReadyMap_.clear();

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            std::pair<int, int> pos = {x, y};
            bool isDark = (x + y) % 2 != 0;
            boardTilesImagesMap_[pos] = isDark ? darkTile : lightTile;
        }
    }

    View& view = *parent_->parent_;
    Controller& ctrl = *view.controller_;
    ctrl.map_all_pieces_to_tiles();

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            std::pair<int, int> pos = {x, y};
            if (piecesBoardMap_.contains(pos)) {
                piece_tile_images_[pos] = view.CombineImagesToBitmap(boardTilesImagesMap_[pos], piecesBoardMap_[pos]);
            }
        }
    }

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            std::pair<int, int> pos = {x, y};
            if (piece_tile_images_.contains(pos)) {
                boardDisplayReadyMap_[pos] = piece_tile_images_[pos];
            } else {
                boardDisplayReadyMap_[pos] = wxBitmap(boardTilesImagesMap_[pos].Copy());
            }
        }
    }
}

void View::GameWindow::initialize_handlers() {
    backButton_->Bind(wxEVT_BUTTON, &View::GameWindow::OnLeaveGameButtonClick, this);
    gameFrame_->Bind(wxEVT_SIZE, &View::GameWindow::OnResize, this);
}

// =======================
//        BoardPanel
// =======================

void View::BoardPanel::OnResize(wxSizeEvent& event) {
    auto size = GetClientSize();
    tileSize_ = wxSize(size.GetWidth() / 8, size.GetHeight() / 8);
    //Refresh is responsible for creating wxEVT_PAINT,
    //informs system that this event has to be handled
    //and the OnPaint comes to handle the event.
    Refresh();
    event.Skip();
}

void View::BoardPanel::OnPaint(wxPaintEvent& event) {
    wxAutoBufferedPaintDC dc(this);

    dc.Clear();

    DrawBoard(dc);
    DrawPieces(dc);
}

void View::BoardPanel::DrawBoard(wxDC& dc) {
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            std::pair<int, int> pos = {x, y};
            //Image to draw
            wxBitmap tile_img = board_->boardTilesImagesMap_.at(pos).Scale(tileSize_.GetWidth(), tileSize_.GetHeight(), wxIMAGE_QUALITY_HIGH);
            dc.DrawBitmap(tile_img, pos.first * tileSize_.GetWidth(), pos.second * tileSize_.GetHeight(), true);
        }
    }
}

void View::BoardPanel::DrawPieces(wxDC& dc) {
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            std::pair<int, int> pos = {x, y};
            wxBitmap img;
            if (board_->piecesBoardMap_.contains(pos)) {
                wxImage piece_img = board_->piecesBoardMap_.at(pos).Scale(tileSize_.GetWidth(), tileSize_.GetHeight(), wxIMAGE_QUALITY_HIGH);
                wxImage tile_img = board_->boardTilesImagesMap_.at(pos).Scale(tileSize_.GetWidth(), tileSize_.GetHeight(), wxIMAGE_QUALITY_HIGH);
                img = View::CombineImagesToBitmap(tile_img, piece_img);
                dc.DrawBitmap(img, pos.first * tileSize_.GetWidth(), pos.second * tileSize_.GetHeight(), true);
            } else {
                //Image to draw
                img = board_->boardTilesImagesMap_.at(pos).Scale(tileSize_.GetWidth(), tileSize_.GetHeight(), wxIMAGE_QUALITY_HIGH);
                dc.DrawBitmap(img, pos.first * tileSize_.GetWidth(), pos.second * tileSize_.GetHeight(), true);
                //dc.DrawBitmap(img, pos.first, pos.second, true);
            }
        }
    }
}
