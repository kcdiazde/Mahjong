
#ifndef PLAYER_H
#define PLAYER_H

#include "mahjong_common.h"
#include "tiles.h"

class Player {

protected:
    std::vector<MahjongTile*> _hand;
    std::vector<MahjongTile*> _flowers;
    uint16_t _points;

    void print_hand();

public:
    explicit Player() {};

    void deal_tiles(std::vector<MahjongTile*> tiles);
    uint8_t get_num_tiles();

    bool play();
};


#endif