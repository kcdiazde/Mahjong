#include <SFML/Graphics.hpp>
#include <iostream>
#include "mahjong_game.h"

int main() {
    MahjongGame mahjongGame; 

    auto window = mahjongGame.getWindow();

    while (window->isOpen())
    {
        while (const std::optional event = window->pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window->close();
            } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window->close();
                }
            }
        }

        mahjongGame.display_game();
    }

    return 0;

}
