#ifndef MAHJONG_H
#define MAHJONG_H

#include "mahjong_common.h"
#include "player.h"
#include "tiles.h"
#include <iostream>
#include <list>
#include <string>
#include <vector>

typedef std::vector<Player *>::iterator PlayerIterator;
typedef std::vector<Player *> PlayersVector;

class Mahjong {

  private:
    MahjongSet *_set;
    MahjongSet *_discarded_tiles;
    PlayersVector _players;
    uint8_t _current_player_id;
    bool _first_player_played;
    MahjongTile * _last_discard;
    PlayerIterator _current_player_it;

  public:
    explicit Mahjong() : _first_player_played(false), _last_discard(nullptr) {};
    void create_match(std::list<std::string> list_of_player_names);
    void create_set();
    void create_players(std::list<std::string> list_of_player_names);
    void rematch();
    void deal_tiles();
    void deal_tile_to_player(Player * current_player);
    void deal_tile_to_player(Player *player, MahjongTile * tile);
    void print_players_hands();
    void pass_3_tiles_to_next_player();
    bool play();
    void update_current_player();
    PlayersVector * get_players() { return &_players; }
};

#endif
