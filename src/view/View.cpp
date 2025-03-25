#include "View.h"
#include "Controller.h"

#include <wx/image.h>
#include <iostream>
#include <wx/filename.h>
#include <wx/stdpaths.h>

int View::start(int argc, char** argv, View& view) {
    return wxEntry(argc, argv);
}

wxBitmap View::ConvertToBitmap(const wxImage& image) {
    return wxBitmap(image);
}

wxBitmap View::CombineImagesToBitmap(const wxImage& tileImage, const wxImage& pieceImage) {
    wxImage combined = tileImage.Copy();

    if (tileImage.GetSize() != pieceImage.GetSize()) {
        wxImage resizedPiece = pieceImage.Scale(tileImage.GetWidth(), tileImage.GetHeight(), wxIMAGE_QUALITY_HIGH);
        combined.Paste(resizedPiece, 0, 0);
    } else {
        combined.Paste(pieceImage, 0, 0);
    }

    return wxBitmap(combined);
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

    board_ = new Board(gameWindow_);     // board's parent set correctly here
    gameWindow_->board_ = board_;        // gameWindow's board_ is set here

    menu_->initialize_frame();
    settings_->initialize_frame();
    gameWindow_->initialize_frame();

    board_->populate_maps();             // ✅ now it's safe to call this!

    ShowMenu();
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
    gameFrame_ = new wxFrame(nullptr, wxID_ANY, "Game", wxDefaultPosition, wxSize(1200, 675));
    gamePanel_ = new wxPanel(gameFrame_, wxID_ANY);
    frontLayout_ = new wxBoxSizer(wxHORIZONTAL);
    gamePanel_->SetSizer(frontLayout_);

    boardPanel_ = new wxPanel(gamePanel_, wxID_ANY);
    gameGrid_ = new wxGridSizer(8, 0, 0);
    boardPanel_->SetSizer(gameGrid_);

    Board* board = parent_->board_;
    if (!board) {
        std::cerr << "[ERROR] board_ is null!\n";
        return;
    }

    board->populate_maps();

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            std::pair<int, int> pos = {x, y};
            gameGrid_->Add(new wxStaticBitmap(boardPanel_, wxID_ANY, board->boardDisplayReadyMap_[pos]));
        }
    }

    frontLayout_->Add(boardPanel_, 3, wxEXPAND | wxALL, 5);

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

    backButton_->Bind(wxEVT_BUTTON, &View::GameWindow::OnLeaveGameButtonClick, this);

    gameFrame_->Show(false);
}

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







