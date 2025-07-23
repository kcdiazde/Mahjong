#include "player.h"
#include "tiles.h"
#include <cassert>
#include <memory>

// Helper to create a numbered tile for testing
MahjongTilePtr CreateTile(const TileGroup& group, int number) {
    return std::make_shared<MahjongTileNumerical>(group, number);
}

// Helper to create a named tile (Wind/Dragon) for testing
MahjongTilePtr CreateTile(const TileGroup& group, const std::string& name) {
    return std::make_shared<MahjongTile>(group, name);
}

void TestDealRegularTile() {
    Player player("Test");
    assert(player.GetNumTotalTiles() == 0);

    player.DealTile(CreateTile(kBamboo, 5));
    assert(player.GetNumTilesSetAndHand() == 1);
    assert(player.GetNumFlowers() == 0);

    std::cout << "TestDealRegularTile: PASSED" << std::endl;
}

void TestDealFlowerTile() {
    Player player("Test");
    assert(player.GetNumTotalTiles() == 0);

    player.DealTile(CreateTile(kFlower, "Spring"));
    assert(player.GetNumTilesSetAndHand() == 0);
    assert(player.GetNumFlowers() == 1);

    std::cout << "TestDealFlowerTile: PASSED" << std::endl;
}

void TestHandSorting() {
    Player player("Test");
    MahjongSet set;
    set.Shuffle();

    constexpr auto kTilesToTest = 14;

    // Deal tiles in an unsorted order
    for (int tiles_dealt = 0; tiles_dealt < kTilesToTest;) {
        auto new_tile = set.TakeTile();
        // Flowers don't increment hand size, so don't count them
        if (new_tile->IsFlower()) {
            player.DealTile(new_tile);
            continue; 
        }
        player.DealTile(new_tile);
        tiles_dealt++;

        const auto hand = *(player.GetHand());
        assert(hand.size() == tiles_dealt);

        TileId prev_tile_id = 0;
        for (const auto& tile : hand) {
            const auto id = tile->GetId();
            assert(id >= prev_tile_id);
            prev_tile_id = id;
        }
    }

    std::cout << "TestHandSorting: PASSED" << std::endl;
}

int main() {
    TestDealRegularTile();
    TestDealFlowerTile();
    TestHandSorting();

  return 0;
}
