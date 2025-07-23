#include "tiles.h"
#include <cassert>

int TestSetCreation() {
    MahjongSet set;
    assert(set.GetNumTiles() == 144);

    return 0;
}

int TestShuffleTiles() {
    MahjongSet set;

    const auto initial_order = set.GetTiles();

    set.Shuffle();

    const auto shuffled_order = set.GetTiles();

    assert(initial_order.size() == shuffled_order.size());
    assert(initial_order != shuffled_order); // This has a small chance to fail
                                             // if shuffle results in same order

    return 0;
}

int main() {
    int results = 0;

    // Test tiles
    results += TestSetCreation();
    results += TestShuffleTiles();

    return results;
}
