#ifndef PLAYER_CC
#define PLAYER_CC

#include "player.h"

void Player::deal_tile(MahjongTile *tile) {
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

void Player::sort_hand() {
    std::sort(_hand.begin(), _hand.end(), MahjongTile::tilesCustomComparator);
}

MahjongHand User::get_tiles_to_pass() {
    return _hand;
}

void Bot::preprocess_hand() {
     // Search for sets of pung
     /*
     for (auto tile = _hand.begin(); tile != _hand.end(); tile++) {
        for (auto tile_to_cmp = tile + 1; tile_to_cmp != _hand.end(); tile_to_cmp++) {
            if ()
        }
     }
     */
     for (auto tile : _hand) {
        
     }
}

MahjongHand Bot::get_tiles_to_pass() {
    return _hand;
}

#endif