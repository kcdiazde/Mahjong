#ifndef TILES_H
#define TILES_H

#include <stdio.h>
#include <string>
#include <list>
#include <cassert>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include "mahjong_common.h"

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

    const TileGroup get_group() {return _tile_group;}
    bool is_flower() {return _tile_group.name == FLOWER.name;}
    std::string get_name() {return _name;}
};

class MahjongTileNumerical : public MahjongTile{
protected:
    uint8_t _value;

public:
    // TODO: Clean constructors
    explicit MahjongTileNumerical() {}
    explicit MahjongTileNumerical(TileGroup tile_group, int value) {
        _tile_group = tile_group;
        _value = value;
        _name = std::to_string(value);
    }
    
    uint8_t get_value() {return _value;}
};

class MahjongSet {
public:
    explicit MahjongSet();
    void print();
    void shuffle();
    MahjongTile* take_tile();

protected:
    std::vector<MahjongTile*> _mahjong_set ;
    
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