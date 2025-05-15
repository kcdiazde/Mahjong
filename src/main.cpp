#include "mahjong_game.h"

int main() {
    MahjongGame mahjongGame; 

    while (mahjongGame.getWindow()->isOpen())
    {
        mahjongGame.handle_events();
        mahjongGame.check_soundtrack_finished();
        mahjongGame.display_game();
    }

    return 0;

}
