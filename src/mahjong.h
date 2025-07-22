#ifndef MAHJONG_H
#define MAHJONG_H

#include "mahjong_common.h"
#include "player.h"
#include "tiles.h"
#include "logger.h"
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <sstream>
#include <string>

typedef std::vector<Player *>::iterator PlayerIterator;
typedef std::vector<Player *> PlayersVector;

class Mahjong {

  private:
    MahjongSet _set;
    std::vector<Player *> _players;
    uint8_t _current_player_id;
    bool _first_player_played;
    std::vector<MahjongTilePtr> _discards;
    PlayerIterator _current_player_it;
    Logger * _logger;
    bool _player_was_updated = false;

    static constexpr auto CLASSNAME = "Mahjong";

  public:
    explicit Mahjong() : _first_player_played(false) {
        _logger = &Logger::instance();
    };
    void create_match(std::list<std::string> list_of_player_names);
    void create_set();
    void create_players(std::list<std::string> list_of_player_names);
    void rematch();
    void deal_tiles();
    bool check_if_player_can_steal(MahjongTile& tile);
    void deal_tile_to_player(Player * current_player);
    void deal_tile_to_player(Player *player, MahjongTilePtr tile);
    void print_players_hands();
    void pass_3_tiles_to_next_player();
    void check_players_integrity();
    bool play();
    void update_current_player();
    PlayersVector * get_players() { return &_players; }
};

#endif
