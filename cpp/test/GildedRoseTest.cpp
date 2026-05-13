#include <gtest/gtest.h>
#include "GildedRose.h"

// --------------------------------------------------------
// Foo — 기본 테스트 (name 불변 확인)
// --------------------------------------------------------

TEST(GildedRoseTest, Foo) {
    std::vector<Item> items = { Item("foo", 0, 0) };
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ("foo", app.items[0].name);
}

// --------------------------------------------------------
// 일반 아이템 (noname)
// --------------------------------------------------------

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

// --------------------------------------------------------
// Sulfuras
// --------------------------------------------------------

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

// 입력: Sulfuras, -1, 5  /  예상: sellIn=-1, quality=5
// 전설 아이템: 기한 마감 후에도 변화 없음
TEST(GildedRoseTest, sulfuras_sellin_m1_quality_5) {
    std::vector<Item> items = {
        Item("Sulfuras, Hand of Ragnaros", -1, 5)
    };
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(-1, app.items[0].sellIn);
    EXPECT_EQ(5,  app.items[0].quality);
}

// --------------------------------------------------------
// Aged Brie
// --------------------------------------------------------

// 입력: Aged Brie, 0, 0  /  예상: sellIn=-1, quality=2
// 기한 지나면 품질 +2
TEST(GildedRoseTest, agedBrie_sellin_0_quality_0) {
    std::vector<Item> items = { Item("Aged Brie", 0, 0) };
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(-1, app.items[0].sellIn);
    EXPECT_EQ(2,  app.items[0].quality);
}

// 입력: Aged Brie, 0, 50  /  예상: sellIn=-1, quality=50
// 품질 상한 50 초과 불가
TEST(GildedRoseTest, agedBrie_sellin_0_quality_50) {
    std::vector<Item> items = { Item("Aged Brie", 0, 50) };
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(-1, app.items[0].sellIn);
    EXPECT_EQ(50, app.items[0].quality);
}