#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "logger.h"
#include "player.h"
#include "tiles.h"
#include "mahjong.h"

class GameEngine {
  public:
    explicit GameEngine()
        : kToolsPath_(MAHJONG_TOOLS),
          kSoundtracks_({kToolsPath_ + "/soundtracks/soundtrack_1.mp3",
                         kToolsPath_ + "/soundtracks/soundtrack_2.mp3",
                         kToolsPath_ + "/soundtracks/soundtrack_3.mp3"}) {
        logger_ = &Logger::Instance();
        GetAspectRatio();
        SetGameResolution();
        RenderWindow();
        ConfigureText();
        UpdateSoundtrack();
        PrintRedDragon();
        GetTileParams();
        LoadIcon();
    }

    sf::RenderWindow *GetWindow() { return &window_; }

    void HandleEvents() {
        while (const std::optional event = window_.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window_.close();
            } else if (const auto *key_pressed =
                           event->getIf<sf::Event::KeyPressed>()) {
                if (key_pressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window_.close();
                }
            }
        }
    }

    void LoadIcon() {
        sf::Image icon;
        auto iconPath = kToolsPath_ + "/mahjong-icon.png";
        if (icon.loadFromFile(iconPath)) {
            window_.setIcon(icon);
        } else {
            std::cerr << "Failed to load icon.png" << std::endl;
        }

    }

    void GetAspectRatio() {
        video_modes_ = sf::VideoMode::getFullscreenModes();
        max_width_ = video_modes_[0].size.x;
        max_height_ = video_modes_[0].size.y;
    }

    void SetGameResolution() {
        // Use second resolution as default
        const auto kSecondResolution = 1;
        const auto kFirstResolution = 0;

        const auto default_res =
            (video_modes_.size() >= 2) ? kSecondResolution : kFirstResolution;

        resolution_width_ = video_modes_[default_res].size.x;
        resolution_height_ = video_modes_[default_res].size.y;
        ratio_ = static_cast<double>(resolution_width_) /
                 static_cast<double>(resolution_height_);
        logger_->Info("Resolution set to W = %d, H = %d, ratio = %f",
                      resolution_width_, resolution_height_, ratio_);
    }

    void RenderWindow() {
        window_.create(sf::VideoMode({resolution_width_, resolution_height_}),
                       "Mahjong");

        int middle_position_x = (max_width_ - resolution_width_) / 2;
        int middle_position_y = (max_height_ - resolution_height_) / 2;

        window_.setPosition({middle_position_x, middle_position_y});
    }

    void ConfigureText() {
        std::string font_path = kToolsPath_;
        font_path.append("/fonts/arial.ttf");
        font_ = new sf::Font(font_path);
        text_ = new sf::Text(*font_);
        text_->setString("Welcome to Mahjong");
        text_->setCharacterSize(32);
        text_->setFillColor(sf::Color::Black);
        text_->setStyle(sf::Text::Bold | sf::Text::Underlined);

        float text_position_x = resolution_width_ / 2.0 - (24 * 5);
        float text_position_y = resolution_height_ / 100.0;

        text_->setPosition({text_position_x, text_position_y});
    }

    void PrintRedDragon() {
        std::string red_dragon_path = kToolsPath_;
        red_dragon_path.append("/sprites/Red.png");
        red_dragon_texture_ = new sf::Texture(red_dragon_path);
        red_dragon_sprite_ = new sf::Sprite(*red_dragon_texture_);
    }

    void GetTileParams() {
        std::string tile_path = kToolsPath_;
        tile_path.append("/sprites/Red.png");
        auto sample_texture = sf::Texture(tile_path);

        auto scale = sample_texture.getSize();
        tile_ratio_ = static_cast<float>(scale.y) / static_cast<float>(scale.x);

        logger_->Info("spriteWidth = %d, spriteHeight = %d, ratio = %f",
                      scale.x, scale.y, tile_ratio_);

        int tiles_per_board = 50;
        float pixels_per_tile = resolution_width_ / tiles_per_board;
        tile_scale_ = pixels_per_tile / scale.x;
        tile_real_width_ = pixels_per_tile;

        logger_->Info("tilesPerBoard = %d", tiles_per_board);
    }

    void CheckSoundtrackFinished() {
        auto elapsed_time = sound_clock_.getElapsedTime().asSeconds();
        if (elapsed_time >= kSoundtrackDurationSecs_[current_soundtrack_num_]) {
            UpdateSoundtrack();
        }
    }

    void UpdateSoundtrack() {
        current_soundtrack_num_ = rand() % 3;
        delete music_;
        music_ = new sf::Music(kSoundtracks_[current_soundtrack_num_]);
        PlaySoundtrack();
    }

    void PlaySoundtrack() {
        sound_clock_.restart();
        music_->play();
    }

    std::string GetSpritePathFromTile(MahjongTile &tile) {
        auto tile_group_name = tile.GetGroup().name;
        std::string tile_name = kToolsPath_;
        tile_name.append("/sprites/");

        if (tile_group_name == "Flower") {
            // TODO: Get propper sprite for this
            tile_name.append("White.png");
        } else if (tile_group_name == "Dragon") {
            tile_name.append(tile.GetName()).append(".png");
        } else if (tile_group_name == "Wind") {
            tile_name.append(tile.GetName()).append(".png");
        } else if (tile_group_name == "Symbol") {
            auto *tile_numerical = dynamic_cast<MahjongTileNumerical *>(&tile);
            auto tile_num = tile_numerical->GetValue();
            tile_name.append("Characters")
                .append(std::to_string(tile_num))
                .append(".png");
        } else if (tile_group_name == "Bamboo") {
            auto *tile_numerical = dynamic_cast<MahjongTileNumerical *>(&tile);
            auto tile_num = tile_numerical->GetValue();
            tile_name.append("Bamboo")
                .append(std::to_string(tile_num))
                .append(".png");
        } else if (tile_group_name == "Dot") {
            auto *tile_numerical = dynamic_cast<MahjongTileNumerical *>(&tile);
            auto tile_num = tile_numerical->GetValue();
            tile_name.append("Circles")
                .append(std::to_string(tile_num))
                .append(".png");
        } else {
            throw std::runtime_error("Oh boy");
        }

        return tile_name;
    }

    void DisplayGame() {
        assert(!players_.empty());

        window_.clear(sf::Color::Green);

        auto firstPlayer = players_[0];
        DisplayPlayer(*firstPlayer);
        DisplayDiscards();

        window_.display();
    }

    void DisplayPlayer(Player &player) {
        const auto hand = player.GetHand();

        int x_offset = resolution_width_ / 2;
        int y_offset = resolution_height_ * 9 / 10;

        DisplaySetsMat(x_offset, y_offset);
        DisplayPlayersMat(x_offset, y_offset);
        DisplayTiles(hand, y_offset, x_offset);
        DisplayTilesNumber(x_offset, y_offset, player.GetNumTilesInHand());
        // window_.draw(*text_);
    }

    void DisplayDiscards() {
        DisplayDiscardedMat();
    }


    void DisplayTiles(const MahjongHand& hand, int center_y, int center_x) {

        // TODO: auto hand_copy = hand;
        std::vector<std::string> tile_paths;
        for (const auto &tile_ptr : hand) {
            auto tile_path = GetSpritePathFromTile(*tile_ptr);
            tile_paths.push_back(tile_path);
        }

        float x_offset = center_x - (tile_real_width_ * 14);
        float y_offset = center_y;

        logger_->Info("x_offset = %f, y_offset = %f", x_offset, y_offset);
        logger_->Info("tileScale = %f", tile_scale_);
        logger_->Info("_tileRealWidth= %d", tile_real_width_);

        for (const auto &tile_path : tile_paths) {
            sf::Texture tile_texture(tile_path);
            sf::Sprite tile_sprite(tile_texture);
            tile_sprite.setScale({tile_scale_, tile_scale_});
            tile_sprite.setPosition({x_offset, y_offset});
            window_.draw(tile_sprite);
            x_offset += tile_real_width_ * 2;
        }
    }


    void DisplayPlayersMat(const int tiles_x_offset, const int tiles_y_offset) {
        const float tile_w = tile_real_width_ * 29;
        const float tile_h = tile_real_width_ * 3;
        sf::RectangleShape rectangle({tile_w, tile_h});
        rectangle.setOrigin({tile_w / 2, 0});

        const float pos_x = resolution_width_ / 2;
        const float pos_y = tiles_y_offset - tile_real_width_;
        rectangle.setPosition({pos_x, pos_y});
        // R G B, min is 1, max 100
        rectangle.setFillColor(sf::Color(1, 100, 1));
        rectangle.setOutlineThickness(tile_real_width_/10);
        rectangle.setOutlineColor(sf::Color::Black);
        window_.draw(rectangle);
    }

    void DisplaySetsMat(const int tiles_x_offset, const int tiles_y_offset) {
        const float tile_w = tile_real_width_ * 29;
        const float tile_h = tile_real_width_ * 3;
        sf::RectangleShape rectangle({tile_w, tile_h});
        rectangle.setOrigin({tile_w / 2, 0});

        const float pos_x = resolution_width_ / 2;
        const float pos_y = tiles_y_offset - (tile_real_width_ * 4.5);
        rectangle.setPosition({pos_x, pos_y});

        rectangle.setFillColor(sf::Color(1, 150, 1));
        rectangle.setOutlineThickness(tile_real_width_/10);
        rectangle.setOutlineColor(sf::Color::Black);
        window_.draw(rectangle);

        auto text = new sf::Text(*font_);
        const auto firstPlayersName = GetFirstPlayersName();
        text->setString(firstPlayersName + "'s Sets");
        text->setCharacterSize(tile_real_width_ / 1.5);
        text->setFillColor(sf::Color::Black);
        text->setStyle(sf::Text::Bold);
        text->setOrigin({tile_real_width_ * 1.5, 0});

        const float text_position_x = resolution_width_ / 2.0;
        const float text_position_y = pos_y ;
        text->setPosition({text_position_x, text_position_y});

        window_.draw(*text);
    }

    void DisplayTilesNumber(const int tiles_x_offset, const int tiles_y_offset, const int num_tiles) {
        float text_position_x = tiles_x_offset - (tile_real_width_ * 14);
        const float text_position_y = tiles_y_offset - tile_real_width_ ;

        auto text = new sf::Text(*font_);

        for (int i = 1; i <= num_tiles; i++) {
            text->setString(std::to_string(i));
            text->setCharacterSize(tile_real_width_ / 1.5);
            text->setFillColor(sf::Color::Black);
            text->setStyle(sf::Text::Bold);

            text->setPosition({text_position_x, text_position_y});
            window_.draw(*text);
            text_position_x += (tile_real_width_ * 2.0);
        }
    }

    void DisplayDiscardedMat() {
        const float tile_w = tile_real_width_ * 29.0;
        const float tile_h = tile_real_width_ * tile_ratio_ * 10;
        sf::RectangleShape rectangle({tile_w, tile_h});
        rectangle.setOrigin({tile_w / 2.0, 0});

        const float pos_x = resolution_width_ / 2.0;
        const float pos_y = resolution_height_ / 7;
        rectangle.setPosition({pos_x, pos_y});

        rectangle.setFillColor(sf::Color(1, 100, 1));
        rectangle.setOutlineThickness(tile_real_width_ / 10.0);
        rectangle.setOutlineColor(sf::Color::Black);
        window_.draw(rectangle);
    }

    // TODO: Needed?
    void DisplayTile(std::string tile_path, float y, float x) {
        sf::Texture tile_texture(tile_path);
        sf::Sprite tile_sprite(tile_texture);

        tile_sprite.setScale({tile_scale_, tile_scale_});
        tile_sprite.setPosition({y, x});
        window_.clear(sf::Color::Green);
        window_.draw(*text_);
        window_.draw(tile_sprite);
        window_.display();
    }

    void SetPlayers(PlayersVector& players) {
        players_ = players; 

        for (const auto& player : players_) {
            logger_->Info("Ready player: %s", player->GetName().c_str());
        }

    }

    std::string GetFirstPlayersName() {
        const auto firstPlayer = players_[0];

        return firstPlayer->GetName();
    }

  private:
    const std::string kToolsPath_;
    const std::vector<int> kSoundtrackDurationSecs_ = {240, 220, 120};
    const std::vector<std::string> kSoundtracks_;

    Logger *logger_;
    std::vector<sf::VideoMode> video_modes_;
    uint16_t max_height_ = 0;
    uint16_t max_width_ = 0;
    uint16_t resolution_width_ = 0;
    uint16_t resolution_height_ = 0;
    float ratio_ = 0;
    sf::RenderWindow window_;
    sf::Font *font_ = nullptr;
    sf::Text *text_ = nullptr;
    sf::Texture *red_dragon_texture_ = nullptr;
    sf::Sprite *red_dragon_sprite_ = nullptr;
    sf::Music *music_ = nullptr;
    sf::Clock sound_clock_;
    int current_soundtrack_num_ = 2;

    int tile_height_ = 0;
    float tile_scale_ = 0;
    float tile_ratio_ = 0;
    int tile_real_width_ = 0;

    PlayersVector players_;
};
