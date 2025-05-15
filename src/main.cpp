#include "game_engine.h"
#include "mahjong.h"

int main() {
    GameEngine gameEngine; 
    gameEngine.display_game();

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
