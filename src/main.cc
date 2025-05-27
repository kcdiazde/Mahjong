#include "game_engine.h"
#include "mahjong.h"

void simulateRun() {
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
}

int main() {

    bool simulate_game = false;

    if (simulate_game) {
        simulateRun();
        return 0;
    }

    GameEngine gameEngine; 
    gameEngine.display_game();
    Logger::instance().set_level(LogLevel::VERBOSE);

    Mahjong my_mahjong = Mahjong();
    my_mahjong.create_match({"Yo", "Player 2", "Player 3", "Player 4"});
    my_mahjong.pass_3_tiles_to_next_player();
    my_mahjong.print_players_hands();
    auto * playerVecP = my_mahjong.get_players();
    auto playerVec = * playerVecP;
    auto * firstPlayer = playerVec[0];

    gameEngine.display_player_tiles(*firstPlayer);

    while (gameEngine.getWindow()->isOpen())
    {
        gameEngine.handle_events();
        gameEngine.check_soundtrack_finished();
    }

    return 0;
}

