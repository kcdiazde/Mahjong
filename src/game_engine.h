#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "logger.h"
#include "player.h"
#include "tiles.h"

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
  }

  sf::RenderWindow* GetWindow() { return &window_; }

  void HandleEvents() {
    while (const std::optional event = window_.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window_.close();
      } else if (const auto* key_pressed =
                     event->getIf<sf::Event::KeyPressed>()) {
        if (key_pressed->scancode == sf::Keyboard::Scancode::Escape) {
          window_.close();
        }
      }
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
    text_->setCharacterSize(24);
    text_->setFillColor(sf::Color::Black);
    text_->setStyle(sf::Text::Bold | sf::Text::Underlined);

    float text_position_x = resolution_width_ / 2.0 - (24 * 5);
    float text_position_y = resolution_height_ / 4.0;

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
    float tile_ratio =
        static_cast<float>(scale.y) / static_cast<float>(scale.x);

    logger_->Info("spriteWidth = %d, spriteHeight = %d, ratio = %f", scale.x,
                  scale.y, tile_ratio);

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

  void DisplayGame() {
    window_.clear(sf::Color::Green);
    window_.draw(*text_);
    window_.draw(*red_dragon_sprite_);
    window_.display();
  }

  std::string GetSpritePathFromTile(MahjongTile& tile) {
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
      auto* tile_numerical = dynamic_cast<MahjongTileNumerical*>(&tile);
      auto tile_num = tile_numerical->GetValue();
      tile_name.append("Characters")
          .append(std::to_string(tile_num))
          .append(".png");
    } else if (tile_group_name == "Bamboo") {
      auto* tile_numerical = dynamic_cast<MahjongTileNumerical*>(&tile);
      auto tile_num = tile_numerical->GetValue();
      tile_name.append("Bamboo")
          .append(std::to_string(tile_num))
          .append(".png");
    } else if (tile_group_name == "Dot") {
      auto* tile_numerical = dynamic_cast<MahjongTileNumerical*>(&tile);
      auto tile_num = tile_numerical->GetValue();
      tile_name.append("Circles")
          .append(std::to_string(tile_num))
          .append(".png");
    } else {
      throw std::runtime_error("Oh boy");
    }

    return tile_name;
  }

  void DisplayTiles(std::vector<std::string>& tile_paths, int y, int x) {
    window_.clear(sf::Color::Green);

    float x_offset = x - (tile_real_width_ * 14);
    float y_offset = y;

    logger_->Info("x_offset = %f, y_offset = %f", x_offset, y_offset);
    logger_->Info("tileScale = %f", tile_scale_);
    logger_->Info("_tileRealWidth= %d", tile_real_width_);

    for (const auto& tile_path : tile_paths) {
      sf::Texture tile_texture(tile_path);
      sf::Sprite tile_sprite(tile_texture);
      tile_sprite.setScale({tile_scale_, tile_scale_});
      tile_sprite.setPosition({x_offset, y_offset});
      window_.draw(tile_sprite);
      x_offset += tile_real_width_ * 2;
    }

    window_.draw(*text_);
    window_.display();
  }

  void DisplayPlayerTiles(Player& player) {
    MahjongHand* player_hand = player.GetHand();
    MahjongHand hand = *player_hand;

    int x_offset = resolution_width_ / 2;
    int y_offset = resolution_height_ * 3 / 4;

    auto hand_copy = hand;

    std::vector<std::string> tile_paths;
    for (const auto& tile_ptr : hand_copy) {
      auto tile_path = GetSpritePathFromTile(*tile_ptr);
      tile_paths.push_back(tile_path);
    }

    DisplayTiles(tile_paths, y_offset, x_offset);
  }

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

 private:
  const std::string kToolsPath_;
  const std::vector<int> kSoundtrackDurationSecs_ = {240, 220, 120};
  const std::vector<std::string> kSoundtracks_;

  Logger* logger_;
  std::vector<sf::VideoMode> video_modes_;
  uint16_t max_height_ = 0;
  uint16_t max_width_ = 0;
  uint16_t resolution_width_ = 0;
  uint16_t resolution_height_ = 0;
  float ratio_ = 0;
  sf::RenderWindow window_;
  sf::Font* font_ = nullptr;
  sf::Text* text_ = nullptr;
  sf::Texture* red_dragon_texture_ = nullptr;
  sf::Sprite* red_dragon_sprite_ = nullptr;
  sf::Music* music_ = nullptr;
  sf::Clock sound_clock_;
  int current_soundtrack_num_ = 2;

  int tile_height_ = 0;
  float tile_scale_ = 0;
  int tile_real_width_ = 0;
};
