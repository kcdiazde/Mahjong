
#ifndef PLAYER_H
#define PLAYER_H

#include "mahjong_common.h"
#include "tiles.h"
#include "logger.h"
#include <string>
#include <list>
#include <iostream>

typedef std::vector<MahjongTilePtr> MahjongHand;
typedef std::vector<MahjongTilePtr>::iterator HandIterator;

class Player {

  protected:
    std::string _name;
    MahjongHand _sets;
    MahjongHand _hand;
    MahjongHand _flowers;
    MahjongHand _tiles_to_pass;
    uint16_t _points;
    MahjongTilePtr _last_discard;
    Logger* _logger;

    static constexpr auto CLASSNAME = "Player";

  public:
    MahjongHand _tiles_received;
    explicit Player(){};
    explicit Player(std::string name) : _name(name), _points(0), _last_discard(nullptr){
        _logger = &Logger::instance(); 
    };
    virtual ~Player() = default;

    void print_hand();
    void print_sets();
    void print_tiles_to_pass();
    void print_tiles_received();
    void deal_tile(MahjongTilePtr);
    uint8_t get_num_total_tiles() { return _hand.size() + _flowers.size(); }
    uint8_t get_num_tiles_in_hand() { return _hand.size(); }
    uint8_t get_num_tiles_set_and_hand() { return _hand.size() + _sets.size(); }
    std::string get_name() { return _name; }
    MahjongHand * get_hand() {return &_hand;}
    void sort_hand();
    bool found_tile_with_id_and_group(TileId tile_id, std::string group);
    bool move_tile_between_hands(MahjongHand& src_hand, MahjongHand& dst_hand, TileId tile_id);
    void remove_tile(TileId tile_id);
    bool tile_is_pung(MahjongTile& tile);
    bool tile_is_partial_pung(MahjongTile& tile);
    bool tile_is_chow(MahjongTile& tile);
    bool tile_is_partial_chow(MahjongTile& tile);
    void pass_3_tiles(Player * receiver);
    void move_received_tiles_to_hand();
    void play();
    void play_pungs();
    void play_chows();
    MahjongTilePtr get_tile_to_discard();
    bool wants_discard_tile(MahjongTilePtr tile);
    bool has_won();

};

class User : public Player {
  protected:
  public:
    explicit User(){};
    explicit User(std::string name) : Player(name){};
    ~User() = default;
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
    void print_pungs();
    void print_chows();

};

#endif
