﻿// Aoc - 2020 day2 Password Philosophy
#include "stdafx.h"

#include "Utils.h"

#define THISDAY "2"

#ifdef THISDAY
#define TODAY DAY THISDAY "/"
#else
#define TODAY
#endif  // THISDAY

namespace
{
struct Solve
{
  struct data
  {
    int    start;
    int    end;
    char   ch;
    string pass;
  };

  vector<data> list;

  Solve(const string & inStr)
  {
    forEachLine(inStr, [&](string line) {
      static const regex matchExp(R"~((\d+)-(\d+) (.): (\w+))~");
      auto               res = match_rx(line, matchExp);
      list.push_back({ stoi(res[1]), stoi(res[2]), res[3].str()[0], res[4] });
    });
  };

  string Do()
  {
    auto sol = count_if(list.begin(), list.end(), [](data & el) {
      auto c = count(el.pass.begin(), el.pass.end(), el.ch);
      return c >= el.start && c <= el.end;
    });
    return string(to_string(sol));
  }

  string Do2()
  {
    auto sol = count_if(list.begin(), list.end(), [](data & el) {
      auto l = el.pass[el.start - 1];
      auto r = el.pass[el.end - 1];
      return (l == el.ch) ^ (r == el.ch);
    });
    return string(to_string(sol));
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", "[.]")
{
  REQUIRE(Solve(R"(1-3 a: abcde
1-3 b: cdefg
2-9 c: ccccccccc)")
            .Do() == "2");
}

TEST_CASE(TODAY "Part One", "[.]")
{
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do() == "560");
}

TEST_CASE(TODAY "Part Two", "[.]")
{
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do2() == "303");
}

