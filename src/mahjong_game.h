#pragma once

#include <SFML/Graphics.hpp>

class MahjongGame {

  public:
    explicit MahjongGame() {
        get_aspect_ratio();
        set_game_resolution();
        render_window();
        configure_text();
        print_red_dragon();
    }

  private:
    std::vector<sf::VideoMode> _videoModes;
    uint16_t _maxHeight = 0;
    uint16_t _maxWidth = 0;
    uint16_t _resolutionWidth = 0;
    uint16_t _resolutionHeight = 0;
    float _ratio = 0;
    sf::RenderWindow _window;
    // TODO: Set path relative to project location
    sf::Font * _font = nullptr;
    sf::Text * _text = nullptr;
    sf::Texture * _redDragonTexture = nullptr;
    sf::Sprite * _redDragonSprite = nullptr;

  public:
    sf::RenderWindow * getWindow() {
        return &_window;
    }

    void get_aspect_ratio() {
        // List of all video modes
        _videoModes = sf::VideoMode::getFullscreenModes();
        _maxHeight = _videoModes[0].size.x;
        _maxWidth = _videoModes[0].size.y;
    }

    void set_game_resolution() {
        // Use second resolution as default
        const auto kSecondResolution = 1;
        const auto kFirstResolution = 0;
        const auto defaultHeight = (_videoModes.size() >= 2) ? kSecondResolution : kFirstResolution;

        _resolutionWidth= _videoModes[defaultHeight].size.y;
        _resolutionHeight = _videoModes[defaultHeight].size.x;
        _ratio = _resolutionHeight / _resolutionWidth;
    }

    void render_window() {
        _window.create(sf::VideoMode({_resolutionHeight, _resolutionWidth}), "Mahjong");

        float middlePositionY = (_maxWidth - _resolutionWidth);
        float middlePositionX = (_maxHeight - _resolutionHeight) / 4;

        _window.setPosition({middlePositionY, middlePositionX});
    }

    void configure_text() {
        _font = new sf::Font("../tools/arial.ttf");
        _text = new sf::Text(*_font);
        _text->setString("Welcome to Mahjong");
        _text->setCharacterSize(24);
        _text->setFillColor(sf::Color::Black);
        _text->setStyle(sf::Text::Bold | sf::Text::Underlined);
        _text->setPosition({100.f, 100.f});
    }

    void print_red_dragon() {
        _redDragonTexture = new sf::Texture("../mahjong_tiles/Red.png");
        _redDragonSprite = new sf::Sprite(*_redDragonTexture);
        _redDragonSprite->setTexture(*_redDragonTexture);

        auto [width, height] = _redDragonTexture->getSize();
        auto [spriteWidth, spriteHeight] = _redDragonSprite->getScale();
        // Magic numbers that look nice
        auto divW = _resolutionWidth / 500 / spriteWidth;
        _redDragonSprite->setScale({divW, divW * _ratio});
    }

    void display_game() {
        _window.clear(sf::Color::Green);
        if (_text) {
            _window.draw(*_text);
        }
        if (_redDragonSprite) {
            _window.draw(*_redDragonSprite);
        }
        _window.display();
    }


};
