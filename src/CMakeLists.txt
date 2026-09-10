add_library(can_reader_lib INTERFACE)
target_include_directories(can_reader_lib INTERFACE .)
#target_include_directories(can_reader_lib PUBLIC .)

add_executable(can_reader
        main.cpp
)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        can_reader_lib
        CLI11::CLI11
        spdlog::spdlog
)