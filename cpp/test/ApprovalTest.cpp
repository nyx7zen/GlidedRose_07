#include <gtest/gtest.h>
#include "ApprovalTests.hpp"
#include <sstream>
#include <vector>
#include "GildedRose.h"
#include "Item.h"

class ApprovalTest : public ::testing::Test {
protected:
    std::string printTextFixture(int days) {
        std::stringstream result;
        result << "OMGHAI!\n";

        std::vector<Item> items = {
            Item("+5 Dexterity Vest",                              10, 20),
            Item("Aged Brie",                                       2,  0),
            Item("Elixir of the Mongoose",                          5,  7),
            Item("Sulfuras, Hand of Ragnaros",                      0, 80),
            Item("Sulfuras, Hand of Ragnaros",                     -1, 80),
            Item("Backstage passes to a TAFKAL80ETC concert",      15, 20),
            Item("Backstage passes to a TAFKAL80ETC concert",      10, 49),
            Item("Backstage passes to a TAFKAL80ETC concert",       5, 49),
            Item("Conjured Mana Cake",                              3,  6),
        };

        GildedRose app(items);

        for (int i = 0; i < days; ++i) {
            result << "-------- day " << i << " --------\n";
            result << "name, sellIn, quality\n";
            for (const auto& item : items)
                result << item.toString() << "\n";
            result << "\n";
            app.updateQuality();
        }

        return result.str();
    }
};

TEST_F(ApprovalTest, ThirtyDays) {
    ApprovalTests::Approvals::verify(printTextFixture(30));
}