
#ifndef PLAYER_H
#define PLAYER_H

#include "mahjong_common.h"
#include "tiles.h"
#include <string>

typedef std::vector<MahjongTile *> MahjongHand;

class Player {

  protected:
    std::string _name;
    MahjongHand _hand;
    MahjongHand _flowers;
    uint16_t _points;

  public:
    explicit Player(){};
    explicit Player(std::string name) : _name(name), _points(0){};
    virtual ~Player() = default;

    void print_hand();
    void deal_tile(MahjongTile *);
    uint8_t get_num_total_tiles() { return _hand.size() + _flowers.size(); }
    uint8_t get_num_tiles_in_hand() { return _hand.size(); }
    std::string get_name() { return _name; }
    void sort_hand();
    virtual std::vector<MahjongTile*> get_tiles_to_pass() = 0;
};

class User : public Player {
  protected:

  public:
    explicit User() {};
    explicit User(std::string name) : Player(name) {};
    ~User() = default;

    std::vector<MahjongTile*> get_tiles_to_pass() {
      return _hand;
    }

};

class Bot : public Player {
  protected:

  public:
    explicit Bot() {};
    explicit Bot(std::string name) : Player(name) {};
    ~Bot() = default;
    
    std::vector<MahjongTile*> get_tiles_to_pass() {
      return _hand;
    }
};

#endif