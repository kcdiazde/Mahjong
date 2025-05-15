#ifndef TILES_H
#define TILES_H

#include "mahjong_common.h"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <list>
#include <random>
#include <stdio.h>
#include <string>
#include <vector>

class MahjongTile {
  protected:
    static TileId _unique_id;

    TileGroup _tile_group;
    std::string _name;
    TileId _id;

    static TileId get_unique_id();

  public:
    // TODO: Clean constructors
    explicit MahjongTile() {}
    explicit MahjongTile(TileGroup tile_group, std::string name)
        : _tile_group(tile_group), _name(name) {
          _id = get_unique_id();
        }
    // Virtual destructor to make class polymorphic
    virtual ~MahjongTile() = default;

    void print() { printf("%s %s [ID: %d]\n", _name.c_str(), _tile_group.name.c_str(), _id); }

    const TileGroup get_group() const { return _tile_group; }
    bool is_flower() { return _tile_group.name == FLOWER.name; }
    const std::string get_name() const { return _name; }
    const std::string get_full_name() const { return _name + " " + _tile_group.name.c_str(); }
    const TileId get_id() const {return _id;}
    static bool tilesCustomComparator(const MahjongTile *a,
                                      const MahjongTile *b);
};

class MahjongTileNumerical : public MahjongTile {
  protected:
    uint8_t _value;

  public:
    // TODO: Clean constructors
    explicit MahjongTileNumerical() {}
    explicit MahjongTileNumerical(TileGroup tile_group, int value) {
        _tile_group = tile_group;
        _value = value;
        _name = std::to_string(value);
        _id = get_unique_id();
    }

    uint8_t get_value() const { return _value; }
};

class MahjongSet {
  public:
    explicit MahjongSet();
    void print();
    void shuffle();
    MahjongTile *take_tile();
    bool get_num_tiles() {return _mahjong_set.size();}

  protected:
    std::vector<MahjongTile *> _mahjong_set;

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
