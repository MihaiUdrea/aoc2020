// day9 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "day9"

#define FIRST_STAR  "22477624"
#define SECOND_STAR "2980044"

#ifdef THISDAY
  #define TODAY THISDAY "/"
  #define HIDE_IF_OLD_TEST "[.]"
#else
  #define TODAY
  #define HIDE_IF_OLD_TEST
#endif  // THISDAY

namespace
{
struct Solve
{

  vector<string> input;
  vector<int64_t>   nrList;
  set<int64_t>   sums;

  Solve(const string & inStr){

    forEachLine(inStr, [&](string line) {
      auto s = stoll(line);
      nrList.push_back(s);
    });
  };

  string Do(int preambleCount = 25)
  { 
    
    for (auto i : irange(preambleCount, (int)nrList.size()))
    {
      sums.clear();
      bool found = false;
      for (auto j : irange(i - preambleCount+1, i))
        for (auto x : irange(i - preambleCount, j))
        sums.insert(nrList[x] + nrList[j]);

      if (sums.find(nrList[i]) == sums.end())
        return to_string(nrList[i]);
    }

    return to_string(-1); }

  string Do2(int64_t nr = 22477624)
  { 
    int64_t sum = nrList[0];
    int     startList = 0;
    int     endList   = 1;
    for (auto i = 1; i < nrList.size(); i++)
    {
      auto newSum = sum + nrList[i];
      if (newSum < nr)
      {
        sum = newSum;        
        endList++;
      }
      else if (newSum > nr)
      {
        sum -= nrList[startList]; 
        startList++;
        i--;
      }
      else
      {
        const auto [min, max] =
          minmax_element(nrList.begin() + startList, nrList.begin() + endList);
        return to_string(*min + *max); 
      }
    }
    return to_string(-1); }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1 + R"(
35
20
15
25
47
40
62
55
65
95
102
117
150
182
127
219
299
277
309
576)")
            .Do(5) == "127");
}

TEST_CASE(TODAY "Sample 2", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1 + R"(
35
20
15
25
47
40
62
55
65
95
102
117
150
182
127
219
299
277
309
576)")
            .Do2(127) == "62");
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
