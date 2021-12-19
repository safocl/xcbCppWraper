project(xcbwraper LANGUAGES CXX)
add_library(${PROJECT_NAME})
file (GLOB cpps  *.cpp)

target_sources(${PROJECT_NAME} PRIVATE ${cpps})
target_link_libraries(${PROJECT_NAME} PUBLIC xcb)
target_link_libraries(${PROJECT_NAME} PUBLIC xcb-composite)
target_link_libraries(${PROJECT_NAME} PUBLIC xcb-shm)
target_link_libraries(${PROJECT_NAME} PUBLIC xcb-util)
