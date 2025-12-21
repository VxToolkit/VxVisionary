cmake_minimum_required(VERSION 4.0)
project(VxtVisionary)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets Quick REQUIRED)

qt_standard_project_setup()

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_add_executable(VxtVisionary
    src/Main.cpp
    models/ProjectSelector/ProjectModel.cpp
    models/ProjectSelector/ProjectModel.hpp
    src/AppController.cpp
    src/AppController.h
    models/Project.cpp
    models/Project.hpp
    src/Editors/vxEditor.cpp
    src/Editors/vxEditor.hpp
    models/Asset.cpp
    models/Asset.hpp
    models/ArenaAsset.cpp
    models/ArenaAsset.hpp
    models/ArenaElements.cpp
    models/ArenaElements.hpp
    models/CopyUtils.cpp
    models/CopyUtils.hpp
    src/Editors/ArenaEditor.cpp
    src/Editors/ArenaEditor.hpp
    models/Assets/AssetFilterModel.cpp
    models/Assets/AssetFilterModel.hpp
    models/Assets/AssetModel.cpp
    models/Assets/AssetModel.hpp
)

target_include_directories(VxtVisionary PRIVATE src)

add_subdirectory(template/vex2025)
include_directories(template/vex2025/src/lib)

qt_add_qml_module(VxtVisionary
    URI "Main"
    VERSION 1.0

    SOURCES
    src/AppController.cpp
    src/AppController.h

    QML_FILES
    content/Main.qml
    content/components/VxButton.qml
    content/ProjectDashboard.qml
    content/components/VxWorkspaceSelector.qml
    content/components/VxEditorToolbar.qml
    content/components/VxToolButton.qml
    content/ArenaEditor.qml
    content/components/VxListView.qml
    content/components/VxAssetDialog.qml
    content/components/VxTabList.qml

    RESOURCES
    assets/VXVisionarySplash.png
    assets/VXT-Logo-Large.png
    assets/Logo.png
    assets/Workspaces/arena-editor.png
    assets/Workspaces/mission-editor.png
    assets/Workspaces/config.png
    assets/Workspaces/auto-journal.png
    assets/Workspaces/build.png
    assets/Workspaces/quick-build.png
    assets/Icons/save-icon.png
    assets/Icons/new-file.png
    assets/Icons/open-file.png

)

target_link_libraries(VxtVisionary
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Widgets
    PRIVATE Qt6::Gui
    PRIVATE Qt6::Core
    PRIVATE VxTemplate
)