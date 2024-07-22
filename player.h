
#ifndef PLAYER_H
#define PLAYER_H

#include "mahjong_common.h"
#include "tiles.h"
#include <string>

class Player {

protected:
    std::string _name;
    std::vector<MahjongTile*> _hand;
    std::vector<MahjongTile*> _flowers;
    uint16_t _points;

public:
    explicit Player() {};
    explicit Player(std::string name) :
                _name(name),
                _points(0) 
                {};

    void print_hand();
    void deal_tile(MahjongTile*);
    uint8_t get_num_tiles() {return _hand.size();}

    bool play();
};


#endif