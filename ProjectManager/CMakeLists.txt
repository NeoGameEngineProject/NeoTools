cmake_minimum_required(VERSION 2.8.11)

project(ProjectManager)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)

QT5_WRAP_UI(UI_HEADERS
	mainwindow.ui
	newprojectwizard.ui
)

add_executable(ProjectManager main.cpp mainwindow.cpp Backend.cpp ${UI_HEADERS})
target_compile_features(ProjectManager PRIVATE cxx_range_for)

target_link_libraries(ProjectManager Qt5::Widgets)

install(TARGETS ProjectManager DESTINATION ${INSTALL_DIR})

