#ifndef MAHJONG_H
#define MAHJONG_H

#include "tiles.h"
#include "player.h"
#include "mahjong_common.h"
#include <list>
#include <string>
#include <vector>
#include <iostream>

class Mahjong {

private:
    MahjongSet * _set;
    std::vector<Player*> _players;
    uint8_t _current_player_id;

public:
    explicit Mahjong()  {};
    void create_match(std::list<std::string> list_of_player_names);
    void create_set();
    void create_players(std::list<std::string> list_of_player_names);
    void rematch();
    void deal_tiles();
    void deal_tile_to_player(Player* player);
    void print_players_hands();


};


#endif