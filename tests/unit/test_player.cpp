#include "player.h"
#include "tiles.h"
#include <cassert>

#include <memory>

// Helper to create a numbered tile for testing
MahjongTilePtr create_tile(TileGroup group, int number) {
    return std::make_shared<MahjongTileNumerical>(TileGroup(group), number);
}

// Helper to create a named tile (Wind/Dragon) for testing
MahjongTilePtr create_tile(TileGroup group, const std::string& name) {
    return std::make_shared<MahjongTile>(TileGroup(group), name);
}


void test_deal_regular_tile() {
    Player player("Test");
    assert(player.get_num_total_tiles() == 0);

    player.deal_tile(create_tile(BAMBOO, 5));
    assert(player.get_num_tiles_set_and_hand() == 1);
    assert(player.get_num_flowers() == 0);

    std::cout << "test_deal_flower_tile: PASSED" << std::endl;
}

void test_deal_flower_tile() {
    Player player("Test");
    assert(player.get_num_total_tiles() == 0);

    player.deal_tile(create_tile(FLOWER, "Spring"));
    assert(player.get_num_tiles_set_and_hand() == 0);
    assert(player.get_num_flowers() == 1);

    std::cout << "test_deal_flower_tile: PASSED" << std::endl;
}

void test_hand_sorting() {
    Player player("Test");
    MahjongSet set;
    set.shuffle();

    constexpr auto TILES_TO_TEST = 14;

    // Deal tiles in an unsorted order
    for (int test_num = 0; test_num < TILES_TO_TEST;) {
        auto newTile = set.take_tile();
        // Flowers don't increment hand size
        if (newTile->is_flower()) {
            break;
        }
        player.deal_tile(newTile);

        const auto hand = *(player.get_hand());
        std::cout << "Hand size: " << hand.size() << std::endl;
        assert(hand.size() == ++test_num);

        auto prev_tile_id = 0;
        for (const auto& tile : hand) {
            const auto id = tile->get_id();
            std::cout << "Curr id: " << static_cast<int>(id) << " prev id: " << prev_tile_id << std::endl;
            assert(id >= prev_tile_id);
            prev_tile_id = id;
        }
    }

    std::cout << "test_hand_sorting: PASSED" << std::endl;
}

int main() {

    test_deal_regular_tile();
    test_deal_flower_tile();
    test_hand_sorting();

  return 0;
}

