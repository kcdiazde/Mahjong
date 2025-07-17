include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

set(SOURCES
    main.cc
    bot.cc
    mahjong.cc
    player.cc
    tiles.cc
)

add_executable(main ${SOURCES})

target_include_directories(main
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

set(MAHJONG_TOOLS "${PROJECT_SOURCE_DIR}/tools")
add_compile_definitions(MAHJONG_TOOLS="${MAHJONG_TOOLS}")

target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio)
