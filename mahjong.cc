#ifndef MAHJONG_CC
#define MAHJONG_CC

#include "mahjong.h"

// TODO: Receive name of player and number of bots
void Mahjong::create_match(std::list<std::string> list_of_player_names) {
    if (list_of_player_names.size() < MIN_NUM_PLAYERS ||
        list_of_player_names.size() > MAX_NUM_PLAYERS) {
        throw std::runtime_error("Game should have at least 1 player");
    }

    create_players(list_of_player_names);
    create_set();
    deal_tiles();
}

void Mahjong::create_players(std::list<std::string> list_of_player_names) {
    // Empty-out if already populated
    while (_players.size()) {
        Player *temp_player = _players.front();
        _players.erase(_players.begin());
        delete temp_player;
    }

    // Create new players
    Player *new_player = nullptr;
    // TODO: Set as false, only bots for testing
    bool user_created = true;
    for (auto player_str : list_of_player_names) {
        if (user_created) {
            new_player = new Bot(player_str);            
        } else {
            new_player = new User(player_str);
        }
        _players.push_back(new_player);
    }
}

void Mahjong::create_set() {
    _set = new MahjongSet();
    _set->shuffle();
}

void Mahjong::deal_tile_to_player(Player *player) {
    player->deal_tile(_set->take_tile());
}

void Mahjong::deal_tiles() {
    // Error checking
    for (auto player : _players) {
        if (player->get_num_total_tiles()) {
            throw std::runtime_error("Players should start with no tiles");
        }
    }

    // Start dealing the expected number of tiles to each player
    for (uint8_t num_tile = 0; num_tile < TILES_PER_PLAYER; num_tile++) {
        for (auto player : _players) {
            // printf("Dealing to %s", player->get_name().c_str());
            deal_tile_to_player(player);
        }
    }

    // Replenish flowers since they don't count as tile in hand
    for (auto player : _players) {
        while (player->get_num_tiles_in_hand() != TILES_PER_PLAYER) {
            deal_tile_to_player(player);
        }
    }

    // First player starts with 1 extra tile
    Player *first_player = _players[0];
    while (first_player->get_num_tiles_in_hand() != (TILES_PER_PLAYER + 1)) {
        deal_tile_to_player(first_player);
    }

    for (auto player : _players) {
        player->sort_hand();
    }
}

void Mahjong::print_players_hands() {
    for (auto player : _players) {
        player->print_hand();
    }
}

void Mahjong::pass_3_tiles_to_next_player() {
    for (auto player : _players) {
        Bot * bot_player = dynamic_cast<Bot*>(player);
        if (bot_player) {
            bot_player->preprocess_hand();
        }
    }
}

int main() {
    printf("Welcome to mahjong\n\n");

    Mahjong my_mahjong = Mahjong();
    my_mahjong.create_match({"Yo", "Player 2", "Player 3", "Player 4"});
    // my_mahjong.print_players_hands();
    my_mahjong.pass_3_tiles_to_next_player();

    printf("Mahjong ended\n");

    return 0;
}

#endif