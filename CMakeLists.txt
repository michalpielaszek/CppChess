cmake_minimum_required(VERSION 3.15)
project(imlosinghope)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
        ${CMAKE_SOURCE_DIR}/src/controller
        ${CMAKE_SOURCE_DIR}/src/model
        ${CMAKE_SOURCE_DIR}/src/view
        ${CMAKE_SOURCE_DIR}/external_UI_framework/include
)

# Link directories for wxWidgets
link_directories(${CMAKE_SOURCE_DIR}/external_UI_framework/lib)

# Source files
add_executable(imlosinghope
        src/main.cpp
        src/controller/Controller.cpp
        src/model/GameModel.cpp
        src/view/View.cpp
        src/model/Piece.cpp
        src/model/Piece.h
        src/model/Player.cpp
        src/model/Player.h
        src/model/Rook.cpp
        src/model/Rook.h
        src/model/Knight.cpp
        src/model/Knight.h
        src/model/Bishop.cpp
        src/model/Bishop.h
        src/model/King.cpp
        src/model/King.h
        src/model/Queen.cpp
        src/model/Queen.h
        src/model/Pawn.cpp
        src/model/Pawn.h
)

# Link wxWidgets only for the View (not Controller or Model)
target_link_libraries(imlosinghope
        wxmsw32u_core
        wxbase32u
        wxpng
        wxzlib
        wxregexu
        comctl32
        shlwapi
        uxtheme
        oleacc
        version
        kernel32
        user32
        gdi32
        winspool
        shell32
        ole32
        oleaut32
        comdlg32
        advapi32

        # ------------------ Required for JPEG/TIFF image support ------------------
        wxjpeg
        wxtiff
)

# Allow multiple definitions to work around the duplicate IID_IAccessible conflict
target_link_options(imlosinghope PRIVATE "-Wl,--allow-multiple-definition")

