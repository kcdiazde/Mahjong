#include "tiles.h"

TileId MahjongTile::unique_id_ = 0;

// TODO: Add destructors

// Constructor
MahjongSet::MahjongSet() {
    CreateFlowerTiles();
    CreateDragonTiles();
    CreateWindTiles();
    CreateSymbolTiles();
    CreateBambooTiles();
    CreateDotTiles();
}

void MahjongSet::CreateTiles(TileGroup group, std::list<std::string> names) {
    // Check that the number of tiles can be completed with provided tiles
    int tiles_incomplete = group.num_tiles_in_set % names.size();
    assert(tiles_incomplete == 0);

    // The number of times each tile repeats based on the total of the group
    int num_repeated_tiles = group.num_tiles_in_set / names.size();

    // Create each tile name and add it to the set num_repeated_tiles times'
    for (auto tile_name : names) {
        auto tile = std::make_shared<MahjongTile>(group, tile_name);
        for (int num_tiles = 0; num_tiles < num_repeated_tiles; num_tiles++) {
            mahjong_set_.push_back(tile);
        }
    }
}

void MahjongSet::CreateNumberedTiles(TileGroup group, int max_num_tile) {
    // Check that the number of tiles can be completed with provided tiles
    int tiles_incomplete = group.num_tiles_in_set % max_num_tile;
    assert(tiles_incomplete == 0);

    // The number of times each tile repeats based on the total of the group
    int num_repeated_tiles = group.num_tiles_in_set / max_num_tile;

    // Create each tile name and add it to the set num_repeated_tiles times'
    for (int tile_num = 0; tile_num < max_num_tile; tile_num++) {
        int real_tile_num = tile_num + 1;
        auto tile =
            std::make_shared<MahjongTileNumerical>(group, real_tile_num);
        for (int num_tiles = 0; num_tiles < num_repeated_tiles; num_tiles++) {
            mahjong_set_.push_back(tile);
        }
    }
}

void MahjongSet::CreateDragonTiles() {
    std::list<std::string> dragons = {"Red", "White", "Green"};
    CreateTiles(kDragon, dragons);
}

void MahjongSet::CreateWindTiles() {
    std::list<std::string> winds = {"North", "South", "East", "West"};
    CreateTiles(kWind, winds);
}

void MahjongSet::CreateFlowerTiles() {
    std::list<std::string> flowers = {"Winter", "Autumn", "Spring", "Fall"};
    CreateTiles(kFlower, flowers);
}

void MahjongSet::CreateSymbolTiles() { CreateNumberedTiles(kSymbol, 9); }
void MahjongSet::CreateBambooTiles() { CreateNumberedTiles(kBamboo, 9); }
void MahjongSet::CreateDotTiles() { CreateNumberedTiles(kDot, 9); }

void MahjongSet::Print() {
    printf("******Mahjong set******\n");
    for (auto tile : mahjong_set_) {
        tile->Print();
    }
    printf("Number of tiles: %i\n\n", int(mahjong_set_.size()));
}

void MahjongSet::Shuffle() {
    std::random_device rd;
    std::mt19937 gen(rd());

    // Shuffle
    std::shuffle(mahjong_set_.begin(), mahjong_set_.end(), rd);
}

MahjongTilePtr MahjongSet::TakeTile() {
    Logger::Instance().Debug("Tiles left %d", mahjong_set_.size());
    MahjongTilePtr tile_to_remove = mahjong_set_.front();
    mahjong_set_.erase(mahjong_set_.begin());
    return tile_to_remove;
}

bool MahjongTile::TilesCustomComparator(const MahjongTilePtr a,
                                        const MahjongTilePtr b) {
    return a->GetId() < b->GetId();
}

TileId MahjongTile::GetUniqueId() {
    TileId current_id = unique_id_++;
    return current_id;
}
