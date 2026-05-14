#pragma once
#include "Item.h"

class AgedBrieItem {
    Item& item_;
public:
    explicit AgedBrieItem(Item& item) : item_(item) {}
    void updateQuality() {
        static constexpr int MAX = 50;
        if (item_.quality < MAX) item_.quality++;
        if (item_.sellIn < 1)
            if (item_.quality < MAX) item_.quality++;
    }
};