// day7 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

//#define THISDAY "day7"

#define FIRST_STAR  "229"
#define SECOND_STAR "6683"

#ifdef THISDAY
  #define TODAY THISDAY "/"
  #define HIDE_IF_OLD_TEST "[.]"
#else
  #define TODAY
  #define HIDE_IF_OLD_TEST
#endif  // THISDAY

// disable conversion warning
#pragma warning(disable : 4267)
namespace
{

struct Solve
{
  struct BagAndCount
  {
    int    count;    
    string bag;
  };

  using BagsList = vector<BagAndCount>;

  map<string, BagsList> data;
  map<string, BagsList> dataBack;
  Solve(const string & inStr){
    const regex rxToken(", ");

    // input = GetLines();

    /**/
    forEachLine(inStr, [&](string line) {
      static const regex matchExp(R"~((\w+ \w+) bags contain (.+))~");
      auto               res = match_rx(line, matchExp);
      BagsList &         list = data[res[1].str()];
      string             Startbag = res[1];
      forEachRxToken(res[2].str(), rxToken, [&](string groupRaw) {
        static const regex matchExp(R"~((\d*) (\w+ \w+) bags?.?)~");
        auto               res2 = match_rx(groupRaw, matchExp);
        if (res2.empty())
          return;
        BagAndCount        bc;
        bc.bag = res2[2];
        bc.count = stoi(res2[1].str());
        list.push_back(bc);


        BagAndCount bcRev;
        bcRev.bag = res[1];
        bcRev.count = stoi(res2[1].str());
        dataBack[res2[2]].push_back(bcRev);
        });
    });
    /**/

    /** /
    forEachLine(inStr, [&](string line) {
      static const regex colsSepRx(":");
      forEachRxToken(line, colsSepRx, [&](string chunk) {
        int i = 0;
        i++;
      });
    });
    /**/
  };

  set<string> hist;

  void Parse(string s) 
  { 
    if (hist.find(s) != hist.end())
      return;

    hist.insert(s);

    for (auto parent : dataBack[s])
    {
      Parse(parent.bag);
    }
  }

  string      Do()
  {
    Parse("shiny gold");
    return to_string(hist.size() - 1); 
  }


  map<string, int> bagSize;
  void Parse2(string s)
  {
    if (bagSize.find(s) != bagSize.end())
      return;

    for (auto bagInside : data[s])
    {
      Parse2(bagInside.bag);
      bagSize[s] += (bagSize[bagInside.bag] + 1) * bagInside.count;
    }
  }

  string Do2() { 
    Parse2("shiny gold");
    return to_string(bagSize["shiny gold"]);
  }
};

}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1 + R"(
light red bags contain 1 bright white bag, 2 muted yellow bags.
dark orange bags contain 3 bright white bags, 4 muted yellow bags.
bright white bags contain 1 shiny gold bag.
muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.
shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.
dark olive bags contain 3 faded blue bags, 4 dotted black bags.
vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.
faded blue bags contain no other bags.
dotted black bags contain no other bags.)")
            .Do() == "4");
}

TEST_CASE(TODAY "Sample 2", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1 + R"(
shiny gold bags contain 2 dark red bags.
dark red bags contain 2 dark orange bags.
dark orange bags contain 2 dark yellow bags.
dark yellow bags contain 2 dark green bags.
dark green bags contain 2 dark blue bags.
dark blue bags contain 2 dark violet bags.
dark violet bags contain no other bags.)")
            .Do2() == "126");
}

TEST_CASE(TODAY "Sample 21", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1 + R"(
light red bags contain 1 bright white bag, 2 muted yellow bags.
dark orange bags contain 3 bright white bags, 4 muted yellow bags.
bright white bags contain 1 shiny gold bag.
muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.
shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.
dark olive bags contain 3 faded blue bags, 4 dotted black bags.
vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.
faded blue bags contain no other bags.
dotted black bags contain no other bags.)")
            .Do2() == "32");
}

TEST_CASE(TODAY "Part One", HIDE_IF_OLD_TEST "[x.]")
{
#ifdef FIRST_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do() == FIRST_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do());
#endif  // FIRST_STAR
}

TEST_CASE(TODAY "Part Two", HIDE_IF_OLD_TEST "[x.]")
{
#ifdef SECOND_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do2() == SECOND_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do2());
#endif
}
