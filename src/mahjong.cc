#include "mahjong.h"

// TODO: Receive name of player and number of bots
void Mahjong::CreateMatch(std::list<std::string> list_of_player_names) {
    logger_->Debug("%s::%s", kClassName, __func__);

    if (list_of_player_names.size() < kMinNumPlayers ||
        list_of_player_names.size() > kMaxNumPlayers) {
        throw std::runtime_error("Game should have at least 1 player");
    }

    logger_->Info("Will create players");
    CreatePlayers(list_of_player_names);
    logger_->Info("Will create set");
    CreateSet();
    logger_->Info("Will deal tiles");
    DealTiles();

    logger_->Info("Will preprocess hands\n");
    for (auto player : players_) {
        Bot *bot_player = dynamic_cast<Bot *>(player);
        if (bot_player) {
            bot_player->PreprocessHand();
        }
    }

    current_player_it_ = players_.begin();
}

void Mahjong::CreatePlayers(std::list<std::string> list_of_player_names) {
    logger_->Debug("%s::%s", kClassName, __func__);

    // Empty-out if already populated
    while (players_.size()) {
        Player *temp_player = players_.front();
        players_.erase(players_.begin());
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
        players_.push_back(new_player);
    }
}

void Mahjong::CreateSet() {
    logger_->Debug("%s::%s", kClassName, __func__);

    set_.Shuffle();
}

void Mahjong::DealTileToPlayer(Player *player, MahjongTilePtr tile) {
    logger_->Debug("%s::%s", kClassName, __func__);

    player->DealTile(tile);
}

void Mahjong::DealTiles() {
    logger_->Debug("%s::%s", kClassName, __func__);

    // Error checking
    for (auto player : players_) {
        if (player->GetNumTotalTiles()) {
            throw std::runtime_error("Players should start with no tiles");
        }
    }

    // Start dealing the expected number of tiles to each player
    for (uint8_t num_tile = 0; num_tile < kTilesPerPlayer; num_tile++) {
        for (auto player : players_) {
            auto tile_to_deal = set_.TakeTile();
            DealTileToPlayer(player, tile_to_deal);
        }
    }

    // Replenish flowers since they don't count as tile in hand
    for (auto player : players_) {
        while (player->GetNumTilesInHand() != kTilesPerPlayer) {
            auto tile_to_deal = set_.TakeTile();
            DealTileToPlayer(player, tile_to_deal);
        }
    }

    // First player starts with 1 extra tile
    Player *first_player = players_[0];
    while (first_player->GetNumTilesInHand() != (kTilesPerPlayer + 1)) {
        auto tile_to_deal = set_.TakeTile();
        DealTileToPlayer(first_player, tile_to_deal);
    }

    for (auto player : players_) {
        player->SortHand();
    }
}

void Mahjong::PrintPlayersHands() {
    logger_->Debug("%s::%s", kClassName, __func__);

    for (auto player : players_) {
        player->PrintHand();
    }
}

void Mahjong::Pass3TilesToNextPlayer() {
    logger_->Debug("%s::%s", kClassName, __func__);

    uint8_t player_num = 1;
    for (auto player_it = players_.begin(); player_it != players_.end();
         ++player_it) {
        Player *player = *player_it;

        if (player_num == (players_.size())) {
            player->Pass3Tiles(players_[0]);
        } else {
            auto next_player_it = std::next(player_it, 1);
            Player *next_player = *next_player_it;
            if (next_player) {
                player->Pass3Tiles(next_player);
            }
        }
        ++player_num;
    }

    for (auto player : players_) {
        player->MoveReceivedTilesToHand();
    }
}

void Mahjong::CheckPlayersIntegrity() {
    for (const auto &player : players_) {
        if (player->GetNumTilesSetAndHand() != kTilesPerPlayer) {
            char buffer[100];
            sprintf(buffer, "%s has %d tiles", player->GetName().c_str(),
                    player->GetNumTilesSetAndHand());
            std::string error = buffer;
            throw std::runtime_error(error);
        }
    }
}

bool Mahjong::Play() {
    logger_->Debug("%s::%s", kClassName, __func__);

    bool game_is_over = false;
    Player *current_player = *current_player_it_;

    logger_->Info("--- %s will play ---", current_player->GetName().c_str());

    if (first_player_played_) {
        DealTileToPlayer(current_player);
    } else {
        first_player_played_ = true;
    }

    if (set_.GetNumTiles() == 0) {
        logger_->Info("Ending due to no more tiles left :(");
        return true;
    }

    if (player_was_updated_) {
        current_player = *current_player_it_;
        player_was_updated_ = false;
    }

    current_player->Play();
    auto tile_discarded = current_player->GetTileToDiscard();

    discards_.push_back(tile_discarded);

    logger_->Info("%s discarded %s\n", current_player->GetName().c_str(),
                  tile_discarded->GetFullName().c_str());

    if (current_player->HasWon()) {
        logger_->Info("%s has won!\n", current_player->GetName().c_str());
        current_player->PrintHand();
        current_player->PrintSets();
        game_is_over = true;
    }
    if (set_.GetNumTiles() == 0) {
        logger_->Info("No more tiles to play\n");
        game_is_over = true;
    }

    UpdateCurrentPlayer();
    CheckPlayersIntegrity();

    return game_is_over;
}

void Mahjong::UpdateCurrentPlayer() {
    logger_->Debug("%s::%s", kClassName, __func__);

    ++current_player_it_;
    if (current_player_it_ == players_.end()) {
        current_player_it_ = players_.begin();
    }
}

bool Mahjong::CheckIfPlayerCanSteal(MahjongTile &tile) {
    for (const auto &player : players_) {
        if (player->IsTilePartialPung(tile)) {
            current_player_it_ =
                std::find(players_.begin(), players_.end(), player);
            player_was_updated_ = true;
            return true;
        }
    }

    return false;
}

void Mahjong::DealTileToPlayer(Player *current_player) {
    logger_->Debug("%s::%s", kClassName, __func__);

    if (!discards_.size()) {
        throw std::runtime_error("Last discard is NULL!");
    }

    auto last_discard = discards_[0];

    {
        auto player_can_steal = CheckIfPlayerCanSteal(*last_discard);
        if (player_can_steal) {
            auto new_player = *current_player_it_;
            if (new_player != current_player) {
                logger_->Info("%s steals Pung!!!",
                              new_player->GetName().c_str());
                current_player = new_player;
            }
        }
    }

    if (current_player->WantsDiscardTile(last_discard)) {
        logger_->Info("%s claims the discard: %s",
                      current_player->GetName().c_str(),
                      last_discard->GetFullName().c_str());
        DealTileToPlayer(current_player, last_discard);
        discards_.erase(discards_.begin());
    } else {
        logger_->Info("%s draws from the wall.",
                      current_player->GetName().c_str());
        do {
            if (set_.GetNumTiles() == 0) {
                return;
            }
            auto tile_to_deal = set_.TakeTile();
            logger_->Info("%s drawn", tile_to_deal->GetFullName().c_str());

            {
                auto player_can_steal = CheckIfPlayerCanSteal(*tile_to_deal);
                if (player_can_steal) {
                    auto new_player = *current_player_it_;
                    if (new_player != current_player) {
                        logger_->Info("%s steals Pung!!!",
                                      new_player->GetName().c_str());
                        current_player = new_player;
                    }
                }
            }

            if (!tile_to_deal) {
                throw std::runtime_error("Tile is NULL!");
            }
            DealTileToPlayer(current_player, tile_to_deal);
            if (current_player->GetNumTilesSetAndHand() < kTilesPerPlayer) {
                throw std::runtime_error("Player has incomplete hand");
            }
        } while (current_player->GetNumTilesSetAndHand() !=
                 kTilesPerPlayer + 1);
    }
}

int FakeMain() {
    printf("Welcome to mahjong\n\n");

    Logger::Instance().SetLevel(LogLevel::kDebug);

    Mahjong my_mahjong = Mahjong();
    my_mahjong.CreateMatch({"Yo", "Player 2", "Player 3", "Player 4"});
    my_mahjong.Pass3TilesToNextPlayer();
    // my_mahjong.PrintPlayersHands();

    bool game_is_over = false;
    do {
        game_is_over = my_mahjong.Play();
    } while (!game_is_over);

    printf("Mahjong ended\n\n\n\n");

    return 0;
}
