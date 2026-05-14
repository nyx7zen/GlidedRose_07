#pragma once
#include "GildedRoseItem.h"

class FoodBeverageItem : public GildedRoseItem {
public:
    explicit FoodBeverageItem(Item& item) : GildedRoseItem(item) {}
    void updateQuality() override {
        if (item_.quality > MIN_QUALITY) item_.quality--;
        if (item_.quality > MIN_QUALITY) item_.quality--;
        if (item_.sellIn < 1) {
            if (item_.quality > MIN_QUALITY) item_.quality--;
            if (item_.quality > MIN_QUALITY) item_.quality--;
        }
    }
};