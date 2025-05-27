#ifndef MAHJONG_CC
#define MAHJONG_CC

#include "mahjong.h"

// TODO: Receive name of player and number of bots
void Mahjong::create_match(std::list<std::string> list_of_player_names) {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    if (list_of_player_names.size() < MIN_NUM_PLAYERS ||
        list_of_player_names.size() > MAX_NUM_PLAYERS) {
        throw std::runtime_error("Game should have at least 1 player");
    }

    _logger->info("Will create players");
    create_players(list_of_player_names);
    _logger->info("Will create set");
    create_set();
    _logger->info("Will deal tiles");
    deal_tiles();

    _logger->info("Will preprocess hands\n");
    for (auto player : _players) {
        Bot *bot_player = dynamic_cast<Bot *>(player);
        if (bot_player) {
            bot_player->preprocess_hand();
        }
    }

    _current_player_it = _players.begin();
}

void Mahjong::create_players(std::list<std::string> list_of_player_names) {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

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
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    _set.shuffle();
}

void Mahjong::deal_tile_to_player(Player *player, MahjongTilePtr tile) {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    player->deal_tile(tile);
}

void Mahjong::deal_tiles() {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    // Error checking
    for (auto player : _players) {
        if (player->get_num_total_tiles()) {
            throw std::runtime_error("Players should start with no tiles");
        }
    }

    // Start dealing the expected number of tiles to each player
    for (uint8_t num_tile = 0; num_tile < TILES_PER_PLAYER; num_tile++) {
        for (auto player : _players) {
            auto tileToDeal = _set.take_tile();
            deal_tile_to_player(player, tileToDeal);
        }
    }

    // Replenish flowers since they don't count as tile in hand
    for (auto player : _players) {
        while (player->get_num_tiles_in_hand() != TILES_PER_PLAYER) {
            auto tileToDeal = _set.take_tile();
            deal_tile_to_player(player, tileToDeal);
        }
    }

    // First player starts with 1 extra tile
    Player *first_player = _players[0];
    while (first_player->get_num_tiles_in_hand() != (TILES_PER_PLAYER + 1)) {
        auto tileToDeal = _set.take_tile();
        deal_tile_to_player(first_player, tileToDeal);
    }

    for (auto player : _players) {
        player->sort_hand();
    }
}

void Mahjong::print_players_hands() {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    for (auto player : _players) {
        player->print_hand();
    }
}

void Mahjong::pass_3_tiles_to_next_player() {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    // Bot * bot_player = dynamic_cast<Bot*>(player);
    // if (bot_player) {
    uint8_t player_num = 1;
    for (auto player_it = _players.begin(); player_it != _players.end();
         ++player_it) {
        Player *player = *player_it;

        if (player_num == (_players.size())) {
            player->pass_3_tiles(_players[0]);
        } else {
            auto next_player_it = std::next(player_it, 1);
            Player *next_player = *next_player_it;
            if (next_player) {
                player->pass_3_tiles(next_player);
            }
        }
        ++player_num;
    }

    for (auto player : _players) {
        player->move_received_tiles_to_hand();
    }
}

bool Mahjong::play() {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    bool game_is_over = false;
    Player *current_player = *_current_player_it;

    printf("Player %s will play\n", current_player->get_name().c_str());

    if (_first_player_played) {
        deal_tile_to_player(current_player);
    } else {
        _first_player_played = true;
    }

    if (_set.get_num_tiles() == 0) {
        _logger->info("Ending due to no more tiles left :(");
        return true;
    }

    current_player->play();
    auto tile_discarded = current_player->get_tile_to_discard();

    _discards.push_back(tile_discarded);
    
    printf("%s was discarded\n", tile_discarded->get_full_name().c_str());

    if (current_player->has_won()) {
        printf("%s has won!\n", current_player->get_name().c_str());
        current_player->print_hand();
        current_player->print_sets();
        game_is_over = true;
    }
    if (_set.get_num_tiles() == 0) {
        printf("No more tiles to play\n");
        game_is_over = true;
    }

    update_current_player();

    return game_is_over;
}

void Mahjong::update_current_player() {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    ++_current_player_it;
    if (_current_player_it == _players.end()) {
        _current_player_it = _players.begin();
    }
}

void Mahjong::deal_tile_to_player(Player *current_player) {
    _logger->debug("%s::%s", CLASSNAME, __func__); 

    if (!_discards.size()) {
        throw std::runtime_error("Last discard is NULL!");
    }

    auto last_discard = _discards[0];

    if (current_player->wants_discard_tile(last_discard)) {
        deal_tile_to_player(current_player, last_discard);
        _discards.erase(_discards.begin());
    } else {
        do {
            if (_set.get_num_tiles() == 0) {
                return;
            }
            auto tileToDeal = _set.take_tile();
            if (!tileToDeal) {
                throw std::runtime_error("Tile is NULL!");
            }
            deal_tile_to_player(current_player, tileToDeal);
        } while (current_player->get_num_tiles_set_and_hand() !=
                 TILES_PER_PLAYER+1);
    }
}

int fake_main() {
    printf("Welcome to mahjong\n\n");

    Logger::instance().set_level(LogLevel::DEBUG);

    Mahjong my_mahjong = Mahjong();
    my_mahjong.create_match({"Yo", "Player 2", "Player 3", "Player 4"});
    my_mahjong.pass_3_tiles_to_next_player();
    my_mahjong.print_players_hands();

    bool game_is_over = false;
    do {
        game_is_over = my_mahjong.play();
    } while(!game_is_over);

    printf("Mahjong ended\n\n\n\n");

    return 0;
}

#endif
