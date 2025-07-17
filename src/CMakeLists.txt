include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

set(SOURCES
    bot.cc
    mahjong.cc
    player.cc
    tiles.cc
)

# add_executable(main ${SOURCES})
add_library(MahjongLib STATIC ${SOURCES})

add_executable(MahjongMain main.cc)

target_include_directories(MahjongMain
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

set(MAHJONG_TOOLS "${PROJECT_SOURCE_DIR}/tools")
add_compile_definitions(MAHJONG_TOOLS="${MAHJONG_TOOLS}")

target_compile_features(MahjongMain PRIVATE cxx_std_17)
target_link_libraries(MahjongMain PRIVATE SFML::Graphics SFML::Audio MahjongLib)

