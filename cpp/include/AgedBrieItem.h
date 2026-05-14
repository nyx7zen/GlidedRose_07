#pragma once
#include "GildedRoseItem.h"

class AgedBrieItem : public GildedRoseItem {
public:
    using GildedRoseItem::GildedRoseItem;
    void updateQuality() override {
        if (item_.quality < MAX_QUALITY) item_.quality++;
        if (item_.sellIn < 1)
            if (item_.quality < MAX_QUALITY) item_.quality++;
    }
};