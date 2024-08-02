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

bool Player::found_tile_with_id_and_group(TileId tile_id,
                                          std::string tile_group) {
    for (auto tile : _hand) {
        bool tile_has_id = (tile->get_id() == tile_id);
        bool tile_has_group = (tile->get_group().name == tile_group);
        if (tile_has_id && tile_has_group) {
            return true;
        }
    }

    return false;
}

bool Player::tile_is_pung(MahjongTile tile) {
    TileId tile_id = tile.get_id();
    uint8_t tiles_with_wanted_id = 0;

    for (auto tile : _hand) {
        if (tile->get_id() == tile_id) {
            ++tiles_with_wanted_id;
        }
    }

    return tiles_with_wanted_id >= TILES_IN_PUNG;
}

bool Player::tile_is_partial_pung(MahjongTile tile) {
    TileId tile_id = tile.get_id();
    uint8_t tiles_with_wanted_id = 0;

    for (auto tile : _hand) {
        if (tile->get_id() == tile_id) {
            ++tiles_with_wanted_id;
        }
    }

    return tiles_with_wanted_id == (TILES_IN_PUNG - 1);
}

bool Player::tile_is_chow(MahjongTile tile) {
    if (!(tile.get_group().has_a_number)) {
        return false;
    }

    TileId tile_id = tile.get_id();

    std::string tile_group = tile.get_group().name;
    TileId next_id = tile_id + 1;
    TileId next_next_id = tile_id + 2;
    TileId prev_id = tile_id - 1;
    TileId prev_prev_id = tile_id - 2;

    bool next_tile_found = found_tile_with_id_and_group(next_id, tile_group);
    bool next_next_tile_found =
        found_tile_with_id_and_group(next_next_id, tile_group);
    bool prev_tile_found = found_tile_with_id_and_group(prev_id, tile_group);
    bool prev_prev_tile_found =
        found_tile_with_id_and_group(prev_prev_id, tile_group);

    // Tile is at beggining of chow
    if (next_tile_found && next_next_tile_found)
        return true;

    // Tile is at middle of chow
    if (prev_tile_found && next_tile_found)
        return true;

    // Tile is at end of chow
    if (prev_prev_tile_found && prev_tile_found)
        return true;

    return false;
}

bool Player::tile_is_partial_chow(MahjongTile tile) {
    if (!(tile.get_group().has_a_number)) {
        return false;
    }

    TileId tile_id = tile.get_id();

    std::string tile_group = tile.get_group().name;
    TileId next_id = tile_id + 1;
    TileId next_next_id = tile_id + 2;
    TileId prev_id = tile_id - 1;
    TileId prev_prev_id = tile_id - 2;

    bool next_tile_found = found_tile_with_id_and_group(next_id, tile_group);
    bool next_next_tile_found =
        found_tile_with_id_and_group(next_next_id, tile_group);
    bool prev_tile_found = found_tile_with_id_and_group(prev_id, tile_group);
    bool prev_prev_tile_found =
        found_tile_with_id_and_group(prev_prev_id, tile_group);

    return next_tile_found || next_next_tile_found || prev_tile_found ||
           prev_prev_tile_found;
}

void Player::pass_3_tiles(Player *receiver) {
    while(_tiles_to_pass.size()) {
        move_tile_between_hands(&_tiles_to_pass, &(receiver->_tiles_received), _tiles_to_pass[0]->get_id());
    }
}

void Player::move_received_tiles_to_hand() {
    while(_tiles_received.size()) {
        move_tile_between_hands(&_tiles_received, &_hand, _tiles_received[0]->get_id());
    }

    sort_hand();
}

void Bot::preprocess_hand() {
    move_tiles_to_pass();
    /*
    print_hand();
    print_pungs();
    print_chows();
    print_tiles_to_pass();
    */
}

void Bot::move_tiles_to_pass() {
    // First search for useless tiles
    for (auto tile : _hand) {
        bool is_pung = tile_is_pung(*tile);
        bool is_partial_pung = tile_is_pung(*tile);
        bool is_chow = tile_is_pung(*tile);
        bool is_partial_chow = tile_is_pung(*tile);

        bool tile_is_useful =
            (is_pung || is_partial_pung || is_chow || is_partial_chow);
        if (!tile_is_useful) {
            move_tile_between_hands(&_hand, &_tiles_to_pass, tile->get_id());
        }

        if (_tiles_to_pass.size() == NUM_TILES_TO_PASS) {
            break;
        }
    }

    // If no useless tiles found then pass other tiles
    while (_tiles_to_pass.size() < NUM_TILES_TO_PASS) {
        move_tile_between_hands(&_hand, &_tiles_to_pass, (_hand[0])->get_id());
    }
}

bool Player::move_tile_between_hands(MahjongHand *src_hand, MahjongHand *dst_hand,
                                  TileId tile_id) {
    bool tile_was_removed = false;
    for (auto tile_it = src_hand->begin(); tile_it != src_hand->end();
         ++tile_it) {
        MahjongTile *tile = *tile_it;
        if (tile->get_id() == tile_id) {
            dst_hand->push_back(tile);
            src_hand->erase(tile_it);
            tile_was_removed = true;
            break;
        }
    }
    return tile_was_removed;
}

// TODO: Common function to print
void Bot::print_pungs() {
    printf("******%s's Pungs******\n", _name.c_str());
    for (auto tile : _hand) {
        bool is_pung = tile_is_pung(*tile);
        if (is_pung) {
            tile->print();
        }
    }
}

void Bot::print_chows() {
    printf("******%s's Chows******\n", _name.c_str());
    for (auto tile : _hand) {
        bool is_chow = tile_is_chow(*tile);
        if (is_chow) {
            tile->print();
        }
    }
}

void Player::print_tiles_to_pass() {
    printf("******%s's Tiles to pass******\n", _name.c_str());
    for (auto tile : _tiles_to_pass) {
        tile->print();
    }
    printf("\n");
}

void Player::print_tiles_received() {
    printf("******%s's Tiles received******\n", _name.c_str());
    for (auto tile : _tiles_received) {
        tile->print();
    }
    printf("\n");
}

#endif