#include "GildedRose.h"

GildedRose::GildedRose(std::vector<Item>& items) : items(items) {}

void GildedRose::updateQuality() {
    for (size_t i = 0; i < items.size(); i++) {
        Item& item = items[i];

        // Step 3-1: 긍정 조건으로 전환 (Invert if)
        // Step 3-2: else if 병합
        if (item.name == AGED_BRIE || item.name == BACKSTAGE_PASS) {
            if (item.quality < MAX_QUALITY) {
                item.quality = item.quality + 1;
            }

            if (item.name == BACKSTAGE_PASS) {
                if (item.sellIn < 11) {
                    if (item.quality < MAX_QUALITY) {
                        item.quality = item.quality + 1;
                    }
                }
                if (item.sellIn < 6) {
                    if (item.quality < MAX_QUALITY) {
                        item.quality = item.quality + 1;
                    }
                }
            }
        } else if (item.name == SULFURAS) {
            // 아무것도 안 함
        } else {
            if (item.quality > MIN_QUALITY) {
                item.quality = item.quality - 1;
            }
        }

        if (item.name != SULFURAS) {
            item.sellIn = item.sellIn - 1;
        }

        // Step 3-3: sellIn 전/후 중복 조건 합치기
        // sellIn < 0 → sellIn < 1 로 경계 조정
        if (item.sellIn < 0) {
            if (item.name == AGED_BRIE) {
                if (item.quality < MAX_QUALITY) {
                    item.quality = item.quality + 1;
                }
            } else if (item.name == BACKSTAGE_PASS) {
                item.quality = 0;
            } else if (item.name != SULFURAS) {
                if (item.quality > MIN_QUALITY) {
                    item.quality = item.quality - 1;
                }
            }
        }
    }
}