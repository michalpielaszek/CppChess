//
// Created by mpiel on 19.03.2025.
//
#pragma once

#include <wx/wx.h>
#include <map>
#include <memory>  // For std::unique_ptr if needed

#ifndef VIEW_H
#define VIEW_H

class Controller;

class View final : public wxApp {
public:
    class Menu;
    class Settings;
    class GameWindow;
    class Board;

    Menu* menu_ = nullptr;
    Settings* settings_ = nullptr;
    GameWindow* gameWindow_ = nullptr;
    Board* board_ = nullptr;
    Controller* controller_ = nullptr;

    View() = default;

    void setController(Controller* controller) { controller_ = controller; }

    bool OnInit() override;

    void ShowMenu();
    void ShowSettings();
    void ShowGameWindow();

    static int start(int argc, char** argv, View& view);
    static wxBitmap ConvertToBitmap(const wxImage& image);
    static wxBitmap CombineImagesToBitmap(const wxImage& tile, const wxImage& piece);

    // =======================
    //         Menu
    // =======================
    class Menu {
    public:
        wxFrame* menuFrame_ = nullptr;
        wxPanel* menuPanel_ = nullptr;
        wxButton* startGameButton_ = nullptr;
        wxButton* settingsButton_ = nullptr;
        wxButton* exitButton_ = nullptr;
        wxButton* scoreboardButton_ = nullptr;
        wxStaticText* title_ = nullptr;

        wxBoxSizer* mainSizerV_ = nullptr;
        wxBoxSizer* secondarySizerH_ = nullptr;

        View* parent_ = nullptr;

        explicit Menu(View* parent) : parent_(parent) {}

        void initialize_frame();
        void OnStartGamePressed(wxCommandEvent& event);
        void OnSettingsPressed(wxCommandEvent& event);
        void OnExitPressed(wxCommandEvent& event);
    };

    // =======================
    //        Settings
    // =======================
    class Settings {
    public:
        wxFrame* settingsFrame_ = nullptr;
        wxPanel* settingsPanel_ = nullptr;
        wxButton* leaveSettingsButton_ = nullptr;
        wxStaticText* title_ = nullptr;

        wxBoxSizer* mainSizerV_ = nullptr;
        wxBoxSizer* secondarySizerH_ = nullptr;

        View* parent_ = nullptr;

        explicit Settings(View* parent) : parent_(parent) {}

        void initialize_frame();
        void OnLeaveSettingsButtonClick(wxCommandEvent& event);
    };

    // =======================
    //       GameWindow
    // =======================
    class GameWindow {
    public:
        wxFrame* gameFrame_ = nullptr;
        wxPanel* gamePanel_ = nullptr;
        wxPanel* boardPanel_ = nullptr;
        wxPanel* infoPanel_ = nullptr;

        wxBoxSizer* frontLayout_ = nullptr;
        wxBoxSizer* infoLayout_ = nullptr;
        wxBoxSizer* secondarySizerH_ = nullptr;

        wxGridSizer* gameGrid_ = nullptr;

        wxStaticText* player_1_info_ = nullptr;
        wxStaticText* player_2_info_ = nullptr;
        wxStaticText* timer_ = nullptr;
        wxStaticText* title_ = nullptr;

        wxButton* player_1_resign_ = nullptr;
        wxButton* player_2_resign_ = nullptr;
        wxButton* player_1_undo = nullptr;
        wxButton* player_2_undo = nullptr;
        wxButton* reverseBoard_ = nullptr;
        wxButton* backButton_ = nullptr;

        Board* board_;
        View* parent_ = nullptr;

        GameWindow(View *parent) : parent_(parent), board_(nullptr) {
            // initialize other members to nullptr too if needed
        }

        void initialize_frame();
        void OnLeaveGameButtonClick(wxCommandEvent& event);
    };

    // =======================
    //          Board
    // =======================
    class Board {
    public:
        GameWindow* parent_ = nullptr;

        std::map<std::pair<int, int>, wxImage> boardTilesImagesMap_;
        std::map<std::pair<int, int>, wxImage> piecesBoardMap_;
        std::map<std::pair<int, int>, wxBitmap> piece_tile_images_;
        std::map<std::pair<int, int>, wxBitmap> boardDisplayReadyMap_;

        Board(GameWindow *parent) : parent_(parent) {
            std::cout << "Board constructor called, parent: " << parent_ << "\n";
        }

        void populate_maps();
    };
};

#endif // VIEW_H

