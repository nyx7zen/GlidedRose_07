#pragma once
#include "GildedRoseItem.h"

class BackstagePassItem : public GildedRoseItem {
public:
    using GildedRoseItem::GildedRoseItem;
    void updateQuality() override {
        if (item_.quality < MAX_QUALITY) item_.quality++;
        if (item_.sellIn < 11 && item_.quality < MAX_QUALITY) item_.quality++;
        if (item_.sellIn < 6  && item_.quality < MAX_QUALITY) item_.quality++;
        if (item_.sellIn < 1) item_.quality = 0;
    }
};