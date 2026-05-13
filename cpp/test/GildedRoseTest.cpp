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