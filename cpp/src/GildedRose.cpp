#include "GildedRose.h"

#include <algorithm>

GildedRose::GildedRose(std::vector<Item>& items) : items(items) {}

namespace {
constexpr const char* AgedBrie = "Aged Brie";
constexpr const char* BackstagePass = "Backstage passes to a TAFKAL80ETC concert";
constexpr const char* Sulfuras = "Sulfuras, Hand of Ragnaros";
constexpr const char* Conjured = "Conjured";

bool isConjured(const Item& item) {
    return item.name.rfind(Conjured, 0) == 0;
}

void increaseQuality(Item& item, int amount) {
    item.quality = std::min(50, item.quality + amount);
}

void decreaseQuality(Item& item, int amount) {
    item.quality = std::max(0, item.quality - amount);
}
}

void GildedRose::updateQuality() {
    for (Item& item : items) {
        if (item.name == Sulfuras) {
            continue;
        }

        if (item.name == AgedBrie) {
            increaseQuality(item, item.sellIn <= 0 ? 2 : 1);
            item.sellIn--;
            continue;
        }

        if (item.name == BackstagePass) {
            if (item.sellIn <= 0) {
                item.quality = 0;
            } else {
                increaseQuality(item, 1);
                if (item.sellIn <= 10) {
                    increaseQuality(item, 1);
                }
                if (item.sellIn <= 5) {
                    increaseQuality(item, 1);
                }
            }
            item.sellIn--;
            continue;
        }

        const int baseDecrease = isConjured(item) ? 2 : 1;
        const int decreaseAmount = item.sellIn <= 0 ? baseDecrease * 2 : baseDecrease;
        decreaseQuality(item, decreaseAmount);
        item.sellIn--;
    }
}
