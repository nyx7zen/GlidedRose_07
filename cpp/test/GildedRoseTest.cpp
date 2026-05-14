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

// --------------------------------------------------------
// Backstage passes
// --------------------------------------------------------

// 입력: Backstage, 15, 0  /  예상: sellIn=14, quality=1
// 10일 초과 시 품질 +1
TEST(GildedRoseTest, backstage_pass_sellin_15_quality_0) {
    std::vector<Item> items = {
        Item("Backstage passes to a TAFKAL80ETC concert", 15, 0)
    };
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(14, app.items[0].sellIn);
    EXPECT_EQ(1,  app.items[0].quality);
}

// 입력: Backstage, 0, 0  /  예상: sellIn=-1, quality=0
// 공연 지난 후 품질 0
TEST(GildedRoseTest, backstage_pass_sellin_0_quality_0) {
    std::vector<Item> items = {
        Item("Backstage passes to a TAFKAL80ETC concert", 0, 0)
    };
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(-1, app.items[0].sellIn);
    EXPECT_EQ(0,  app.items[0].quality);
}

// 입력: Backstage, 0, 51  /  예상: sellIn=-1, quality=0
// 품질 초과 후 공연 종료 시 0
TEST(GildedRoseTest, backstage_pass_sellin_0_quality_51) {
    std::vector<Item> items = {
        Item("Backstage passes to a TAFKAL80ETC concert", 0, 51)
    };
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(-1, app.items[0].sellIn);
    EXPECT_EQ(0,  app.items[0].quality);
}

// --------------------------------------------------------
// 빈 배열
// --------------------------------------------------------

// 아이템 없을 경우 정상 처리
TEST(GildedRoseTest, should_be_nothing_when_no_item) {
    std::vector<Item> items;
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(0u, items.size());
}

// --------------------------------------------------------
// Step 1 — 회귀 테스트 보강
// --------------------------------------------------------

// 일반 아이템: 매일 quality -1, sellIn -1
TEST(GildedRoseTest, NormalItemDegrades) {
    std::vector<Item> items = { Item("+5 Dexterity Vest", 10, 20) };
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(9,  items[0].sellIn);
    EXPECT_EQ(19, items[0].quality);
}

// Aged Brie: 오래될수록 quality 증가
TEST(GildedRoseTest, AgedBrieIncreasesQuality) {
    std::vector<Item> items = { Item("Aged Brie", 5, 10) };
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(11, items[0].quality);
}

// Backstage: sellIn 11일 → quality +1
TEST(GildedRoseTest, BackstagePass_SellIn11_QualityPlus1) {
    std::vector<Item> items = {
        Item("Backstage passes to a TAFKAL80ETC concert", 11, 20)
    };
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(21, items[0].quality);
}

// Backstage: sellIn 10일 → quality +2 (경계값)
TEST(GildedRoseTest, BackstagePass_SellIn10_QualityPlus2) {
    std::vector<Item> items = {
        Item("Backstage passes to a TAFKAL80ETC concert", 10, 20)
    };
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(22, items[0].quality);
}

// Backstage: sellIn 6일 → quality +2
TEST(GildedRoseTest, BackstagePass_SellIn6_QualityPlus2) {
    std::vector<Item> items = {
        Item("Backstage passes to a TAFKAL80ETC concert", 6, 20)
    };
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(22, items[0].quality);
}

// Backstage: sellIn 5일 → quality +3 (경계값)
TEST(GildedRoseTest, BackstagePass_SellIn5_QualityPlus3) {
    std::vector<Item> items = {
        Item("Backstage passes to a TAFKAL80ETC concert", 5, 20)
    };
    GildedRose app(items);
    app.updateQuality();
    EXPECT_EQ(23, items[0].quality);
}