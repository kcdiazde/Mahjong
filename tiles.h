#ifndef MAHJONG_H
#define MAHJONG_H

#include <stdio.h>
#include<string>
#include<list>
#include<cassert>
#include<vector>
#include <algorithm>
#include <random>
#include <chrono>

#define TILES_IN_SET 144

struct TileGroup {
    std::string name;
    bool has_a_number;
    int num_tiles_in_set;
};

class MahjongTile {
protected:
    TileGroup _tile_group;
    std::string _name;

public:
    // TODO: Clean constructors
    explicit MahjongTile() {}
    explicit MahjongTile(TileGroup tile_group, std::string name):
                         _tile_group(tile_group), _name(name) {}

    void print() {
        printf("%s %s\n", _name.c_str(), _tile_group.name.c_str());
    }
};

class MahjongTileNumerical : public MahjongTile{
protected:
    int _value;

public:
    // TODO: Clean constructors
    explicit MahjongTileNumerical() {}
    explicit MahjongTileNumerical(TileGroup tile_group, int value) {
        _tile_group = tile_group;
        _value = value;
        _name = std::to_string(value);
    }
};

class MahjongSet {
public:
    explicit MahjongSet();
    void print();
    void shuffle();

protected:
    std::vector<MahjongTile*> _mahjong_set ;

    const TileGroup DRAGON = TileGroup{.name = "Dragon", .has_a_number = false, .num_tiles_in_set = 12};
    const TileGroup WIND = TileGroup{.name = "Wind", .has_a_number = false, .num_tiles_in_set = 16};
    const TileGroup FLOWER = TileGroup{.name = "Flower", .has_a_number = false, .num_tiles_in_set = 8};
    const TileGroup SYMBOL = TileGroup{.name = "Symbol", .has_a_number = true, .num_tiles_in_set = 36};
    const TileGroup BAMBOO = TileGroup{.name = "Bamboo", .has_a_number = true, .num_tiles_in_set = 36};
    const TileGroup DOT = TileGroup{.name = "Dot", .has_a_number = true, .num_tiles_in_set = 36};
    
    void create_tiles(TileGroup group, std::list<std::string> names);
    void create_numbered_tiles(TileGroup group, int max_num_tile);
    void create_dragon_tiles();
    void create_wind_tiles();
    void create_flower_tiles();
    void create_symbol_tiles();
    void create_bamboo_tiles();
    void create_dot_tiles();

};

#endif