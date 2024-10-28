project(mainLoopWDT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(${PROJECT_NAME} SHARED
	mainLoopWdt.h
	mainLoopWdt.cpp
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
	Qt${QT_VERSION_MAJOR}::Core
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	WINDOWS_EXPORT_ALL_SYMBOLS ON  # For Windows DLLs if symbols are needed
)
