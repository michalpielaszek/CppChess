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
    class BoardPanel;
    class GameWindow {
    public:
        wxFrame* gameFrame_ = nullptr;
        wxPanel* gamePanel_ = nullptr;
        BoardPanel* boardPanel_ = nullptr;
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

        bool resizeReady_ = false;

        GameWindow(View *parent) : parent_(parent), board_(nullptr) {
            // initialize other members to nullptr too if needed
        }

        void initialize_frame();
        void OnLeaveGameButtonClick(wxCommandEvent& event);
        void OnResize(wxSizeEvent& event);
        void initialize_handlers();
        /*
        void size_tiles_update(int x, int y);

        void refresh_board_display();
        */
    };

    // =======================
    //          Board
    // =======================
    class Board {
    public:
        GameWindow* parent_ = nullptr;
        int size_;

        std::map<std::pair<int, int>, wxImage> boardTilesImagesMap_;
        std::map<std::pair<int, int>, wxImage> piecesBoardMap_;
        std::map<std::pair<int, int>, wxBitmap> piece_tile_images_;
        std::map<std::pair<int, int>, wxBitmap> boardDisplayReadyMap_;

        Board(GameWindow *parent) : parent_(parent) {
            std::cout << "Board constructor called, parent: " << parent_ << "\n";
        }

        void populate_maps();

        int getSize() {
            return size_;
        }

        void setSize(int size) {
            size_ = size;
        }
    };

    // =======================
    //        BoardPanel
    // =======================

    /*
     * This class is for custom rendering, since current / previous approach included
     * refactoring and applying images to the Panels on the run, during resize and was extremely slow.
     * My suspicion is that this approach is good for statically drawing images and themes once on start,
     * terrible for resize in short time.
     * Now idea is to hand printing all images on the run to probably better dedicated tools and
     * BoardPanel comes in handy since all event handling and drawing can be contained in one class
     */

    class BoardPanel : public wxPanel {
    public:
        Board* board_ = nullptr;
        BoardPanel(wxPanel* parent, Board* board_) : wxPanel(parent), board_(board_) {
            Bind(wxEVT_PAINT, &BoardPanel::OnPaint, this);
            Bind(wxEVT_SIZE, &BoardPanel::OnResize, this);
            SetBackgroundStyle(wxBG_STYLE_PAINT);
        };
        void OnPaint(wxPaintEvent& event);
        void OnResize(wxSizeEvent& event);

    private:
        /*  Note for OnPaint
         *  It is called each time resize, or in fact any action requiring repaint.
         *  We need to provide custom set of instructions to handle the process ourselves
         *  Bind must be placed in constructor so that we instantly tell wxWidgets' drawing system
         *  that we handle printing and resizing before system assign its own methods
         */
        void DrawBoard(wxDC& dc);
        void DrawPieces(wxDC& dc);

        wxSize tileSize_; // dynamiczny rozmiar kafelka
    };
};

#endif // VIEW_H

