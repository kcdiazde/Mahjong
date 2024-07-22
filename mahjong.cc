
#ifndef MAHJONG_CC
#define MAHJONG_CC

#include "mahjong.h"

void Mahjong::create_match(std::list<std::string> list_of_player_names) {

    if (!list_of_player_names.size()) {
        throw std::runtime_error("Game should have at least 1 player");
    }

    create_players(list_of_player_names);
    create_set();

}


void Mahjong::create_players(std::list<std::string> list_of_player_names) {
        // Empty if already populated
    while (_players.size()) {
            Player * temp_player = _players.front();
            _players.erase(_players.begin());
            delete temp_player;
    }

    // Create new players
    Player * new_player = nullptr;
    for (auto player_str : list_of_player_names) {
        new_player = new Player(player_str);
    }
    _players.push_back(new_player);
}

void Mahjong::create_set() {
    _set = new MahjongSet();
    _set->shuffle();
}

int main() {
    printf("Welcome to mahjong\n\n");

    MahjongSet mahjong_set = MahjongSet();
    // mahjong_set.print();
    mahjong_set.shuffle();
    // mahjong_set.print();

    /*
    Player player_1 = Player("Yo");
    Player player_2 = Player("P2");
    Player player_3 = Player("P3");
    Player player_4 = Player("P4");
    for (int i = 0; i < 13; i++) {
        player_1.deal_tile(mahjong_set.take_tile());
    }
    player_1.print_hand();
    */

    // mahjong_set.print();

    return 0;
}

#endif