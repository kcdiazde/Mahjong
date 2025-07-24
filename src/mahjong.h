#ifndef MAHJONG_H
#define MAHJONG_H

#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

#include "logger.h"
#include "mahjong_common.h"
#include "player.h"
#include "tiles.h"

using PlayerIterator = std::vector<Player *>::iterator;
using PlayersVector = std::vector<Player *>;

class Mahjong {
  private:
    MahjongSet set_;
    std::vector<Player *> players_;
    uint8_t current_player_id_;
    bool first_player_played_;
    std::vector<MahjongTilePtr> discards_;
    PlayerIterator current_player_it_;
    Logger *logger_;
    bool player_was_updated_ = false;

    static constexpr auto kClassName = "Mahjong";

  public:
    explicit Mahjong() : first_player_played_(false) {
        logger_ = &Logger::Instance();
    };
    void CreateMatch(std::list<std::string> list_of_player_names);
    void CreateSet();
    void CreatePlayers(std::list<std::string> list_of_player_names);
    void Rematch();
    void DealTiles();
    bool CheckIfPlayerCanSteal(MahjongTile &tile);
    void DealTileToPlayer(Player *current_player);
    void DealTileToPlayer(Player *player, MahjongTilePtr tile);
    void PrintPlayersHands();
    void Pass3TilesToNextPlayer();
    void CheckPlayersIntegrity();
    bool Play();
    void UpdateCurrentPlayer();
    PlayersVector GetPlayers() { return players_; }
    std::vector<MahjongTilePtr> GetDiscards() { return discards_; }
};

#endif
