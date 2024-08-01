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
void User::pass_3_tiles(Player *receiver) {}
void User::receive_tile(MahjongTile *tile) {}

void Bot::preprocess_hand() {
    // TODO: Consider removing this functions
    /*find_and_conceal_pungs();
    find_and_conceal_chows();
    find_and_move_pairs();
    find_and_move_potential_chows();

    print_concealed();
    print_wanted();
    */

    print_hand();

    for (auto tile : _hand) {
        bool is_pung = tile_is_pung(*tile);
        bool is_partial_pung = tile_is_partial_pung(*tile);
        bool is_chow = tile_is_chow(*tile);
        bool is_partial_chow = tile_is_partial_chow(*tile);
        if (is_pung) {
            printf("Tile %s is pung\n", tile->get_full_name().c_str());
        } else if (is_partial_pung) {
            printf("Tile %s is partial pung\n", tile->get_full_name().c_str());
        }
        if (is_chow) {
            printf("Tile %s is chow\n", tile->get_full_name().c_str());
        } else if (is_partial_chow) {
            printf("Tile %s is partial chow\n", tile->get_full_name().c_str());
        }
    }
}

void Bot::find_and_conceal_pungs() {
    for (auto tile_it = _hand.begin(); tile_it != _hand.end();) {
        MahjongTile *tile = *tile_it;
        uint8_t wanted_id = tile->get_id();
        uint8_t tiles_with_wanted_id = 0;

        for (auto sub_tile : _hand) {
            if (sub_tile->get_id() == wanted_id) {
                ++tiles_with_wanted_id;
            }
        }

        const uint8_t PUNG_OF_TILES = 3;
        if (tiles_with_wanted_id >= PUNG_OF_TILES) {
            move_pung_to_concealed(tile->get_id());
            tile_it = _hand.begin();
        } else {
            tile_it++;
        }
    }
}

void Bot::find_and_conceal_chows() {
    for (auto tile_it = _hand.begin(); tile_it != _hand.end();) {
        MahjongTile *tile = *tile_it;
        if (!(tile->get_group().has_a_number)) {
            ++tile_it;
            continue;
        }

        TileId next_id = tile->get_id() + 1;
        TileId next_next_id = tile->get_id() + 2;
        bool chow_found = false;

        for (auto sub_tile : _hand) {
            bool tiles_are_same_group =
                (tile->get_group().name == sub_tile->get_group().name);
            if ((sub_tile->get_id() == next_id) && tiles_are_same_group) {
                for (auto sub_sub_tile : _hand) {
                    tiles_are_same_group = (tile->get_group().name ==
                                            sub_sub_tile->get_group().name);
                    if (sub_sub_tile->get_id() == next_next_id) {
                        chow_found = true;
                        break;
                    }
                }
                if (chow_found) {
                    break;
                }
            }
        }

        if (chow_found) {
            move_chow_to_concealed(tile->get_id());
            tile_it = _hand.begin();
            chow_found = false;
        } else {
            tile_it++;
        }
    }
}

void Bot::find_and_move_pairs() {
    for (auto tile_it = _hand.begin(); tile_it != _hand.end();) {
        MahjongTile *tile = *tile_it;
        uint8_t wanted_id = tile->get_id();
        uint8_t tiles_with_wanted_id = 0;

        for (auto sub_tile : _hand) {
            if (sub_tile->get_id() == wanted_id) {
                ++tiles_with_wanted_id;
            }
        }

        const uint8_t PAIR_OF_TILES = 2;
        if (tiles_with_wanted_id == PAIR_OF_TILES) {
            move_pair_to_useful(tile->get_id());
            tile_it = _hand.begin();
        } else {
            tile_it++;
        }
    }
}

void Bot::find_and_move_potential_chows() {
    for (auto tile_it = _hand.begin(); tile_it != _hand.end();) {
        MahjongTile *tile = *tile_it;

        if (!(tile->get_group().has_a_number)) {
            ++tile_it;
            continue;
        }

        TileId id = tile->get_id();
        TileId next_id = id + 1;
        TileId next_next_id = id + 2;

        bool chow_found = false;

        for (auto sub_tile : _hand) {
            std::string tile_group_name = tile->get_group().name;
            std::string subtile_group_name = sub_tile->get_group().name;
            if (tile_group_name != subtile_group_name) {
                continue;
            }

            bool next_id_is_chow = (sub_tile->get_id() == next_id);
            bool next_next_id_is_chow = (sub_tile->get_id() == next_next_id);
            if (next_id_is_chow || next_next_id_is_chow) {
                TileId chow_id = (next_id_is_chow) ? next_id : next_next_id;
                move_chow_to_useful(id, chow_id);
                chow_found = true;
                break;
            }
        }

        if (chow_found) {
            tile_it = _hand.begin();
            chow_found = false;
        } else {
            tile_it++;
        }
    }
}

bool Bot::move_tile_between_hands(MahjongHand *src_hand, MahjongHand *dst_hand,
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

void Bot::move_pung_to_concealed(TileId id) {
    bool tile_was_removed = false;
    do {
        tile_was_removed =
            move_tile_between_hands(&_hand, &_concealed_sets, id);
    } while (tile_was_removed);
}

void Bot::move_chow_to_concealed(TileId id) {
    const uint8_t NUM_TILES_IN_CHOW = 3;
    for (uint8_t chow_num = 0; chow_num < NUM_TILES_IN_CHOW; ++chow_num) {
        move_tile_between_hands(&_hand, &_concealed_sets, id++);
    }
}

void Bot::move_pair_to_useful(TileId id) {
    bool tile_was_removed = false;
    do {
        tile_was_removed = move_tile_between_hands(&_hand, &_useful_tiles, id);
    } while (tile_was_removed);
}

void Bot::move_chow_to_useful(TileId chow_tile1, TileId chow_tile2) {
    move_tile_between_hands(&_hand, &_useful_tiles, chow_tile1);
    move_tile_between_hands(&_hand, &_useful_tiles, chow_tile2);
}

/*
MahjongHand Bot::get_tiles_to_pass() {

    std::list<MahjongHand *> hands_to_extract_from {&_hand, &_useful_tiles};
    uint8_t tiles_moved = 0;
    const uint8_t TILES_TO_MOVE = 3;

    for (auto hand_to_extract : hands_to_extract_from) {
        for (auto tile_it = hand_to_extract->begin(); tile_it !=
hand_to_extract->end(); ++tile_it) { MahjongTile * tile = * tile_it;
            move_tile_between_hands(hand_to_extract, &_tiles_to_pass,
tile->get_id()); tiles_moved = _tiles_to_pass.size(); if (tiles_moved ==
TILES_TO_MOVE) { return _tiles_to_pass;
            }
        }
    }


    const uint8_t TILES_TO_MOVE = 3;
    uint8_t num_tiles_to_discard = _hand.size() + _useful_tiles.size();

    // If not enough tiles in hand and usefull, take from concealed
    if (num_tiles_to_discard < TILES_TO_MOVE) {
        for (auto tile : _concealed_sets) {
            move_tile_between_hands(&_concealed_sets, &_tiles_to_pass,
tile->get_id());
            // If tiles taken are a Kung, then remove the needed tile
            if (_tiles_to_pass[0]->get_id() == _concealed_sets[0]->get_id()) {
                move_tile_between_hands(&_concealed_sets, &_unwanted_tiles,
_concealed_sets[0]->get_id());
            }
            if (_tiles_to_pass.size() == TILES_TO_MOVE) {
                return _tiles_to_pass;
            }
        }
    }

    for (auto tile : _hand) {
        move_tile_between_hands(&_hand, &_tiles_to_pass, tile->get_id());
        if (_tiles_to_pass.size() == TILES_TO_MOVE) {
            return _tiles_to_pass;
        }
    }

    for (auto tile : _useful_tiles) {
        move_tile_between_hands(&_useful_tiles, &_tiles_to_pass,
tile->get_id()); if (_tiles_to_pass.size() == TILES_TO_MOVE) { return
_tiles_to_pass;
        }
    }

    return _tiles_to_pass;
}
*/

void Bot::pass_3_tiles(Player *receiver) {
    const uint8_t TILES_TO_MOVE = 3;
    uint8_t num_passed_tiles = 0;

    uint8_t num_tiles_to_discard = _hand.size() + _useful_tiles.size();

    // If not enough tiles in hand and usefull, take from concealed
    if (num_tiles_to_discard < TILES_TO_MOVE) {
        for (auto tile : _concealed_sets) {
            move_tile_between_hands(&_concealed_sets, &_tiles_to_pass,
                                    tile->get_id());
            // If tiles taken are a Kung, then remove the needed tile
            if (_tiles_to_pass[0]->get_id() == _concealed_sets[0]->get_id()) {
                move_tile_between_hands(&_concealed_sets, &_unwanted_tiles,
                                        _concealed_sets[0]->get_id());
            }
            if (_tiles_to_pass.size() == TILES_TO_MOVE) {
                break;
            }
        }
    }
}

void Bot::receive_tile(MahjongTile *tile) {

    // Search if tile forms a kung in concealed or sets (function)
    // Search if tile forms a
}

void Bot::print_concealed() {
    printf("******%s's Concealed******\n", _name.c_str());
    for (auto tile : _concealed_sets) {
        tile->print();
    }
    printf("\n");
}

void Bot::print_wanted() {
    printf("******%s's Wanted******\n", _name.c_str());
    for (auto tile : _useful_tiles) {
        tile->print();
    }
    printf("\n");
}

#endif