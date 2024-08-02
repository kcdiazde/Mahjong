
#ifndef PLAYER_H
#define PLAYER_H

#include "mahjong_common.h"
#include "tiles.h"
#include <string>
#include <list>
#include <iostream>

typedef std::vector<MahjongTile *> MahjongHand;
typedef std::vector<MahjongTile *>::iterator HandIterator;

class Player {

  protected:
    std::string _name;
    MahjongHand _sets[4];
    MahjongHand _hand;
    MahjongHand _flowers;
    MahjongHand _tiles_to_pass;
    uint16_t _points;

  public:
    explicit Player(){};
    explicit Player(std::string name) : _name(name), _points(0){};
    virtual ~Player() = default;

    void print_hand();
    void print_tiles_to_pass();
    void deal_tile(MahjongTile *);
    uint8_t get_num_total_tiles() { return _hand.size() + _flowers.size(); }
    uint8_t get_num_tiles_in_hand() { return _hand.size(); }
    std::string get_name() { return _name; }
    void sort_hand();
    bool found_tile_with_id_and_group(TileId tile_id, std::string group);
    bool tile_is_pung(MahjongTile tile);
    bool tile_is_partial_pung(MahjongTile tile);
    bool tile_is_chow(MahjongTile tile);
    bool tile_is_partial_chow(MahjongTile tile);
    virtual void pass_3_tiles(Player * receiver) = 0;
    virtual void receive_tile(MahjongTile * tile) = 0;
};

class User : public Player {
  protected:
  public:
    explicit User(){};
    explicit User(std::string name) : Player(name){};
    ~User() = default;

    void pass_3_tiles(Player * receiver);
    void receive_tile(MahjongTile * tile);
};

// TODO: Keep count of tiles that have come out
class Bot : public Player {
  protected:

  public:
    explicit Bot(){};
    explicit Bot(std::string name) : Player(name){};
    ~Bot() = default;

    // MahjongHand get_tiles_to_pass();
    void preprocess_hand();
    void move_tiles_to_pass();
    bool move_tile_between_hands(MahjongHand *src_hand, MahjongHand *dst_hand, TileId tile_id);
    void print_pungs();
    void print_chows();


    void pass_3_tiles(Player * receiver);
    void receive_tile(MahjongTile * tile);
};

#endif