#pragma once
#include "Item.h"

class GildedRoseItem {
protected:
    Item& item_;
    static constexpr int MAX_QUALITY = 50;
    static constexpr int MIN_QUALITY = 0;
public:
    explicit GildedRoseItem(Item& item) : item_(item) {}
    virtual void updateQuality() = 0;
    virtual ~GildedRoseItem() = default;
};