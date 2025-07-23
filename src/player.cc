#include "player.h"

void Player::DealTile(MahjongTilePtr tile) {
    logger_->Debug("%s::%s", kClassName, __func__);

    if (tile->IsFlower()) {
        flowers_.push_back(tile);
    } else {
        hand_.push_back(tile);
        SortHand();
    }
}

void Player::PrintHand() {
    logger_->Debug("%s::%s", kClassName, __func__);

    printf("******%s's Hand******\n", name_.c_str());
    for (auto tile : hand_) {
        tile->Print();
    }
    printf("******Flowers******\n");
    for (auto flower : flowers_) {
        flower->Print();
    }
    printf("\n");
}

void Player::PrintSets() {
    logger_->Debug("%s::%s", kClassName, __func__);

    printf("******%s's Sets******\n", name_.c_str());
    for (auto tile : sets_) {
        tile->Print();
    }
    printf("\n");
}

void Player::SortHand() {
    logger_->Debug("%s::%s", kClassName, __func__);

    std::sort(hand_.begin(), hand_.end(), MahjongTile::TilesCustomComparator);
}

bool Player::FindTileWithIdAndGroup(TileId tile_id, std::string tile_group) {
    logger_->Debug("%s::%s", kClassName, __func__);

    for (auto tile : hand_) {
        bool tile_has_id = (tile->GetId() == tile_id);
        bool tile_has_group = (tile->GetGroup().name == tile_group);
        if (tile_has_id && tile_has_group) {
            return true;
        }
    }

    return false;
}

bool Player::IsTilePung(MahjongTile &tile) {
    logger_->Debug("%s::%s", kClassName, __func__);

    auto tile_id = tile.GetId();
    uint8_t tiles_with_wanted_id = 0;

    for (auto tile_ptr : hand_) {
        if (tile_ptr->GetId() == tile_id) {
            ++tiles_with_wanted_id;
        }
    }

    return tiles_with_wanted_id >= kTilesInPung;
}

bool Player::IsTilePartialPung(MahjongTile &tile) {
    logger_->Debug("%s::%s", kClassName, __func__);

    auto tile_id = tile.GetId();
    uint8_t tiles_with_wanted_id = 0;

    for (auto tile_ptr : hand_) {
        if (tile_ptr->GetId() == tile_id) {
            ++tiles_with_wanted_id;
        }
    }

    return tiles_with_wanted_id == (kTilesInPung - 1);
}

bool Player::IsTileChow(MahjongTile &tile) {
    logger_->Debug("%s::%s", kClassName, __func__);

    if (!(tile.GetGroup().has_a_number)) {
        return false;
    }

    TileId tile_id = tile.GetId();

    std::string tile_group = tile.GetGroup().name;
    TileId next_id = tile_id + 1;
    TileId next_next_id = tile_id + 2;
    TileId prev_id = tile_id - 1;
    TileId prev_prev_id = tile_id - 2;

    bool next_tile_found = FindTileWithIdAndGroup(next_id, tile_group);
    bool next_next_tile_found =
        FindTileWithIdAndGroup(next_next_id, tile_group);
    bool prev_tile_found = FindTileWithIdAndGroup(prev_id, tile_group);
    bool prev_prev_tile_found =
        FindTileWithIdAndGroup(prev_prev_id, tile_group);

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

bool Player::IsTilePartialChow(MahjongTile &tile) {
    logger_->Debug("%s::%s", kClassName, __func__);

    if (!(tile.GetGroup().has_a_number)) {
        return false;
    }

    TileId tile_id = tile.GetId();

    std::string tile_group = tile.GetGroup().name;
    TileId next_id = tile_id + 1;
    TileId next_next_id = tile_id + 2;
    TileId prev_id = tile_id - 1;
    TileId prev_prev_id = tile_id - 2;

    bool next_tile_found = FindTileWithIdAndGroup(next_id, tile_group);
    bool next_next_tile_found =
        FindTileWithIdAndGroup(next_next_id, tile_group);
    bool prev_tile_found = FindTileWithIdAndGroup(prev_id, tile_group);
    bool prev_prev_tile_found =
        FindTileWithIdAndGroup(prev_prev_id, tile_group);

    return next_tile_found || next_next_tile_found || prev_tile_found ||
           prev_prev_tile_found;
}

void Player::Pass3Tiles(Player *receiver) {
    logger_->Debug("%s::%s", kClassName, __func__);

    while (tiles_to_pass_.size()) {
        MoveTileBetweenHands(tiles_to_pass_, receiver->tiles_received_,
                             tiles_to_pass_[0]->GetId());
    }
}

void Player::MoveReceivedTilesToHand() {
    logger_->Debug("%s::%s", kClassName, __func__);

    while (tiles_received_.size()) {
        MoveTileBetweenHands(tiles_received_, hand_,
                             tiles_received_[0]->GetId());
    }

    SortHand();
}

void Player::Play() {
    logger_->Debug("%s::%s", kClassName, __func__);

    // Look for pungs
    PlayPungs();
    PlayChows();
}

void Player::PlayPungs() {
    logger_->Debug("%s::%s", kClassName, __func__);

    auto hand_copy = hand_;

    for (const auto &tile_ptr : hand_copy) {
        bool is_pung = IsTilePung(*tile_ptr);
        if (is_pung) {
            logger_->Info("Pung of %s!!!", tile_ptr->GetName().c_str());
            TileId pung_id = tile_ptr->GetId();
            bool tile_was_removed = false;
            do {
                tile_was_removed = MoveTileBetweenHands(hand_, sets_, pung_id);
            } while (tile_was_removed);
        }
    }
}

void Player::PlayChows() {
    logger_->Debug("%s::%s", kClassName, __func__);

    bool finished_searching = false;

    while (!finished_searching) {
        const auto hand_copy = hand_;

        for (const auto &tile_ptr : hand_copy) {
            if (IsTileChow(*tile_ptr)) {
                logger_->Info("Chow of %s!!!", tile_ptr->GetName().c_str());
                TileId chow_id = tile_ptr->GetId();
                TileId chow_next_id = chow_id + 1;
                TileId chow_next_next_id = chow_id + 2;
                MoveTileBetweenHands(hand_, sets_, chow_id);
                MoveTileBetweenHands(hand_, sets_, chow_next_id);
                MoveTileBetweenHands(hand_, sets_, chow_next_next_id);
                finished_searching = false;
                break;
            }
            finished_searching = true;
        }
    }
}

MahjongTilePtr Player::GetTileToDiscard() {
    logger_->Debug("%s::%s", kClassName, __func__);

    auto hand_copy = hand_;

    // Look for useless tiles
    for (auto tile : hand_copy) {
        bool is_pung = IsTilePung(*tile);
        bool is_partial_pung = IsTilePartialPung(*tile);
        bool is_chow = IsTileChow(*tile);
        bool is_partial_chow = IsTilePartialChow(*tile);

        bool tile_is_useful =
            (is_pung || is_partial_pung || is_chow || is_partial_chow);
        if (!tile_is_useful) {
            RemoveTile(tile->GetId());
            last_discard_ = tile;
            return tile;
        }
    }

    // Look for partial sets
    for (auto tile : hand_) {
        bool is_pung = IsTilePung(*tile);
        bool is_partial_pung = IsTilePartialPung(*tile);
        bool is_chow = IsTileChow(*tile);
        bool is_partial_chow = IsTilePartialChow(*tile);

        bool tile_is_useful =
            (!is_pung && !is_chow && (is_partial_pung || is_partial_chow));
        if (!tile_is_useful) {
            RemoveTile(tile->GetId());
            last_discard_ = tile;
            return tile;
        }
    }

    // Else remove first tile
    MahjongTilePtr first_tile = hand_[0];
    RemoveTile(first_tile->GetId());

    last_discard_ = first_tile;

    return first_tile;
}

bool Player::WantsDiscardTile(MahjongTilePtr tile) {
    logger_->Debug("%s::%s", kClassName, __func__);

    bool is_partial_pung = IsTilePartialPung(*tile);
    bool is_partial_chow = IsTilePartialChow(*tile);
    return is_partial_chow || is_partial_pung;
}

void Bot::PreprocessHand() {
    logger_->Debug("%s::%s", kClassName, __func__);

    MoveTilesToPass();
}

void Bot::MoveTilesToPass() {
    logger_->Debug("%s::%s", kClassName, __func__);

    // First search for useless tiles
    auto hand_copy = hand_;

    for (const auto &tile : hand_copy) {
        bool is_pung = IsTilePung(*tile);
        bool is_partial_pung = IsTilePartialPung(*tile);
        bool is_chow = IsTileChow(*tile);
        bool is_partial_chow = IsTilePung(*tile);

        bool tile_is_useful =
            (is_pung || is_partial_pung || is_chow || is_partial_chow);
        if (!tile_is_useful) {
            MoveTileBetweenHands(hand_, tiles_to_pass_, tile->GetId());
        }

        if (tiles_to_pass_.size() == kNumTilesToPass) {
            break;
        }
    }

    // If no useless tiles found then pass other tiles
    while (tiles_to_pass_.size() < kNumTilesToPass) {
        MoveTileBetweenHands(hand_, tiles_to_pass_, (hand_[0])->GetId());
    }
}

bool Player::MoveTileBetweenHands(MahjongHand &src_hand, MahjongHand &dst_hand,
                                  TileId tile_id) {
    logger_->Debug("%s::%s", kClassName, __func__);

    auto src_hand_copy = src_hand;

    bool tile_was_removed = false;
    for (const auto &tile_ptr : src_hand_copy) {
        if (tile_ptr->GetId() == tile_id) {
            dst_hand.push_back(tile_ptr);
            src_hand.erase(
                std::find(src_hand.begin(), src_hand.end(), tile_ptr));
            tile_was_removed = true;
            break;
        }
    }

    return tile_was_removed;
}

void Player::RemoveTile(TileId tile_id) {
    logger_->Debug("%s::%s", kClassName, __func__);

    for (auto tile_it = hand_.begin(); tile_it != hand_.end(); ++tile_it) {
        MahjongTilePtr tile = *tile_it;
        if (tile->GetId() == tile_id) {
            hand_.erase(tile_it);
            return;
        }
    }
}

bool Player::HasWon() {
    logger_->Debug("%s::%s", kClassName, __func__);

    if (hand_.size() <= 1) {
        return true;
    } else {
        return false;
    }
}

// TODO: Common function to print
void Bot::PrintPungs() {
    logger_->Debug("%s::%s", kClassName, __func__);

    printf("******%s's Pungs******\n", name_.c_str());
    for (auto tile : hand_) {
        bool is_pung = IsTilePung(*tile);
        if (is_pung) {
            tile->Print();
        }
    }
}

void Bot::PrintChows() {
    logger_->Debug("%s::%s", kClassName, __func__);

    printf("******%s's Chows******\n", name_.c_str());
    for (auto tile : hand_) {
        bool is_chow = IsTileChow(*tile);
        if (is_chow) {
            tile->Print();
        }
    }
}

void Player::PrintTilesToPass() {
    logger_->Debug("%s::%s", kClassName, __func__);

    printf("******%s's Tiles to pass******\n", name_.c_str());
    for (auto tile : tiles_to_pass_) {
        tile->Print();
    }
    printf("\n");
}

void Player::PrintTilesReceived() {
    logger_->Debug("%s::%s", kClassName, __func__);

    printf("******%s's Tiles received******\n", name_.c_str());
    for (auto tile : tiles_received_) {
        tile->Print();
    }
    printf("\n");
}
