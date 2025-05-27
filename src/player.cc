#ifndef PLAYER_CC
#define PLAYER_CC

#include "player.h"

void Player::deal_tile(MahjongTilePtr tile) {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    if (tile->is_flower()) {
        _flowers.push_back(tile);
    } else {
        _hand.push_back(tile);
        sort_hand();
    }
}

void Player::print_hand() {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

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

void Player::print_sets() {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    printf("******%s's Sets******\n", _name.c_str());
    for (auto tile : _sets) {
        tile->print();
    }
    printf("\n");
}

void Player::sort_hand() {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    std::sort(_hand.begin(), _hand.end(), MahjongTile::tilesCustomComparator);
}

bool Player::found_tile_with_id_and_group(TileId tile_id,
                                          std::string tile_group) {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    for (auto tile : _hand) {
        bool tile_has_id = (tile->get_id() == tile_id);
        bool tile_has_group = (tile->get_group().name == tile_group);
        if (tile_has_id && tile_has_group) {
            return true;
        }
    }

    return false;
}

bool Player::tile_is_pung(MahjongTile& tile) {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    auto tile_id = tile.get_id();
    uint8_t tiles_with_wanted_id = 0;

    for (auto tilePtr : _hand) {
        if (tilePtr->get_id() == tile_id) {
            ++tiles_with_wanted_id;
        }
    }

    return tiles_with_wanted_id >= TILES_IN_PUNG;
}

bool Player::tile_is_partial_pung(MahjongTile& tile) {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    auto tile_id = tile.get_id();
    uint8_t tiles_with_wanted_id = 0;

    for (auto tilePtr : _hand) {
        if (tilePtr->get_id() == tile_id) {
            ++tiles_with_wanted_id;
        }
    }

    return tiles_with_wanted_id == (TILES_IN_PUNG - 1);
}

bool Player::tile_is_chow(MahjongTile& tile) {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    printf("Checking if tile is chow\n");

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

bool Player::tile_is_partial_chow(MahjongTile& tile) {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

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
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    while (_tiles_to_pass.size()) {
        move_tile_between_hands(_tiles_to_pass, receiver->_tiles_received,
                                _tiles_to_pass[0]->get_id());
    }
}

void Player::move_received_tiles_to_hand() {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    while (_tiles_received.size()) {
        move_tile_between_hands(_tiles_received, _hand,
                                _tiles_received[0]->get_id());
    }

    sort_hand();
}

void Player::play() {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    // Look for pungs
    play_pungs();
    play_chows();
}

void Player::play_pungs() {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    auto hand_copy = _hand;

    for (const auto& tilePtr : hand_copy) {
        bool is_pung = tile_is_pung(*tilePtr);
        if (is_pung) {
            TileId pung_id = tilePtr->get_id();
            bool tile_was_removed = false;
            do {
                tile_was_removed =
                    move_tile_between_hands(_hand, _sets, pung_id);
            } while (tile_was_removed);
            printf("PUNG of %s!\n", tilePtr->get_full_name().c_str());
        }
    }
}

void Player::play_chows() {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    const auto hand_copy = _hand;

    for (const auto& tilePtr : hand_copy) {
        if (tile_is_chow(*tilePtr)) {
            TileId chow_id = tilePtr->get_id();
            TileId chow_next_id = chow_id + 1;
            TileId chow_next_next_id = chow_id + 2;
            move_tile_between_hands(_hand, _sets, chow_id);
            move_tile_between_hands(_hand, _sets, chow_next_id);
            move_tile_between_hands(_hand, _sets, chow_next_next_id);
            printf("CHOW of %s!\n", tilePtr->get_full_name().c_str());
        }
    }
}

MahjongTilePtr Player::get_tile_to_discard() {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    auto hand_copy = _hand;

    // Look for useless tiles
    for (auto tile : hand_copy) {
        bool is_pung = tile_is_pung(*tile);
        bool is_partial_pung = tile_is_partial_pung(*tile);
        bool is_chow = tile_is_chow(*tile);
        bool is_partial_chow = tile_is_partial_chow(*tile);

        bool tile_is_useful =
            (is_pung || is_partial_pung || is_chow || is_partial_chow);
        if (!tile_is_useful) {
            remove_tile(tile->get_id());
            _last_discard = tile;
            return tile;
        }
    }

    // Look for partial sets
    for (auto tile : _hand) {
        bool is_pung = tile_is_pung(*tile);
        bool is_partial_pung = tile_is_partial_pung(*tile);
        bool is_chow = tile_is_chow(*tile);
        bool is_partial_chow = tile_is_partial_chow(*tile);

        bool tile_is_useful =
            (!is_pung && !is_chow && (is_partial_pung || is_partial_chow));
        if (!tile_is_useful) {
            remove_tile(tile->get_id());
            _last_discard = tile;
            return tile;
        }
    }

    // Else remove first tile
    MahjongTilePtr first_tile = _hand[0];
    remove_tile(first_tile->get_id());

    _last_discard = first_tile;

    return first_tile;
}

bool Player::wants_discard_tile(MahjongTilePtr tile) {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    bool is_partial_pung = tile_is_partial_pung(*tile);
    bool is_partial_chow = tile_is_partial_chow(* tile);
    return is_partial_chow || is_partial_pung;
}

void Bot::preprocess_hand() {
    _logger->debug("%s::%s", CLASSNAME, __func__); 
    
    move_tiles_to_pass();
}

void Bot::move_tiles_to_pass() {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    // First search for useless tiles

    auto hand_copy = _hand;

    printf("Will move_tiles_to_pass\n");
    for (const auto& tile : hand_copy) {
        bool is_pung = tile_is_pung(*tile);
        bool is_partial_pung = tile_is_partial_pung(*tile);
        bool is_chow = tile_is_chow(*tile);
        bool is_partial_chow = tile_is_pung(*tile);

        bool tile_is_useful =
            (is_pung || is_partial_pung || is_chow || is_partial_chow);
        if (!tile_is_useful) {
            move_tile_between_hands(_hand, _tiles_to_pass, tile->get_id());
        }

        if (_tiles_to_pass.size() == NUM_TILES_TO_PASS) {
            break;
        }
    }

    // If no useless tiles found then pass other tiles
    while (_tiles_to_pass.size() < NUM_TILES_TO_PASS) {
        move_tile_between_hands(_hand, _tiles_to_pass, (_hand[0])->get_id());
    }
}

bool Player::move_tile_between_hands(MahjongHand& src_hand,
                                     MahjongHand& dst_hand, TileId tile_id) {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    auto src_hand_copy = src_hand;

    bool tile_was_removed = false;
    for (const auto& tilePtr : src_hand_copy) {
        if (tilePtr->get_id() == tile_id) {
            dst_hand.push_back(tilePtr);
            src_hand.erase(std::remove(src_hand.begin(), src_hand.end(), tilePtr), src_hand.end());
            tile_was_removed = true;
            break;
        }
    }
    return tile_was_removed;
}

void Player::remove_tile(TileId tile_id) {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    for (auto tile_it = _hand.begin(); tile_it != _hand.end(); ++tile_it) {
        MahjongTilePtr tile = *tile_it;
        if (tile->get_id() == tile_id) {
            _hand.erase(tile_it);
            return;
        }
    }
}

bool Player::has_won() {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    if (_hand.size() == 1) {
        return true;
    } else {
        return false;
    }
}

// TODO: Common function to print
void Bot::print_pungs() {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    printf("******%s's Pungs******\n", _name.c_str());
    for (auto tile : _hand) {
        bool is_pung = tile_is_pung(*tile);
        if (is_pung) {
            tile->print();
        }
    }
}

void Bot::print_chows() {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    printf("******%s's Chows******\n", _name.c_str());
    for (auto tile : _hand) {
        bool is_chow = tile_is_chow(*tile);
        if (is_chow) {
            tile->print();
        }
    }
}

void Player::print_tiles_to_pass() {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    printf("******%s's Tiles to pass******\n", _name.c_str());
    for (auto tile : _tiles_to_pass) {
        tile->print();
    }
    printf("\n");
}

void Player::print_tiles_received() {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    printf("******%s's Tiles received******\n", _name.c_str());
    for (auto tile : _tiles_received) {
        tile->print();
    }
    printf("\n");
}

#endif
