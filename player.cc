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

MahjongHand User::get_tiles_to_pass() { return _hand; }

void Bot::preprocess_hand() {
    find_and_conceal_pungs();
    find_and_conceal_chows();
    find_and_move_pairs();
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

        HandIterator next_it;
        HandIterator next_next_it;
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
            move_chow_to_concealed(tile_it);
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
            move_tiles_to_useful(tile->get_id());
            tile_it = _hand.begin();
        } else {
            tile_it++;
        }
    }
}

void Bot::move_pung_to_concealed(TileId id) {
    for (auto tile_it = _hand.begin(); tile_it != _hand.end();) {
        MahjongTile *tile = *tile_it;
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

void Bot::move_tiles_to_useful(TileId id) {
    for (auto tile_it = _hand.begin(); tile_it != _hand.end();) {
        MahjongTile *tile = *tile_it;
        if (tile->get_id() == id) {
            _useful_tiles.push_back(tile);
            _hand.erase(tile_it);
            tile_it = _hand.begin();
        } else {
            tile_it++;
        }
    }
    print_wanted();
}

void Bot::move_chow_to_concealed(HandIterator tile_it) {
    MahjongTile *tile = *tile_it;
    TileId tile_id = (*tile_it)->get_id();
    TileId next_tile_id = tile_id + 1;
    TileId next_next_tile_id = tile_id + 2;

    _concealed_sets.push_back(tile);
    _hand.erase(tile_it);

    for (auto t_it = _hand.begin(); t_it != _hand.end(); ++t_it) {
        tile = *t_it;
        if (tile->get_id() == next_tile_id) {
            _concealed_sets.push_back(tile);
            _hand.erase(t_it);
        }
    }

    for (auto t_it = _hand.begin(); t_it != _hand.end(); ++t_it) {
        tile = *t_it;
        if (tile->get_id() == next_next_tile_id) {
            _concealed_sets.push_back(tile);
            _hand.erase(t_it);
        }
    }

    print_concealed();
}

MahjongHand Bot::get_tiles_to_pass() { return _hand; }

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