// Aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
// TEMPLATE
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"

struct Solve 
{
  int sol = 0;
  int sol2 = 0;
  set<int> hist;
  vector<string> data;
  Solve(const string& inStr) {
    forEachRxToken(inStr, lineRxToken, [&](string line) {
      static const regex matchExp(R"~((\d+)-(\d+) (.): (\w+))~");
      auto res = match_rx(line, matchExp);

      int start = stoi(res[1]);
      int end = stoi(res[2]);
      char s = res[3].str()[0];
      string pass = res[4];

      auto c = count(pass.begin(), pass.end(), s);

      if (c >= start && c <= end)
        sol++;

      if ((pass[start - 1] == s && pass[end - 1] != s) || (pass[end - 1] == s && pass[start - 1] != s))
        sol2++;
      });
      
      
  };
    
  string Do()
  {

    return string(to_string(sol));
  }

  
  string Do2()
  {

    return string(to_string(sol2));
  }
};

TEST_CASE("Sample 1", "[x.]") {
  cout << endl << "Tests   ------------- " << endl;

  REQUIRE(Solve(R"(1-3 a: abcde
1-3 b: cdefg
2-9 c: ccccccccc)").Do() == "2");

}

TEST_CASE("Part One", "[x.]") {
  cout << endl << "Part One ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do());  
}

TEST_CASE("Part Two", "[x.]") {
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do2());
 
  cout << endl << "Part Two ------------- " << endl;
}
