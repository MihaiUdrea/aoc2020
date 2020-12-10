// day10 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "day10"

#define FIRST_STAR  "2048"
#define SECOND_STAR "1322306994176"

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

  vector<int> input;
  set<int>    myset;

  Solve(const string & inStr){
    forEachLine(inStr, [&](string line) {
      input.push_back(stoi(line));
      myset.insert(stoi(line));
    });
  };

  string Do() {

    input.push_back(0);
    sort(input.begin(), input.end());

    int d3 = 1;
    int d1 = 0;
    for (auto i : irange(1, input.size()))
    {
      auto diff = input[i] - input[i - 1];
      if (diff == 1)
      {
        d1++;
      }
      else if (diff == 3)
      {
        d3++;
      }
    }

    return to_string(d1*d3); 
  }

  set<int> crSet;
  int64_t mods = 0;
  int      max  = 0;
  
  map<int, int64_t> results;
  void Search(int last) 
  { 
    if (last == max)
    {
      mods++;

      return;
    } else if (last > max)
    {
      return;
    }

    for (auto i : irange(last + 1, last + 4))
    {
      if (myset.find(i) != myset.end() && crSet.find(i) == crSet.end())
      {
        if (results.find(i) == results.end())
        {
          int64_t crMods = mods;
          crSet.insert(i);
          Search(i);
          crSet.erase(i);
          results[i] = mods - crMods;
        }
        else
          mods += results[i];
        
      }
    }
  }
   
  string Do2() 
  {
    max = *max_element(input.begin(), input.end());

    Search(0);
    return to_string(mods);
  }

};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[x.]")
{
  cout << endl << "Tests   ------------- " << endl;

  REQUIRE(Solve(1 + R"(
16
10
15
5
1
11
7
19
6
12
4)")
            .Do2() == "8");
}

TEST_CASE(TODAY "Sample 2", HIDE_IF_OLD_TEST "[x.]")
{
  cout << endl << "Tests   ------------- " << endl;

  REQUIRE(Solve(1 + R"(
28
33
18
42
31
14
46
20
48
47
24
23
49
45
19
38
39
11
1
32
25
35
8
17
7
9
4
2
34
10
3)")
            .Do2() == "19208");
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
