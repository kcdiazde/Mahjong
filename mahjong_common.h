#ifndef MAHJONG_COMMON_H
#define MAHJONG_COMMON_H

#include <string>

#define TILES_IN_SET 144
#define TILES_PER_PLAYER 13

struct TileGroup {
    std::string name;
    bool has_a_number;
    int num_tiles_in_set;
};

const TileGroup DRAGON = TileGroup{.name = "Dragon", .has_a_number = false, .num_tiles_in_set = 12};
const TileGroup WIND = TileGroup{.name = "Wind", .has_a_number = false, .num_tiles_in_set = 16};
const TileGroup FLOWER = TileGroup{.name = "Flower", .has_a_number = false, .num_tiles_in_set = 8};
const TileGroup SYMBOL = TileGroup{.name = "Symbol", .has_a_number = true, .num_tiles_in_set = 36};
const TileGroup BAMBOO = TileGroup{.name = "Bamboo", .has_a_number = true, .num_tiles_in_set = 36};
const TileGroup DOT = TileGroup{.name = "Dot", .has_a_number = true, .num_tiles_in_set = 36};

#endif