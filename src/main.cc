#include "game_engine.h"
#include "mahjong.h"

void SimulateRun() {
    printf("Welcome to mahjong\n\n");

    Mahjong my_mahjong = Mahjong();
    my_mahjong.CreateMatch({"Yo", "Player 2", "Player 3", "Player 4"});
    my_mahjong.Pass3TilesToNextPlayer();
    my_mahjong.PrintPlayersHands();

    bool game_is_over = false;
    do {
        game_is_over = my_mahjong.Play();
    } while (!game_is_over);

    printf("Mahjong ended\n\n\n\n");
}

int main() {
    Logger::Instance().SetLevel(LogLevel::kInfo);

    bool simulate_game = false;

    if (simulate_game) {
        SimulateRun();
        return 0;
    }

    GameEngine game_engine;
    game_engine.DisplayGame();

    Mahjong my_mahjong = Mahjong();
    my_mahjong.CreateMatch({"Yo", "Player 2", "Player 3", "Player 4"});
    my_mahjong.Pass3TilesToNextPlayer();
    // my_mahjong.PrintPlayersHands();
    auto *players_ptr = my_mahjong.GetPlayers();
    auto players_vector = *players_ptr;
    auto *first_player = players_vector[0];

    game_engine.DisplayPlayerTiles(*first_player);
    game_engine.DisplayPlayerTiles(*first_player);

    while (game_engine.GetWindow()->isOpen()) {
        game_engine.HandleEvents();
        game_engine.CheckSoundtrackFinished();
    }

    return 0;
}
