#ifndef TILES_CC
#define TILES_CC

#include "tiles.h"

uint8_t MahjongTile::_unique_id = 0;

// TODO: Add destructors

// Constructor
MahjongSet::MahjongSet() {
    create_flower_tiles();
    create_dragon_tiles();
    create_wind_tiles();
    create_symbol_tiles();
    create_bamboo_tiles();
    create_dot_tiles();
}

void MahjongSet::create_tiles(TileGroup group, std::list<std::string> names) {
    // Check that the number of tiles can be completed with provided tiles
    int tiles_incomplete = group.num_tiles_in_set % names.size();
    assert(tiles_incomplete == 0);

    // The number of times each tile repeats based on the total of the group
    int num_repeated_tiles = group.num_tiles_in_set / names.size();

    // Create each tile name and add it to the set num_repeated_tiles times'
    for (auto tile_name : names) {
        auto tile = std::make_shared<MahjongTile>(group, tile_name);
        for (int num_tiles = 0; num_tiles < num_repeated_tiles; num_tiles++) {
            _mahjong_set.push_back(tile);
        }
    }
}

void MahjongSet::create_numbered_tiles(TileGroup group, int max_num_tile) {
    // Check that the number of tiles can be completed with provided tiles
    int tiles_incomplete = group.num_tiles_in_set % max_num_tile;
    assert(tiles_incomplete == 0);

    // The number of times each tile repeats based on the total of the group
    int num_repeated_tiles = group.num_tiles_in_set / max_num_tile;

    // Create each tile name and add it to the set num_repeated_tiles times'
    for (int tile_num = 0; tile_num < max_num_tile; tile_num++) {
        int real_tile_num = tile_num + 1;
        auto tile = std::make_shared<MahjongTileNumerical>(group, real_tile_num);
        for (int num_tiles = 0; num_tiles < num_repeated_tiles; num_tiles++) {
            _mahjong_set.push_back(tile);
        }
    }
}

void MahjongSet::create_dragon_tiles() {
    std::list<std::string> dragons = {"Red", "White", "Green"};
    create_tiles(DRAGON, dragons);
}

void MahjongSet::create_wind_tiles() {
    std::list<std::string> winds = {"North", "South", "East", "West"};
    create_tiles(WIND, winds);
}

void MahjongSet::create_flower_tiles() {
    std::list<std::string> flowers = {"Winter", "Autumn", "Spring", "Fall"};
    create_tiles(FLOWER, flowers);
}

void MahjongSet::create_symbol_tiles() { create_numbered_tiles(SYMBOL, 9); }
void MahjongSet::create_bamboo_tiles() { create_numbered_tiles(BAMBOO, 9); }
void MahjongSet::create_dot_tiles() { create_numbered_tiles(DOT, 9); }

void MahjongSet::print() {
    printf("******Mahjong set******\n");
    for (auto tile : _mahjong_set) {
        tile->print();
    }
    printf("Number of tiles: %i\n\n", int(_mahjong_set.size()));
}

void MahjongSet::shuffle() {
    std::random_device rd;
    std::mt19937 gen(rd());

    // Shuffle
    std::shuffle(_mahjong_set.begin(), _mahjong_set.end(), rd);
}

MahjongTilePtr MahjongSet::take_tile() {
    Logger::instance().debug("Tiles left %d", _mahjong_set.size());
    MahjongTilePtr tile_to_remove = _mahjong_set.front();
    _mahjong_set.erase(_mahjong_set.begin());
    return tile_to_remove;
}

bool MahjongTile::tilesCustomComparator(const MahjongTilePtr a,
                                        const MahjongTilePtr b) {
    return a->get_id() < b->get_id();
}

uint8_t MahjongTile::get_unique_id() {
    uint8_t current_id = _unique_id++;
    return current_id;
}

#endif
