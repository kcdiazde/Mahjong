#ifndef MAHJONG_COMMON_H
#define MAHJONG_COMMON_H

#include <string>

#define TILES_IN_SET 144
#define TILES_PER_PLAYER 13
#define MIN_NUM_PLAYERS 2
#define MAX_NUM_PLAYERS 4

typedef uint8_t TileId;

struct TileGroup {
    std::string name;
    bool has_a_number;
    uint8_t num_tiles_in_set;
    uint8_t priority;
};

const TileGroup FLOWER = TileGroup{.name = "Flower",
                                   .has_a_number = false,
                                   .num_tiles_in_set = 8,
                                   .priority = 0};
const TileGroup DRAGON = TileGroup{.name = "Dragon",
                                   .has_a_number = false,
                                   .num_tiles_in_set = 12,
                                   .priority = 1};
const TileGroup WIND = TileGroup{.name = "Wind",
                                 .has_a_number = false,
                                 .num_tiles_in_set = 16,
                                 .priority = 2};
const TileGroup SYMBOL = TileGroup{.name = "Symbol",
                                   .has_a_number = true,
                                   .num_tiles_in_set = 36,
                                   .priority = 3};
const TileGroup BAMBOO = TileGroup{.name = "Bamboo",
                                   .has_a_number = true,
                                   .num_tiles_in_set = 36,
                                   .priority = 4};
const TileGroup DOT = TileGroup{.name = "Dot",
                                .has_a_number = true,
                                .num_tiles_in_set = 36,
                                .priority = 5};

#endif