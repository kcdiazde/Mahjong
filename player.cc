#ifndef PLAYER_CC
#define PLAYER_CC

void Player::deal_tiles(std::vector<MahjongTile*> tiles) {
    if (tiles) {
        for (auto tile : tiles) {
            if (tile->get_group() == FLOWER) {
                _flowers.push_back(tile);
            } else {
                _hand.push_back(tile);
            }
        }
    }
}

#endif