#include "GildedRose.h"
#include "AgedBrieItem.h"
#include "BackstagePassItem.h"
#include "SulfurasItem.h"
#include "NormalItem.h"

GildedRose::GildedRose(std::vector<Item>& items) : items(items) {}

void GildedRose::updateQuality() {
    for (auto& item : items) {
        if      (item.name == AGED_BRIE)      AgedBrieItem{item}.updateQuality();
        else if (item.name == BACKSTAGE_PASS) BackstagePassItem{item}.updateQuality();
        else if (item.name == SULFURAS)       SulfurasItem{item}.updateQuality();
        else                                  NormalItem{item}.updateQuality();
        updateSellIn(item);
    }
}

void GildedRose::updateSellIn(Item& item) {
    if (item.name != SULFURAS) item.sellIn--;
}