// day18 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "day18"

#define FIRST_STAR  "7293529867931"
#define SECOND_STAR "60807587180737"

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
  /** /
  struct data
  {
    int    start;
    int    end;
    char   ch;
    string pass;
  };

  vector<data> list;
  /**/

  vector<string> input;

  Solve(const string & inStr){
    input = GetLines(inStr);
  };

  INT64 Evaluate2(string left, string right, char op) 
  { 
    INT64 lv = Evaluate(left);
    INT64 rv = Evaluate(right);
    switch (op)
    {
    case '+':
      return lv + rv;
    case '-':
      return lv - rv;
    case '*':
      return lv * rv;
    }

    return -1;
  }

  INT64 Evaluate(string line) 
  { 
    int  depth        = 0;
    vector<pair<int, int>> brackets;
    int                    lastPosOpOr = -1;
    int                    lastPosOpPlus = -1;
    for (auto [idx, ch] : line | ranges::views::enumerate)
    {
      if (ch == '(')
      {
        depth++;
        brackets.push_back({ idx, -1 });
      }
      else if (ch == ')')
      {
        depth--;
        brackets.back().second = idx;
      }
      else if (depth == 0 && (ch == '*'))
        lastPosOpOr = idx;        
       else if (depth == 0 && (ch == '-' || ch == '+'))
        lastPosOpPlus = idx;        
    }

    int lastPosOp = -1;

    if (!mPart2)
      lastPosOp = std::max(lastPosOpOr, lastPosOpPlus);
    else
    {
      // part 2
      if (lastPosOpOr != -1)
        lastPosOp = lastPosOpOr;
      else
        lastPosOp = lastPosOpPlus;
    }

    if (lastPosOp != -1)
      return Evaluate2(line.substr(0, lastPosOp - 1), line.substr(lastPosOp + 1), line[lastPosOp]);

    int firstBracket = line.find('(');    
    int lastBracket = line.find_last_of(')');

    if (firstBracket >= 0 && firstBracket <= 1 && lastBracket >= line.size() - 2)
    {
      int startPos = firstBracket + 1;
      int endPos = line.find_last_of(')') - startPos;

      return Evaluate(line.substr(startPos, endPos));
    }
    
    auto firstCharPos = line.find_first_of("+-*");
    if (firstCharPos == line.npos)
    {
      return stoll(line);
    }  
    else
    {
      return -1;
    }
  }

  string Do() { 
    vector<INT64> results;

    for (auto i : input)
      results.push_back(Evaluate(i));

    return to_string(ranges::accumulate(results, 0ll)); 
  }
  bool   mPart2 = false;
  string Do2() { 
    mPart2 = true;
    return Do(); }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1 + R"(
2 * 3 + (4 * 5))")
            .Do() == "26");
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
