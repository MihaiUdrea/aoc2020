// day8 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"
#include <ranges>

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

struct Program
{
  enum instructions
  {
    nop,
    acc,
    jmp,

    max_inst
  };

  struct data
  {
    int val;
    int opcode;
  };

  vector<data> list;

  int crInst = 0;
  int regAcc = 0;

  Program(const string & inStr)
  {
    vector instructionsOrder{ "nop"s, "acc"s, "jmp"s };

    for (auto line : GetLines(inStr))
    {
      static const regex matchExp(R"~((\w+) ([+-]?\d*))~");
      auto               res = match_rx(line, matchExp);

      if (res.empty())
        continue;

      auto opcode = ranges::find(instructionsOrder, res[1]) - instructionsOrder.begin();

      list.push_back({ stoi(res[2]), static_cast<instructions>(opcode) });
    };
  }

  void ExecCurrent()
  {
    auto inst = list[crInst];

    if (inst.opcode == jmp)
      crInst += inst.val;
    else if (inst.opcode == nop)
      crInst++;
    else
    {
      regAcc += inst.val;
      crInst++;
    }
  }
};

struct Solve : Program
{
  set<int> hist;

  Solve(const string & inStr)
    : Program(inStr)
  {
  }


  string Do() { 
    while (crInst >= 0 && crInst < (int) list.size())
    {
      if (hist.find(crInst) != hist.end())
        return to_string(regAcc);

      hist.insert(crInst);

      ExecCurrent();
    }
    return to_string(regAcc); }

  string Do2() { 
    for (auto & inst : list)
    {
      crInst = 0;
      regAcc    = 0;
      hist.clear();

      if (inst.opcode == jmp)
      {
        inst.opcode = nop;
        Do();
        inst.opcode = jmp;
      }
      else if (inst.opcode == nop)
      {
        inst.opcode = jmp;
        Do();
        inst.opcode = nop;
      }
      else
        continue;

      if (crInst == list.size())
        return to_string(regAcc);
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
