project(testApp LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)

set(SOURCES
	main.cpp
	main.qml
	utils.cpp
	utils.h
)
if(${QT_VERSION_MAJOR} EQUAL "6" )
	qt_add_executable(${PROJECT_NAME}
		${SOURCES}
		qml.qrc
	)
else()
	add_executable(${PROJECT_NAME}
		${SOURCES}
		qml.qrc
	)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ../mainLoopWdt)

target_link_libraries(${PROJECT_NAME} PRIVATE
	Qt${QT_VERSION_MAJOR}::Core
	Qt${QT_VERSION_MAJOR}::Quick
)

target_link_libraries(${PROJECT_NAME} PRIVATE mainLoopWDT)
