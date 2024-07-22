
#ifndef MAHJONG_CC
#define MAHJONG_CC

#include "tiles.h"
#include "player.h"
#include "mahjong_common.h"

int main() {
    printf("Welcome to mahjong\n\n");

    MahjongSet mahjong_set = MahjongSet();
    // mahjong_set.print();
    mahjong_set.shuffle();
    // mahjong_set.print();

    Player player_1 = Player("Yo");
    for (int i = 0; i < 14; i++) {
        player_1.deal_tile(mahjong_set.take_tile());
    }
    player_1.print_hand();

    // mahjong_set.print();

    return 0;
}

#endif