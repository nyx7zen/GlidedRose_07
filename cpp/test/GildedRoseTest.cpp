#include <gtest/gtest.h>
#include "GildedRose.h"

TEST(GildedRoseTest, Foo) {
    std::vector<Item> items = { Item("foo", 0, 0) };
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ("foo", app.items[0].name);
}

// 입력: noname, 0, 0  /  예상: sellIn=-1, quality=0
// 일반 아이템: 품질이 이미 0이라 감소 안 됨
TEST(GildedRoseTest, noname_sellin_0_quality_0) {
    std::vector<Item> items = { Item("noname", 0, 0) };
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(-1, app.items[0].sellIn);
    EXPECT_EQ(0,  app.items[0].quality);
}

// 입력: noname, 0, 5  /  예상: sellIn=-1, quality=3
// 일반 아이템: 유통기한 지나면 품질 -2
TEST(GildedRoseTest, noname_sellin_0_quality_5) {
    std::vector<Item> items = { Item("noname", 0, 5) };
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(-1, app.items[0].sellIn);
    EXPECT_EQ(3,  app.items[0].quality);
}

// 입력: Sulfuras, 0, 5  /  예상: sellIn=0, quality=5
// 전설 아이템: 아무것도 변화 없음
TEST(GildedRoseTest, sulfuras_sellin_0_quality_5) {
    std::vector<Item> items = {
        Item("Sulfuras, Hand of Ragnaros", 0, 5)
    };
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(0, app.items[0].sellIn);
    EXPECT_EQ(5, app.items[0].quality);
}
