cmake_minimum_required(VERSION 3.19)
project(c501checkers)

set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)


include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
# Add other flags to the compiler
add_definitions(${GTK3_CFLAGS_OTHER})

find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})

find_package(SFML 2 REQUIRED COMPONENTS graphics window)
include_directories(${SFML_INCLUDE_DIR})


include_directories(./src/checkers)

find_package(wxWidgets REQUIRED gl core base adv)
include(${wxWidgets_USE_FILE})

add_executable(c501checkers
        src/AppConfig.hpp
        src/config.hpp
        src/config.hpp.in
        src/DialogAbout.cpp
        src/DialogAbout.hpp
        src/DialogConfig.cpp
        src/DialogConfig.hpp
        src/findFile.cpp
        src/findFile.hpp
        src/MainFrame.cpp
        src/MainFrame.hpp
        src/MainPanel.cpp
        src/MainPanel.hpp
        src/SfmlBoard.cpp
        src/SfmlBoard.hpp
        src/strWxStdConv.cpp
        src/strWxStdConv.hpp
        src/wxSFMLCanvas.cpp
        src/wxSFMLCanvas.hpp
        src/checkers/Board.cpp
        src/checkers/Board.hpp
        src/checkers/Mvt.cpp
        src/checkers/Mvt.hpp
        src/checkers/MyException.cpp
        src/checkers/MyException.hpp
        src/checkers/nbStrConvert.hpp
        src/App.cpp
        src/App.hpp
)

target_link_libraries(c501checkers PRIVATE ${wxWidgets_LIBRARIES} ${GTK3_LIBRARIES} ${X11_LIBRARIES} sfml-window sfml-graphics)