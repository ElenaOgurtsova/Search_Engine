#include "gtest/gtest.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include "ConverterJSON.h"
#include "Checks.h"
#include "Entry.h"
#include "Exceptions.h"

TEST(ExampleTest, AlwaysTrue) {
    EXPECT_EQ(1, 1);
    ASSERT_TRUE(true);
}

TEST(sample_test_case, sample_test)
{
    EXPECT_EQ(1, 1);

}

void TestInvertedIndexFunctionality(
    std::vector<std::string>& docs,
    std::vector<std::string>& requests,
    std::vector<std::vector<Entry>>& expected
) {
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;
    idx.UpdateDocumentBase( docs);
        for (auto& request : requests) {
        std::vector<Entry> word_count = idx.GetWordCount(request);
        result.push_back(word_count);
    }
    ASSERT_EQ(result, expected);
}

TEST(TestCaseInvertedIndex, TestBasic) {
    std::vector<std::string> docs = {
        "london is the capital of great britain",
        "big ben is the nickname for the Great bell of the striking clock"
    };
    std::vector<std::string> requests = {"london", "the"};
    std::vector<std::vector<Entry>> expected = {
        {{0, 1}},
        {{0, 1}, {1, 3}}
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestBasic2) {
    std::vector<std::string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuchino"
    };
    std::vector<std::string> requests = {"milk", "water", "cappuchino"};
    std::vector<std::vector<Entry>> expected = {
        {{0, 4}, {1, 1}, {2, 5}},
        {{0, 3}, {1, 2}, {2, 5}},
        {{{3, 1}}}
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    std::vector<std::string> docs = {
        "a b c d e f g h i j k l",
        "statement"
    };
    std::vector<std::string> requests = {"m", "statement"};
    std::vector<std::vector<Entry>> expected = {
        {},
        {{1, 1}}
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseSearchServer, TestSimple) {
vector<string> docs = {
"milk milk milk milk water water water",
"milk water water",
"milk milk milk milk milk water water water water water",
"americano cappuccino"
};
const vector<string> request = {"milk water", "sugar"};
const std::vector<vector<RelativeIndex>> expected = {
{
{2, 1},
{0, 0.699999988},
{1, 0.300000012},
},
{

}
};
InvertedIndex idx;
idx.UpdateDocumentBase(docs);
SearchServer srv(idx);
std::vector<vector<RelativeIndex>> result = srv.search(request);
ASSERT_EQ(result, expected);
}
TEST(TestCaseSearchServer, TestTop5) {
vector<string> docs = {
"london is the capital of great britain",
"paris is the capital of france",
"berlin is the capital of germany",
"rome is the capital of italy",
"madrid is the capital of spain",
"lisboa is the capital of portugal",
"bern is the capital of switzerland",
"moscow is the capital of russia",
"kiev is the capital of ukraine",
"minsk is the capital of belarus",
"astana is the capital of kazakhstan",
"beijing is the capital of china",
"tokyo is the capital of japan",
"bangkok is the capital of thailand",
"welcome to moscow the capital of russia the third rome",
"amsterdam is the capital of netherlands",
"helsinki is the capital of finland",
"oslo is the capital of norway",
"stockholm is the capital of sweden",
"riga is the capital of latvia",
"tallinn is the capital of estonia",
"warsaw is the capital of poland",
};
vector<string> request = {"moscow is the capital of russia"};
std::vector<vector<RelativeIndex>> expected = {
{
{7, 1},
{14, 1},
{0, 0.666666687},
{1, 0.666666687},
{2, 0.666666687},

}
};
InvertedIndex idx;
idx.UpdateDocumentBase(docs);
SearchServer srv(idx);
std::vector<vector<RelativeIndex>> result = srv.search(request);
ASSERT_EQ(result, expected);
}


