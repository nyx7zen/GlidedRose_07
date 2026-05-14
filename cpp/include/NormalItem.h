#pragma once
#include "GildedRoseItem.h"

class NormalItem : public GildedRoseItem {
public:
    using GildedRoseItem::GildedRoseItem;
    void updateQuality() override {
        if (item_.quality > MIN_QUALITY) item_.quality--;
        if (item_.sellIn < 1 && item_.quality > MIN_QUALITY) item_.quality--;
    }
};