
#ifndef PLAYER_H
#define PLAYER_H

#include "mahjong_common.h"
#include "tiles.h"
#include <string>

typedef std::vector<MahjongTile *> MahjongHand;

class Player {

  protected:
    std::string _name;
    MahjongHand _sets[4];
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
    virtual MahjongHand get_tiles_to_pass() = 0;
};

class User : public Player {
  protected:

  public:
    explicit User() {};
    explicit User(std::string name) : Player(name) {};
    ~User() = default;

    MahjongHand get_tiles_to_pass();
};

// TODO: Keep count of tiles that have come out
class Bot : public Player {
  protected:
    MahjongHand _concealed_sets;
    MahjongHand _usefull_tiles;
    MahjongHand _unwanted_tiles;
    

  public:
    explicit Bot() {};
    explicit Bot(std::string name) : Player(name) {};
    ~Bot() = default;

    MahjongHand get_tiles_to_pass();
    void preprocess_hand();
    void move_tiles_to_concealed(TileId id);
    void print_concealed();
};

#endif