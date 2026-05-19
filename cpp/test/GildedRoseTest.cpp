#include <gtest/gtest.h>
#include "GildedRose.h"

class GildedRoseTest : public ::testing::Test {
protected:
    static constexpr const char* BackstagePass =
        "Backstage passes to a TAFKAL80ETC concert";

    static Item updateSingleItem(const std::string& name, int sellIn, int quality) {
        std::vector<Item> items = { Item(name, sellIn, quality) };
        GildedRose app(items);

        app.updateQuality();

        return app.items[0];
    }
};

TEST_F(GildedRoseTest, NormalItemDecreasesQualityAndSellInBeforeSellDate) {
    // Given
    const std::string name = "Dexterity Vest";

    // When
    const Item item = updateSingleItem(name, 10, 20);

    // Then
    EXPECT_EQ(9, item.sellIn);
    EXPECT_EQ(19, item.quality);
}

TEST_F(GildedRoseTest, NormalItemDegradesTwiceAsFastAtSellInZero) {
    // Given
    const std::string name = "Dexterity Vest";

    // When
    const Item item = updateSingleItem(name, 0, 20);

    // Then
    EXPECT_EQ(-1, item.sellIn);
    EXPECT_EQ(18, item.quality);
}

TEST_F(GildedRoseTest, NormalItemDegradesTwiceAsFastAfterSellDate) {
    // Given
    const std::string name = "Dexterity Vest";

    // When
    const Item item = updateSingleItem(name, -1, 20);

    // Then
    EXPECT_EQ(-2, item.sellIn);
    EXPECT_EQ(18, item.quality);
}

TEST_F(GildedRoseTest, NormalItemQualityDoesNotGoBelowZero) {
    // Given
    const std::string name = "Dexterity Vest";

    // When
    const Item item = updateSingleItem(name, 10, 0);

    // Then
    EXPECT_EQ(9, item.sellIn);
    EXPECT_EQ(0, item.quality);
}

TEST_F(GildedRoseTest, NormalItemQualityDoesNotGoBelowZeroAfterSellDate) {
    // Given
    const std::string name = "Dexterity Vest";

    // When
    const Item item = updateSingleItem(name, -1, 1);

    // Then
    EXPECT_EQ(-2, item.sellIn);
    EXPECT_EQ(0, item.quality);
}

TEST_F(GildedRoseTest, AgedBrieIncreasesQualityAndDecreasesSellInBeforeSellDate) {
    // Given
    const std::string name = "Aged Brie";

    // When
    const Item item = updateSingleItem(name, 10, 20);

    // Then
    EXPECT_EQ(9, item.sellIn);
    EXPECT_EQ(21, item.quality);
}

TEST_F(GildedRoseTest, AgedBrieIncreasesQualityTwiceAsFastAtSellInZero) {
    // Given
    const std::string name = "Aged Brie";

    // When
    const Item item = updateSingleItem(name, 0, 20);

    // Then
    EXPECT_EQ(-1, item.sellIn);
    EXPECT_EQ(22, item.quality);
}

TEST_F(GildedRoseTest, AgedBrieIncreasesQualityTwiceAsFastAfterSellDate) {
    // Given
    const std::string name = "Aged Brie";

    // When
    const Item item = updateSingleItem(name, -1, 20);

    // Then
    EXPECT_EQ(-2, item.sellIn);
    EXPECT_EQ(22, item.quality);
}

TEST_F(GildedRoseTest, AgedBrieQualityDoesNotExceedFifty) {
    // Given
    const std::string name = "Aged Brie";

    // When
    const Item item = updateSingleItem(name, 10, 50);

    // Then
    EXPECT_EQ(9, item.sellIn);
    EXPECT_EQ(50, item.quality);
}

TEST_F(GildedRoseTest, AgedBrieQualityDoesNotExceedFiftyAfterSellDate) {
    // Given
    const std::string name = "Aged Brie";

    // When
    const Item item = updateSingleItem(name, -1, 49);

    // Then
    EXPECT_EQ(-2, item.sellIn);
    EXPECT_EQ(50, item.quality);
}

TEST_F(GildedRoseTest, BackstagePassIncreasesQualityByOneMoreThanTenDaysBeforeConcert) {
    // Given
    const std::string name = BackstagePass;

    // When
    const Item item = updateSingleItem(name, 11, 20);

    // Then
    EXPECT_EQ(10, item.sellIn);
    EXPECT_EQ(21, item.quality);
}

TEST_F(GildedRoseTest, BackstagePassIncreasesQualityByTwoAtTenDaysBeforeConcert) {
    // Given
    const std::string name = BackstagePass;

    // When
    const Item item = updateSingleItem(name, 10, 20);

    // Then
    EXPECT_EQ(9, item.sellIn);
    EXPECT_EQ(22, item.quality);
}

TEST_F(GildedRoseTest, BackstagePassIncreasesQualityByThreeAtFiveDaysBeforeConcert) {
    // Given
    const std::string name = BackstagePass;

    // When
    const Item item = updateSingleItem(name, 5, 20);

    // Then
    EXPECT_EQ(4, item.sellIn);
    EXPECT_EQ(23, item.quality);
}

TEST_F(GildedRoseTest, BackstagePassQualityDoesNotExceedFifty) {
    // Given
    const std::string name = BackstagePass;

    // When
    const Item item = updateSingleItem(name, 5, 49);

    // Then
    EXPECT_EQ(4, item.sellIn);
    EXPECT_EQ(50, item.quality);
}

TEST_F(GildedRoseTest, BackstagePassQualityDropsToZeroAtSellInZero) {
    // Given
    const std::string name = BackstagePass;

    // When
    const Item item = updateSingleItem(name, 0, 20);

    // Then
    EXPECT_EQ(-1, item.sellIn);
    EXPECT_EQ(0, item.quality);
}

TEST_F(GildedRoseTest, BackstagePassQualityStaysZeroAfterConcert) {
    // Given
    const std::string name = BackstagePass;

    // When
    const Item item = updateSingleItem(name, -1, 20);

    // Then
    EXPECT_EQ(-2, item.sellIn);
    EXPECT_EQ(0, item.quality);
}

TEST_F(GildedRoseTest, SulfurasDoesNotDecreaseSellIn) {
    // Given
    const std::string name = "Sulfuras, Hand of Ragnaros";

    // When
    const Item item = updateSingleItem(name, 10, 80);

    // Then
    EXPECT_EQ(10, item.sellIn);
    EXPECT_EQ(80, item.quality);
}

TEST_F(GildedRoseTest, SulfurasDoesNotDecreaseQuality) {
    // Given
    const std::string name = "Sulfuras, Hand of Ragnaros";

    // When
    const Item item = updateSingleItem(name, 5, 80);

    // Then
    EXPECT_EQ(5, item.sellIn);
    EXPECT_EQ(80, item.quality);
}

TEST_F(GildedRoseTest, SulfurasKeepsQualityEightyAboveNormalMaximum) {
    // Given
    const std::string name = "Sulfuras, Hand of Ragnaros";

    // When
    const Item item = updateSingleItem(name, 0, 80);

    // Then
    EXPECT_EQ(0, item.sellIn);
    EXPECT_EQ(80, item.quality);
}

TEST_F(GildedRoseTest, SulfurasRemainsUnchangedAfterSellDate) {
    // Given
    const std::string name = "Sulfuras, Hand of Ragnaros";

    // When
    const Item item = updateSingleItem(name, -1, 80);

    // Then
    EXPECT_EQ(-1, item.sellIn);
    EXPECT_EQ(80, item.quality);
}

TEST_F(GildedRoseTest, SulfurasQualityDoesNotChangeAtZero) {
    // Given
    const std::string name = "Sulfuras, Hand of Ragnaros";

    // When
    const Item item = updateSingleItem(name, 10, 0);

    // Then
    EXPECT_EQ(10, item.sellIn);
    EXPECT_EQ(0, item.quality);
}

TEST_F(GildedRoseTest, ConjuredItemDecreasesQualityTwiceAsFastBeforeSellDate) {
    // Given
    const std::string name = "Conjured Mana Cake";

    // When
    const Item item = updateSingleItem(name, 10, 20);

    // Then
    EXPECT_EQ(9, item.sellIn);
    EXPECT_EQ(18, item.quality);
}

TEST_F(GildedRoseTest, ConjuredItemDegradesFourPointsAtSellInZero) {
    // Given
    const std::string name = "Conjured Mana Cake";

    // When
    const Item item = updateSingleItem(name, 0, 20);

    // Then
    EXPECT_EQ(-1, item.sellIn);
    EXPECT_EQ(16, item.quality);
}

TEST_F(GildedRoseTest, ConjuredItemDegradesFourPointsAfterSellDate) {
    // Given
    const std::string name = "Conjured Mana Cake";

    // When
    const Item item = updateSingleItem(name, -1, 20);

    // Then
    EXPECT_EQ(-2, item.sellIn);
    EXPECT_EQ(16, item.quality);
}

TEST_F(GildedRoseTest, ConjuredItemQualityDoesNotGoBelowZero) {
    // Given
    const std::string name = "Conjured Mana Cake";

    // When
    const Item item = updateSingleItem(name, 10, 0);

    // Then
    EXPECT_EQ(9, item.sellIn);
    EXPECT_EQ(0, item.quality);
}

TEST_F(GildedRoseTest, ConjuredItemQualityDoesNotGoBelowZeroAfterSellDate) {
    // Given
    const std::string name = "Conjured Mana Cake";

    // When
    const Item item = updateSingleItem(name, -1, 3);

    // Then
    EXPECT_EQ(-2, item.sellIn);
    EXPECT_EQ(0, item.quality);
}
