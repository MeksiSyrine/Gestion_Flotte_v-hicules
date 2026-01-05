cmake_minimum_required(VERSION 3.16)

project(FlotteDeliveryQt VERSION 1.0 LANGUAGES CXX)

# Configuration C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configuration Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Trouver Qt6 (ou Qt5 en fallback)
find_package(Qt6 COMPONENTS Core Widgets QUIET)
if (NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
    set(QT_VERSION_MAJOR 5)
else()
    set(QT_VERSION_MAJOR 6)
endif()

# Fichiers sources du CORE (logique métier - code existant)
set(CORE_SOURCES
    Colis.cpp
    ColisStandard.cpp
    ColisFragile.cpp
    ColisRefrigere.cpp
    ColisExpress.cpp
    Vehicule.cpp
    Drone.cpp
    CamionnetteElectrique.cpp
    RobotTrottoir.cpp
    Flotte.cpp
)

set(CORE_HEADERS
    Colis.h
    ColisStandard.h
    ColisFragile.h
    ColisRefrigere.h
    ColisExpress.h
    Vehicule.h
    Drone.h
    CamionnetteElectrique.h
    RobotTrottoir.h
    Flotte.h
)

# Fichiers sources de la GUI (interface Qt)
set(GUI_SOURCES
    gui/main_qt.cpp
    gui/mainwindow.cpp
    gui/dialogcreervehicule.cpp
    gui/dialogcreercolis.cpp
    gui/dialogdetailsvehicule.cpp
)

set(GUI_HEADERS
    gui/mainwindow.h
    gui/dialogcreervehicule.h
    gui/dialogcreercolis.h
    gui/dialogdetailsvehicule.h
)

set(GUI_UI
    gui/mainwindow.ui
    gui/dialogcreervehicule.ui
    gui/dialogcreercolis.ui
    gui/dialogdetailsvehicule.ui
)

# Créer l'exécutable Qt
add_executable(flotte_qt
    ${CORE_SOURCES}
    ${CORE_HEADERS}
    ${GUI_SOURCES}
    ${GUI_HEADERS}
    ${GUI_UI}
)

# Lier Qt
if (QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(flotte_qt PRIVATE Qt6::Core Qt6::Widgets)
else()
    target_link_libraries(flotte_qt PRIVATE Qt5::Core Qt5::Widgets)
endif()

# Compiler aussi la version console (optionnel)
add_executable(flotte_console
    ${CORE_SOURCES}
    ${CORE_HEADERS}
    main.cpp
)

# Configuration pour Windows (éviter la console en mode Release)
if(WIN32)
    set_target_properties(flotte_qt PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Messages informatifs
message(STATUS "Configuration du projet Flotte de Livraison")
message(STATUS "  - Qt Version: ${QT_VERSION_MAJOR}")
message(STATUS "  - C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  - Build Type: ${CMAKE_BUILD_TYPE}")
