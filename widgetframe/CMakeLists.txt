project(WidgetFrame)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB _src *.h *.cpp)

add_library(${PROJECT_NAME} STATIC
    ${_src}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(${PROJECT_NAME} PUBLIC . ..)

target_compile_definitions(${PROJECT_NAME} PRIVATE WIDGETFRAME_LIBRARY)
