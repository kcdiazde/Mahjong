#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <list>
#include <string>

#include "logger.h"
#include "mahjong_common.h"
#include "tiles.h"

using MahjongHand = std::vector<MahjongTilePtr>;
using HandIterator = std::vector<MahjongTilePtr>::iterator;

class Player {
  protected:
    std::string name_;
    MahjongHand sets_;
    MahjongHand hand_;
    MahjongHand flowers_;
    MahjongHand tiles_to_pass_;
    uint16_t points_;
    MahjongTilePtr last_discard_;
    Logger *logger_;

    static constexpr auto kClassName = "Player";

  public:
    MahjongHand tiles_received_;
    explicit Player(){};
    explicit Player(std::string name)
        : name_(name), points_(0), last_discard_(nullptr) {
        logger_ = &Logger::Instance();
    };
    virtual ~Player() = default;

    void PrintHand();
    void PrintSets();
    void PrintTilesToPass();
    void PrintTilesReceived();
    void DealTile(MahjongTilePtr);
    uint8_t GetNumTotalTiles() { return hand_.size() + flowers_.size(); }
    uint8_t GetNumTilesInHand() { return hand_.size(); }
    uint8_t GetNumTilesSetAndHand() { return hand_.size() + sets_.size(); }
    uint8_t GetNumFlowers() { return flowers_.size(); }
    std::string GetName() { return name_; }
    MahjongHand *GetHand() { return &hand_; }
    void SortHand();
    bool FindTileWithIdAndGroup(TileId tile_id, std::string group);
    bool MoveTileBetweenHands(MahjongHand &src_hand, MahjongHand &dst_hand,
                              TileId tile_id);
    void RemoveTile(TileId tile_id);
    bool IsTilePung(MahjongTile &tile);
    bool IsTilePartialPung(MahjongTile &tile);
    bool IsTileChow(MahjongTile &tile);
    bool IsTilePartialChow(MahjongTile &tile);
    void Pass3Tiles(Player *receiver);
    void MoveReceivedTilesToHand();
    void Play();
    void PlayPungs();
    void PlayChows();
    MahjongTilePtr GetTileToDiscard();
    bool WantsDiscardTile(MahjongTilePtr tile);
    bool HasWon();
};

class User : public Player {
  public:
    explicit User(){};
    explicit User(std::string name) : Player(name){};
    ~User() = default;
};

// TODO: Keep count of tiles that have come out
class Bot : public Player {
  public:
    explicit Bot(){};
    explicit Bot(std::string name) : Player(name){};
    ~Bot() = default;

    // MahjongHand GetTilesToPass();
    void PreprocessHand();
    void MoveTilesToPass();
    void PrintPungs();
    void PrintChows();
};

#endif
