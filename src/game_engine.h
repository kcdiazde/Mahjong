#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "tiles.h"
#include "player.h"
#include "logger.h"
#include <iostream>

class GameEngine {

    const std::string TOOLS_PATH = MAHJONG_TOOLS;

    std::string TOOLS_PATH_1 = TOOLS_PATH;
    const std::string SOUND_1 = TOOLS_PATH_1.append("/soundtracks/soundtrack_1.mp3");

    std::string TOOLS_PATH_2 = TOOLS_PATH;
    const std::string SOUND_2 = TOOLS_PATH_2.append("/soundtracks/soundtrack_2.mp3");

    std::string TOOLS_PATH_3 = TOOLS_PATH;
    const std::string SOUND_3 = TOOLS_PATH_3.append("/soundtracks/soundtrack_3.mp3");

    const std::vector<int> SOUNDTRACK_DURATION_SECS = {240, 220, 120};
    const std::vector<std::string> SOUNDTRACKS = {SOUND_1, SOUND_2, SOUND_3};

  public:
    explicit GameEngine () {
        _logger = &Logger::instance(); 
        get_aspect_ratio();
        set_game_resolution();
        render_window();
        configure_text();
        update_soundtrack();
        print_red_dragon();
        get_tile_params();
    }

  private:
    Logger* _logger;
    std::vector<sf::VideoMode> _videoModes;
    uint16_t _maxHeight = 0;
    uint16_t _maxWidth = 0;
    uint16_t _resolutionWidth = 0;
    uint16_t _resolutionHeight = 0;
    float _ratio = 0;
    sf::RenderWindow _window;
    sf::Font * _font = nullptr;
    sf::Text * _text = nullptr;
    sf::Texture * _redDragonTexture = nullptr;
    sf::Sprite * _redDragonSprite = nullptr;
    sf::Music * _music = nullptr;
    sf::Clock _soundClock;
    int _currentSoundtrackNum = 2;

    int _tileWidth = 0;
    int _tileHeight = 0;
    int _tileScale = 0;
    int _tileRealWidth = 0;

  public:
    sf::RenderWindow * getWindow() {
        return &_window;
    }

    void handle_events() {
        while (const std::optional event = _window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                _window.close();
            } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    _window.close();
                }
            }
        }
    }

    void get_aspect_ratio() {
        // List of all video modes
        _videoModes = sf::VideoMode::getFullscreenModes();
        _maxWidth = _videoModes[0].size.x;
        _maxHeight = _videoModes[0].size.y;
    }

    void set_game_resolution() {
        // Use second resolution as default
        const auto kSecondResolution = 1;
        const auto kFirstResolution = 0;

        const auto defaultRes = (_videoModes.size() >= 2) ? kSecondResolution : kFirstResolution;

        _resolutionWidth = _videoModes[defaultRes].size.x;
        _resolutionHeight = _videoModes[defaultRes].size.y;
        _ratio = static_cast<double>(_resolutionWidth) / static_cast<double>(_resolutionHeight);
        _logger->info("Resolution set to W = %d, H = %d, ratio = %f", _resolutionWidth, _resolutionHeight, _ratio);
    }

    void render_window() {
        _window.create(sf::VideoMode({_resolutionWidth, _resolutionHeight}), "Mahjong");

        float middlePositionX = (_maxWidth - _resolutionWidth) / 2;
        float middlePositionY = (_maxHeight - _resolutionHeight) / 2;

        _window.setPosition({middlePositionX, middlePositionY});
    }

    void configure_text() {
        // TODO: static constexpr
        std::string fontPath = MAHJONG_TOOLS;
        fontPath.append("/fonts/arial.ttf");
        _font = new sf::Font(fontPath);
        _text = new sf::Text(*_font);
        _text->setString("Welcome to Mahjong");
        _text->setCharacterSize(24);
        _text->setFillColor(sf::Color::Black);
        _text->setStyle(sf::Text::Bold | sf::Text::Underlined);
            
        auto textPositionX = _resolutionWidth / 2 - (24 * 5);
        auto textPositionY = _resolutionHeight / 4;


        _text->setPosition({textPositionX, textPositionY});
    }

    void print_red_dragon() {
        std::string redDragonPath = MAHJONG_TOOLS;
        redDragonPath.append("/sprites/Red.png");
        _redDragonTexture = new sf::Texture(redDragonPath);
        _redDragonSprite = new sf::Sprite(*_redDragonTexture);

        // // auto [width, height] = _redDragonTexture->getSize();
        // auto [spriteWidth, spriteHeight] = _redDragonSprite->getScale();
        // auto divW = _resolutionWidth / 500 / spriteWidth;
        // _redDragonSprite->setScale({divW, divW * _ratio});
    }

    void get_tile_params() {
        std::string tilePath = MAHJONG_TOOLS;
        tilePath.append("/sprites/Red.png");
        auto sampleTexture = sf::Texture(tilePath);
        auto sampleSprite = sf::Sprite(sampleTexture);

        auto scale = sampleTexture.getSize();
        float tile_ratio = static_cast<float>(scale.y) / static_cast<float>(scale.x);

        _logger->info("spriteWidth = %d, spriteHeight = %d, ratio = %f", scale.x, scale.y, tile_ratio);

        int tilesPerBoard = _resolutionWidth / 700.0;
        _tileScale = tile_ratio * tilesPerBoard;
        _tileWidth = tilesPerBoard;
        _tileRealWidth = scale.x * _tileScale;
    }

    void check_soundtrack_finished() {
        auto elapsedTime = _soundClock.getElapsedTime().asSeconds();
        if (elapsedTime >= SOUNDTRACK_DURATION_SECS[_currentSoundtrackNum]) {
            update_soundtrack();        
        }
    }

    void update_soundtrack() {
        _currentSoundtrackNum = rand() % 3;
        delete _music;
        _music = new sf::Music(SOUNDTRACKS[_currentSoundtrackNum]);
        play_soundtrack();
    }

    void play_soundtrack() {
        _soundClock.restart();
        _music->play();
    }

    void display_game() {
        _window.clear(sf::Color::Green);
        _window.draw(*_text);
        _window.draw(*_redDragonSprite);
        _window.display();
    }

    std::string get_sprint_from_tile(MahjongTile& tile) {
        auto tileGroupName = tile.get_group().name;
        std::string tile_name = MAHJONG_TOOLS;
        tile_name.append("/sprites/");

        if (tileGroupName == "Flower") {
            // TODO: Get propper sprite for this
            tile_name.append("White.png");
        } else if (tileGroupName == "Dragon") {
            tile_name.append(tile.get_name()).append(".png");
        } else if (tileGroupName == "Wind") {
            tile_name.append(tile.get_name()).append(".png");
        } else if (tileGroupName == "Symbol") {
            auto * tileNumerical = dynamic_cast<MahjongTileNumerical*>(&tile);
            auto tileNum = tileNumerical->get_value();
            tile_name.append("Characters").append(std::to_string(tileNum)).append(".png");
        } else if (tileGroupName == "Bamboo") {
            auto * tileNumerical = dynamic_cast<MahjongTileNumerical*>(&tile);
            auto tileNum = tileNumerical->get_value();
            tile_name.append("Bamboo").append(std::to_string(tileNum)).append(".png");
        } else if (tileGroupName == "Dot") {
            auto * tileNumerical = dynamic_cast<MahjongTileNumerical*>(&tile);
            auto tileNum = tileNumerical->get_value();
            tile_name.append("Circles").append(std::to_string(tileNum)).append(".png");
        } else {
            throw std::runtime_error("Oh boy");
        }

        return tile_name;
    }

    void display_tiles(std::vector<std::string>& tilePaths, int y, int x) {
        _window.clear(sf::Color::Green);

        int x_offset = x - (_tileRealWidth * 14);
        int y_offset = y;

        _logger->info("x_offset = %d, y_offset = %d", x_offset, y_offset);
        _logger->info("tileScale = %d", _tileScale);
        _logger->info("_tileRealWidth= %d", _tileRealWidth);

        for (const auto& tilePath : tilePaths) {
            sf::Texture tileTexture(tilePath);
            sf::Sprite tileSprite(tileTexture);
            tileSprite.setScale({_tileScale, _tileScale});
            tileSprite.setPosition({x_offset, y_offset});
            _window.draw(tileSprite);
            x_offset += _tileRealWidth * 2;
        }

        _window.draw(*_text);
        _window.display();
    }


    void display_player_tiles(Player& player) {
        std::cout << "Player is: " << player.get_name() << std::endl;
        MahjongHand * player_hand = player.get_hand();
        MahjongHand hand = *player_hand;
        
        // int y_offset = (_resolutionWidth / 2) - (_tileWidth * 8 * _ratio);
        int x_offset = _resolutionWidth / 2;
        int y_offset = _resolutionHeight * 3 / 4;

        auto hand_copy = hand;

        std::vector<std::string> tilePaths;
        for (const auto& tilePtr : hand_copy) {
            auto tilePath = get_sprint_from_tile(*tilePtr);

            std::cout << "Tile is: " << tilePtr->get_full_name() << std::endl;
            std::cout << "Tile path is: " << tilePath << std::endl;
            tilePaths.push_back(tilePath);
        }

        display_tiles(tilePaths, y_offset, x_offset);

    }

    void display_tile(std::string tilePath, float y, float x) {
        sf::Texture tileTexture(tilePath);
        sf::Sprite tileSprite(tileTexture);

        tileSprite.setScale({_tileWidth, _tileWidth * _ratio});
        tileSprite.setPosition({y, x});
        _window.clear(sf::Color::Green);
        _window.draw(*_text);
        _window.draw(tileSprite);
        _window.display();
    }

};
