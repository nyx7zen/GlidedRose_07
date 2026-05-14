#include "GildedRose.h"
#include "AgedBrieItem.h"
#include "BackstagePassItem.h"
#include "SulfurasItem.h"
#include "NormalItem.h"
#include "FoodBeverageItem.h"

GildedRose::GildedRose(std::vector<Item>& items) : items(items) {}

void GildedRose::updateQuality() {
    for (auto& item : items) {
        std::unique_ptr<GildedRoseItem> gi = createItem(item);
        gi->updateQuality();
        updateSellIn(item);
    }
}

std::unique_ptr<GildedRoseItem> GildedRose::createItem(Item& item) {
    if      (item.name == AGED_BRIE)      return std::make_unique<AgedBrieItem>(item);
    else if (item.name == BACKSTAGE_PASS) return std::make_unique<BackstagePassItem>(item);
    else if (item.name == SULFURAS)       return std::make_unique<SulfurasItem>(item);
    else if (item.name.find(FOOD_BEVERAGE) != std::string::npos)
                                          return std::make_unique<FoodBeverageItem>(item);
    else                                  return std::make_unique<NormalItem>(item);
}

void GildedRose::updateSellIn(Item& item) {
    if (item.name != SULFURAS) item.sellIn--;
}