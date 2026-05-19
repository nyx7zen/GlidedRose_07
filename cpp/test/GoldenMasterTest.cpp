#include <gtest/gtest.h>

#include "GildedRose.h"

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#ifndef GOLDEN_MASTER_EXPECTED_DIR
#define GOLDEN_MASTER_EXPECTED_DIR "."
#endif

namespace {
constexpr const char* BackstagePass =
    "Backstage passes to a TAFKAL80ETC concert";

std::vector<Item> createTexttestFixtureItems() {
    return {
        Item("+5 Dexterity Vest", 10, 20),
        Item("Aged Brie", 2, 0),
        Item("Elixir of the Mongoose", 5, 7),
        Item("Sulfuras, Hand of Ragnaros", 0, 80),
        Item("Sulfuras, Hand of Ragnaros", -1, 80),
        Item(BackstagePass, 15, 20),
        Item(BackstagePass, 10, 49),
        Item(BackstagePass, 5, 49),
        Item("Conjured Mana Cake", 3, 6),
    };
}

std::string renderTexttestFixtureOutput(int days) {
    std::vector<Item> items = createTexttestFixtureItems();
    GildedRose app(items);

    std::ostringstream output;
    output << "OMGHAI!\n";

    for (int day = 0; day <= days; ++day) {
        output << "-------- day " << day << " --------\n";
        output << "name, sellIn, quality\n";
        for (const Item& item : items) {
            output << item.toString() << "\n";
        }
        output << "\n";

        app.updateQuality();
    }

    return output.str();
}

std::string readFile(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open file: " + path.string());
    }

    return std::string(std::istreambuf_iterator<char>(file),
                       std::istreambuf_iterator<char>());
}

std::string normalizeLineEndings(std::string value) {
    std::string normalized;
    normalized.reserve(value.size());

    for (std::size_t index = 0; index < value.size(); ++index) {
        if (value[index] == '\r') {
            if (index + 1 < value.size() && value[index + 1] == '\n') {
                continue;
            }
            normalized.push_back('\n');
            continue;
        }

        normalized.push_back(value[index]);
    }

    return normalized;
}

void writeFile(const std::filesystem::path& path, const std::string& content) {
    std::ofstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not write file: " + path.string());
    }

    file << content;
}
}

TEST(GoldenMasterTest, TexttestFixtureOutputMatchesThirtyDayBaseline) {
    const std::filesystem::path expectedPath =
        std::filesystem::path(GOLDEN_MASTER_EXPECTED_DIR) /
        "TexttestFixture.ThirtyDays.expected.txt";
    const std::filesystem::path receivedPath =
        expectedPath.parent_path() / "TexttestFixture.ThirtyDays.received.txt";

    const std::string actual = renderTexttestFixtureOutput(30);

    ASSERT_TRUE(std::filesystem::exists(expectedPath))
        << "Missing golden master file: " << expectedPath
        << "\nCreate or approve it from the received output.";

    const std::string expected = normalizeLineEndings(readFile(expectedPath));
    if (expected != actual) {
        writeFile(receivedPath, actual);
    }

    EXPECT_EQ(expected, actual)
        << "Golden master mismatch. Actual output was written to: "
        << receivedPath;
}
