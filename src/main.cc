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

    Mahjong my_mahjong = Mahjong();
    my_mahjong.CreateMatch({"P1", "P2", "P3", "P4"});
    my_mahjong.Pass3TilesToNextPlayer();
    // my_mahjong.PrintPlayersHands();

    PlayersVector players = my_mahjong.GetPlayers();
    game_engine.SetPlayers(players);

    game_engine.DisplayGame();

    while (game_engine.GetWindow()->isOpen()) {
        game_engine.HandleEvents();
        game_engine.CheckSoundtrackFinished();

    }

    Logger::Instance().SetLevel(LogLevel::kQuiet);

    return 0;
}
