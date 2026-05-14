#pragma once
#include "Item.h"
#include "GildedRoseItem.h"
#include <vector>
#include <memory>

class GildedRose {
public:
    std::vector<Item>& items;

    GildedRose(std::vector<Item>& items);
    void updateQuality();

private:
    static constexpr const char* AGED_BRIE      = "Aged Brie";
    static constexpr const char* BACKSTAGE_PASS =
        "Backstage passes to a TAFKAL80ETC concert";
    static constexpr const char* SULFURAS       =
        "Sulfuras, Hand of Ragnaros";
    static constexpr const char* FOOD_BEVERAGE  = "[F&B]";   // 추가

    std::unique_ptr<GildedRoseItem> createItem(Item& item);
    void updateSellIn(Item& item);
};