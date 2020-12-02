// Aoc - 2020 Day1 Report Repair
#include "stdafx.h"
#include "catch.hpp"
#include "Utils.h"

struct Solve 
{
  set<int> hist;
  vector<string> data;
  Solve(const string& inStr) {
    data = GetLines(inStr);
    for (auto i : data)
      hist.insert(stoi(i));
  };
    
  string Do()
  {
    for (auto i : hist)
    {
      if (hist.find(2020 - i) != hist.end())
      {
        return to_string(i * (2020 -i));

      }
    }
    return string();
  }
  
  string Do2()
  {
    for (auto j : hist)
      for (auto i : hist)
      {
        if (hist.find(2020 - i - j) != hist.end())
        {
          return to_string(i * j * (2020 - i - j));

        }
      }
    return string();
  }
};

TEST_CASE("Sample 1", "[.]") {
  cout << endl << "Tests   ------------- " << endl;

  REQUIRE(Solve(R"(1721
979
366
299
675
1456)").Do() == "514579");
}

TEST_CASE("Part One", "[x.]") {
  cout << endl << "Part One ------------- " << endl;
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do());  
}

TEST_CASE("Part Two", "[x.]") {
  toClipboard(Solve(ReadFileToString(L"input.txt")).Do2());
 
  cout << endl << "Part Two ------------- " << endl;
}
