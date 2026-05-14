#pragma once
#include "Item.h"
#include <vector>

class GildedRose {
public:
    std::vector<Item>& items;

    GildedRose(std::vector<Item>& items);
    void updateQuality();

private:
    // Step 2-1: 매직 스트링 상수 추출
    static constexpr const char* AGED_BRIE    = "Aged Brie";
    static constexpr const char* BACKSTAGE_PASS =
        "Backstage passes to a TAFKAL80ETC concert";
    static constexpr const char* SULFURAS     =
        "Sulfuras, Hand of Ragnaros";

    // Step 2-1: 품질 경계값 상수 추출
    static constexpr int MAX_QUALITY = 50;
    static constexpr int MIN_QUALITY = 0;
};