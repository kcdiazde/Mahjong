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
     // TODO: need to expand for so I can reset the count if I moved tiles
    for (auto tile : _hand) {
        uint8_t wanted_id = tile->get_id();
        uint8_t wanted_id_num = 0;

        for (auto sub_tile : _hand) {
            if (sub_tile->get_id() == wanted_id) {
                ++wanted_id_num;
            }
        }

        if (wanted_id_num >= 3) {
            move_tiles_to_concealed(tile->get_id());
            // TODO: RESET COUNTER
        }
    }
}

void Bot::move_tiles_to_concealed(TileId id) {
    for (auto tile_it = _hand.begin(); tile_it != _hand.end();) {
        MahjongTile * tile = *tile_it;
        if (tile->get_id() == id) {
            _concealed_sets.push_back(tile);
            _hand.erase(tile_it);
            tile_it = _hand.begin();
        } else {
            tile_it++;
        }
    }
    print_concealed();
}

MahjongHand Bot::get_tiles_to_pass() {
    return _hand;
}

void Bot::print_concealed() {
    printf("******Concealed******\n");
    for (auto tile : _concealed_sets) {
        tile->print();
    }
}

#endif