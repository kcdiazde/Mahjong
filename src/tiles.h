#ifndef TILES_H
#define TILES_H

#include <algorithm>
#include <cassert>
#include <chrono>
#include <list>
#include <memory>
#include <random>
#include <stdio.h>
#include <string>
#include <vector>

#include "logger.h"
#include "mahjong_common.h"

class MahjongTile {
 protected:
  static TileId unique_id_;
  TileGroup tile_group_;
  std::string name_;
  TileId id_;

  static TileId GetUniqueId();

 public:
  // TODO: Clean constructors
  explicit MahjongTile() {}
  explicit MahjongTile(TileGroup tile_group, std::string name)
      : tile_group_(tile_group), name_(name) {
    id_ = GetUniqueId();
  }
  // Virtual destructor to make class polymorphic
  virtual ~MahjongTile() {
    Logger::Instance().Verbose(
        "GOOOOOOOOOOOOOODBYE WOOOOOOOOOOOOOOOOOOOOOOOOOOOOOORLD!!!!!!!!!!!!!!!");
    Logger::Instance().Verbose("Att: %s\n", GetFullName().c_str());
    Logger::Instance().Verbose("ID: %d\n", GetId());
  };

  void Print() {
    printf("%s %s [ID: %d]\n", name_.c_str(), tile_group_.name.c_str(), id_);
  }

  const TileGroup GetGroup() const { return tile_group_; }
  bool IsFlower() { return tile_group_.name == kFlower.name; }
  const std::string GetName() const { return name_; }
  const std::string GetFullName() const {
    return name_ + " " + tile_group_.name.c_str();
  }
  const TileId GetId() const { return id_; }
  static bool TilesCustomComparator(const std::shared_ptr<MahjongTile> a,
                                    const std::shared_ptr<MahjongTile> b);
};

class MahjongTileNumerical : public MahjongTile {
 protected:
  uint8_t value_;

 public:
  // TODO: Clean constructors
  explicit MahjongTileNumerical() {}
  explicit MahjongTileNumerical(TileGroup tile_group, int value) {
    tile_group_ = tile_group;
    value_ = value;
    name_ = std::to_string(value);
    id_ = GetUniqueId();
  }

  uint8_t GetValue() const { return value_; }
};

using MahjongTilePtr = std::shared_ptr<MahjongTile>;

class MahjongSet {
 public:
  explicit MahjongSet();
  void Print();
  void Shuffle();
  MahjongTilePtr TakeTile();
  int GetNumTiles() { return mahjong_set_.size(); }
  std::vector<MahjongTilePtr> GetTiles() { return mahjong_set_; }

 protected:
  std::vector<MahjongTilePtr> mahjong_set_;

  void CreateTiles(TileGroup group, std::list<std::string> names);
  void CreateNumberedTiles(TileGroup group, int max_num_tile);
  void CreateDragonTiles();
  void CreateWindTiles();
  void CreateFlowerTiles();
  void CreateSymbolTiles();
  void CreateBambooTiles();
  void CreateDotTiles();
};

#endif
