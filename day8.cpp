// day8 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "day8"

#define FIRST_STAR  "1489"
#define SECOND_STAR "1539"

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
  struct data
  {
    int    val;
    
    string inst;
  };

  vector<data> list;
  set<int>     hist;

  int            crInst = 0;
  int            acc    = 0;

  Solve(const string & inStr){

    for (auto line : GetLines(inStr))
    {
      static const regex matchExp(R"~((\w+) ([+-]?\d*))~");
      auto               res = match_rx(line, matchExp);

      if (res.empty())
        continue;

      list.push_back({ stoi(res[2]), res[1]});
    };
  };

  string Do() { 
    while (crInst >= 0 && crInst < list.size())
    {
      if (hist.find(crInst) != hist.end())
        return to_string(acc);

      hist.insert(crInst);

      auto inst = list[crInst];

      if (inst.inst == "jmp")
        crInst += inst.val;
      else if (inst.inst == "nop")
        crInst++;
      else
      {
        acc += inst.val;
        crInst++;
      }
    }
    return to_string(acc); }

  string Do2() { 
    for (auto & inst : list)
    {
      crInst = 0;
      acc    = 0;
      hist.clear();

      if (inst.inst == "jmp")
      {
        inst.inst = "nop";
        Do();
        inst.inst = "jmp";
      }
      else if (inst.inst == "nop")
      {
        inst.inst = "jmp";
        Do();
        inst.inst = "nop";
      }
      else
        continue;

      if (crInst == list.size())
        return to_string(acc);
    }
    
    return to_string(2); }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[.]")
{
  REQUIRE(Solve(1 + R"(
nop +0
acc +1
jmp +4
acc +3
jmp -3
acc -99
acc +1
jmp -4
acc +6)")
            .Do() == "5");
}

TEST_CASE(TODAY "Sample 2", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1 + R"(
nop +0
acc +1
jmp +4
acc +3
jmp -3
acc -99
acc +1
jmp -4
acc +6)")
            .Do2() == "8");
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
