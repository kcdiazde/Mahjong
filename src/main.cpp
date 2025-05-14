#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    // Display the list of all the video modes available for fullscreen
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();

    const auto MAX_HEIGHT = modes[0].size.x;
    const auto MAX_WIDTH = modes[0].size.y;

    auto defaultHeight = (modes.size() >= 2) ? 1 : 0;
    auto currentWidth= modes[defaultHeight].size.y;
    auto currentHeight = modes[defaultHeight].size.x;
    float ratio = currentHeight/currentWidth;
    std::cout << "Width = " << currentWidth << " Height = " << currentHeight << std::endl;

    sf::RenderWindow window;
    window.create(sf::VideoMode({currentHeight, currentWidth}), "Mahjong");
    float middlePositionY = (MAX_WIDTH - currentWidth);
    float middlePositionX = (MAX_HEIGHT - currentHeight)/4;
    window.setPosition({middlePositionY, middlePositionX});

    // sf::CircleShape shape(100.f);
    // shape.setFillColor(sf::Color::Green);

    sf::Font font("../tools/arial.ttf");
    sf::Text text(font);
    text.setString("Welcome to Mahjong");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    text.setPosition({100.f, 100.f});

    sf::Texture redDragonTexture("../mahjong_tiles/Red.png");
    auto [width, height] = redDragonTexture.getSize();
    sf::Sprite redDragonSprite(redDragonTexture);
    auto [spriteWidth, spriteHeight] = redDragonSprite.getScale();
    // Magic numbers that look nice
    auto divW = currentWidth / 500 / spriteWidth;
    redDragonSprite.setScale({divW, divW * ratio});


    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
            }
        }

        window.clear(sf::Color::Green);
        // window.draw(shape);
        window.draw(text);
        window.draw(redDragonSprite);
        window.display();
    }

    return 0;
}

