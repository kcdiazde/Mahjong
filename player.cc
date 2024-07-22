#ifndef PLAYER_CC
#define PLAYER_CC

#include "player.h"

void Player::deal_tile(MahjongTile* tile) {
    if (tile->is_flower()) {
        _flowers.push_back(tile);
    } else {
        _hand.push_back(tile);
    }
}

void Player::print_hand() {
    printf("******%s's Hand******\n", _name.c_str());
    for (auto tile : _hand) {
        tile->print();
    }
    printf("******Flowers******\n");
        for (auto flower : _flowers) {
        flower->print();
    }
    printf("\n");
}

#endif