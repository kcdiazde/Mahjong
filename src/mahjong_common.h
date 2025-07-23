#ifndef MAHJONG_COMMON_H
#define MAHJONG_COMMON_H

#include <string>

constexpr int kTilesInSet = 144;
constexpr int kTilesPerPlayer = 13;
constexpr int kMinNumPlayers = 2;
constexpr int kMaxNumPlayers = 4;
constexpr int kTilesInPung = 3;
constexpr int kNumTilesToPass = 3;

using TileId = uint8_t;

struct TileGroup {
  std::string name;
  bool has_a_number;
  uint8_t num_tiles_in_set;
  uint8_t priority;
};

const TileGroup kFlower = TileGroup{.name = "Flower",
                                    .has_a_number = false,
                                    .num_tiles_in_set = 8,
                                    .priority = 0};
const TileGroup kDragon = TileGroup{.name = "Dragon",
                                    .has_a_number = false,
                                    .num_tiles_in_set = 12,
                                    .priority = 1};
const TileGroup kWind = TileGroup{.name = "Wind",
                                  .has_a_number = false,
                                  .num_tiles_in_set = 16,
                                  .priority = 2};
const TileGroup kSymbol = TileGroup{.name = "Symbol",
                                    .has_a_number = true,
                                    .num_tiles_in_set = 36,
                                    .priority = 3};
const TileGroup kBamboo = TileGroup{.name = "Bamboo",
                                    .has_a_number = true,
                                    .num_tiles_in_set = 36,
                                    .priority = 4};
const TileGroup kDot = TileGroup{.name = "Dot",
                                 .has_a_number = true,
                                 .num_tiles_in_set = 36,
                                 .priority = 5};

#endif
