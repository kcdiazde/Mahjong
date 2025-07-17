#include "tiles.h"
#include "player.h"
#include <cassert>

int test_set_creation() {
    MahjongSet set;
    assert(set.get_num_tiles() == 144);
    
    return 0;
}

int test_shuffle_tiles() {
    MahjongSet set;
    
    const auto initial_order = set.get_tiles();

    set.shuffle();

    const auto shuffled_order = set.get_tiles();

    assert(initial_order.size() == shuffled_order.size());
    assert(initial_order != shuffled_order);

    return 0;
}



int main() {
    int results = 0;

    // Test tiles
    results += test_set_creation();
    results += test_shuffle_tiles();

  return results;
}

