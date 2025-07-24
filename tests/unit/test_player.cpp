#include "player.h"
#include "tiles.h"
#include <cassert>
#include <iostream>
#include <mahjong_common.h>
#include <memory>
#include <vector>

class PlayerTest {
  public:
    MahjongSet GlobalSet;

    // Helper to create a numbered tile for testing
    MahjongTilePtr CreateTile(const TileGroup &group, int number) {
        const auto &set = GlobalSet.GetTiles();

        for (const auto &p_tile : set) {
            auto *tile_numerical =
                dynamic_cast<MahjongTileNumerical *>(p_tile.get());
            if (tile_numerical) {
                if (tile_numerical->GetGroup().name == group.name) {
                    if (tile_numerical &&
                        tile_numerical->GetValue() == number) {
                        return p_tile;
                    }
                }
            }
        }

        throw std::runtime_error("Couldn't find tile :(");
        return nullptr;
    }

    // Helper to create a named tile (Wind/Dragon) for testing
    MahjongTilePtr CreateTile(const TileGroup &group, const std::string &name) {
        const auto &set = GlobalSet.GetTiles();

        for (const auto &p_tile : set) {
            bool same_group = p_tile->GetGroup().name == group.name;
            bool same_name = p_tile->GetName() == name;
            if (same_group && same_name) {
                return p_tile;
            }
        }

        throw std::runtime_error("Couldn't find tile :(");
        return nullptr;
    }

    // Helper to set up a player's hand with a specific set of tiles
    void SetupPlayerWithHand(Player &player,
                             const std::vector<MahjongTilePtr> &tiles) {
        for (const auto &tile : tiles) {
            player.DealTile(tile);
        }
    }

    // === TESTS ===

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
        MahjongSet test_set;
        test_set.Shuffle();

        constexpr auto kTilesToTest = 14;

        for (int tiles_dealt = 0; tiles_dealt < kTilesToTest;) {
            auto new_tile = test_set.TakeTile();
            if (new_tile->IsFlower()) {
                player.DealTile(new_tile);
                continue;
            }
            player.DealTile(new_tile);
            tiles_dealt++;

            const auto hand = player.GetHand();
            assert(hand.size() == tiles_dealt);

            TileId prev_tile_id = 0;
            for (const auto &tile : hand) {
                const auto id = tile->GetId();
                assert(id >= prev_tile_id);
                prev_tile_id = id;
            }
        }

        std::cout << "TestHandSorting: PASSED" << std::endl;
    }

    void TestIsPung() {
        Player player("Test Pung");
        SetupPlayerWithHand(player, {
                                        CreateTile(kDot, 3),
                                        CreateTile(kDot, 3),
                                        CreateTile(kDot, 3),
                                        CreateTile(kDot, 4),
                                        CreateTile(kDot, 4),
                                    });

        auto test_tile_is_pung = CreateTile(kDot, 3);
        auto test_tile_is_not_pung = CreateTile(kDot, 4);

        assert(player.IsTilePung(*test_tile_is_pung) == true);
        assert(player.IsTilePung(*test_tile_is_not_pung) == false);

        std::cout << "TestIsPung: PASSED" << std::endl;
    }

    void TestIsPartialPung() {
        Player player("Test Partial Pung");
        SetupPlayerWithHand(player, {
                                        CreateTile(kWind, "East"),
                                        CreateTile(kWind, "East"),
                                        CreateTile(kSymbol, 1),
                                    });

        auto test_tile_is_partial = CreateTile(kWind, "East");
        auto test_tile_is_not_partial = CreateTile(kSymbol, 1);

        assert(player.IsTilePartialPung(*test_tile_is_partial) == true);
        assert(player.IsTilePartialPung(*test_tile_is_not_partial) == false);

        std::cout << "TestIsPartialPung: PASSED" << std::endl;
    }

    void TestIsChow() {
        Player player("Test Chow");
        SetupPlayerWithHand(player, {
                                        CreateTile(kBamboo, 4),
                                        CreateTile(kBamboo, 5),
                                        CreateTile(kBamboo, 6),
                                        CreateTile(kDragon, "Red"),
                                    });

        auto chow_start = CreateTile(kBamboo, 4);
        auto chow_middle = CreateTile(kBamboo, 5);
        auto chow_end = CreateTile(kBamboo, 6);
        auto not_chow = CreateTile(kDragon, "Red");

        assert(player.IsTileChow(*chow_start) == true);
        assert(player.IsTileChow(*chow_middle) == true);
        assert(player.IsTileChow(*chow_end) == true);
        assert(player.IsTileChow(*not_chow) == false);

        std::cout << "TestIsChow: PASSED" << std::endl;
    }

    void TestPlayPungs() {
        Player player("Test Play Pung");
        SetupPlayerWithHand(player, {
                                        CreateTile(kSymbol, 8),
                                        CreateTile(kSymbol, 8),
                                        CreateTile(kSymbol, 8),
                                        CreateTile(kBamboo, 1),
                                        CreateTile(kDot, 9),
                                    });

        assert(player.GetNumTilesInHand() == 5);
        player.PlayPungs();

        // The pung of 8s should be moved to sets
        assert(player.GetNumTilesInHand() == 2);
        assert(player.GetNumTilesSetAndHand() == 5);

        std::cout << "TestPlayPungs: PASSED" << std::endl;
    }

    void TestPlayChows() {
        Player player("Test Play Chow");
        SetupPlayerWithHand(player, {
                                        CreateTile(kDot, 7),
                                        CreateTile(kDot, 8),
                                        CreateTile(kDot, 9),
                                        CreateTile(kWind, "South"),
                                        CreateTile(kDragon, "Green"),
                                    });

        assert(player.GetNumTilesInHand() == 5);
        player.PlayChows();

        // The chow of 7,8,9 should be moved to sets
        assert(player.GetNumTilesInHand() == 2);
        assert(player.GetNumTilesSetAndHand() == 5);

        std::cout << "TestPlayChows: PASSED" << std::endl;
    }

    void TestPassAndReceiveTiles() {
        Bot bot1("Bot1");
        Player player2("Player2");

        // Bots have a public method to select tiles to pass
        SetupPlayerWithHand(bot1, {
                                      CreateTile(kWind, "North"),   // Useless
                                      CreateTile(kDragon, "White"), // Useless
                                      CreateTile(kDot, 1),          // Useless
                                      CreateTile(kBamboo, 5),
                                      CreateTile(kBamboo, 6),
                                  });

        bot1.PreprocessHand(); // This will move the 3 "useless" tiles
        bot1.Pass3Tiles(&player2);

        assert(bot1.GetNumTilesInHand() == 2);
        assert(player2.tiles_received_.size() == 3);

        player2.MoveReceivedTilesToHand();
        assert(player2.tiles_received_.empty());
        assert(player2.GetNumTilesInHand() == 3);

        std::cout << "TestPassAndReceiveTiles: PASSED" << std::endl;
    }

    void TestGetTileToDiscard() {
        Player player("Test Discard");
        auto useless_tile = CreateTile(kWind, "West");

        SetupPlayerWithHand(player, {CreateTile(kDot, 2),
                                     CreateTile(kDot, 3), // Partial chow
                                     useless_tile});

        auto discarded_tile = player.GetTileToDiscard();

        // The strategy should discard the useless tile before breaking a
        // partial set
        assert(discarded_tile->GetId() == useless_tile->GetId());
        assert(player.GetNumTilesInHand() == 2);

        std::cout << "TestGetTileToDiscard: PASSED" << std::endl;
    }

    void TestHasWon() {
        Player player("Winner");
        player.DealTile(CreateTile(kBamboo, 1));
        assert(player.HasWon() == true);

        player.DealTile(CreateTile(kBamboo, 2));
        assert(player.HasWon() == false);

        std::cout << "TestHasWon: PASSED" << std::endl;
    }
};

int main() {
    PlayerTest test;
    test.TestDealRegularTile();
    test.TestDealFlowerTile();
    test.TestHandSorting();
    test.TestIsPung();
    test.TestIsPartialPung();
    test.TestIsChow();
    test.TestPlayPungs();
    test.TestPlayChows();
    test.TestPassAndReceiveTiles();
    test.TestGetTileToDiscard();
    test.TestHasWon();

    return 0;
}
