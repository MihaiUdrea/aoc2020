// day16 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "day16"

#define FIRST_STAR  "24110"
#define SECOND_STAR "6766503490793"

#ifdef THISDAY
  #define TODAY THISDAY "/"
  #define HIDE_IF_OLD_TEST "[.]"
#else
  #define TODAY
  #define HIDE_IF_OLD_TEST
#endif  // THISDAY

// disable conversion warning
#pragma warning(disable : 4267)


//namespace
//{
struct Solve
{
  /**/
  struct data
  {
    pair<int, int> firstInt;
    pair<int, int> secondInt;
    string         str;
  };

  vector<data> list;
  /**/

  vector<string> input;
  vector<int> yourTicket;
  vector<vector<int>>    nearByTicket;

  Solve(const string & inStr){

    // input = GetLines(inStr);

    /** /
    forEachLine(inStr, [&](string line) {
      static const regex matchExp(R"~((\d+)-(\d+) (.): (\w+))~");
      auto               res = match_rx(line, matchExp);
      list.push_back({ stoi(res[1]), stoi(res[2]), res[3].str()[0], res[4] });
    });
    /**/

    /**/
    static const regex colsSepRx("\n\n");
    forEachRxTokenIdx(inStr, colsSepRx, [&](string chunk, int idx) {
      
    forEachLineIdx(chunk, [&](string line, int idx2) {
        if (idx == 0)
        {
          data               d;
          static const regex matchExp(R"~(([^:]+): (\d+)-(\d+) or (\d+)-(\d+))~");
          auto               res = match_rx(line, matchExp);
          list.push_back(
            { { stoi(res[2]), stoi(res[3]) }, { stoi(res[4]), stoi(res[5]) }, res[1].str() });
        }
        else if (idx == 1)
        {
          if (idx2 == 1)
          {
            static const regex colsSepRx(",");
            forEachRxToken(line, colsSepRx, [&](string nrS) {
              yourTicket.push_back(stoi(nrS));
            });
          }
        }
        else
        {
          if (idx2 >= 1)
          {
            nearByTicket.push_back({});
            static const regex colsSepRx(",");
            forEachRxToken(line, colsSepRx, [&](string nrS) {
              nearByTicket.back().push_back(stoi(nrS));
            });
          }
        }
      }
      );
    });
    /**/
  };

  string Do() { 
    vector<int> invalidList;
    for (auto n : nearByTicket)
    {
      for (auto k : n)
      {
        bool found = false;
        for (auto w : list)
        {
          if ((w.firstInt.first <= k && k <= w.firstInt.second) ||
              (w.secondInt.first <= k && k <= w.secondInt.second))
          {
            found = true;
            break;
          }
        }
        if (!found)
          invalidList.push_back(k);
      }
    }

    int sum = 0;
    for (auto v : invalidList)
    {
      sum += v;
    }

    return to_string(sum); }

  string Do2()
  {
    vector<int> invalidList;
    for (auto idxn : irange(0, nearByTicket.size()))
    {
      auto & n = nearByTicket[idxn]; 
      for (auto idxk : irange(0, n.size()))
      {
        auto & k     = n[idxk];
        bool found = false;
        for (auto w : list)
        {
          if ((w.firstInt.first <= k && k <= w.firstInt.second) ||
              (w.secondInt.first <= k && k <= w.secondInt.second))
          {
            found = true;
            break;
          }
        }
        if (!found)
        {
          invalidList.push_back(idxn);
          break;
        }
      }
    }

    for (auto eraseIdx : irange(invalidList.size() - 1, -1))
    {
      nearByTicket.erase(nearByTicket.begin() + invalidList[eraseIdx]);
    }

    map<int, set<int>> nokPos;

    nearByTicket.push_back(yourTicket);
    for (auto idxn : irange(0, nearByTicket.size()))
    {
      auto & n = nearByTicket[idxn];
      for (auto idxk : irange(0, n.size()))
      {
        auto & k     = n[idxk];
        bool   found = false;
        for (auto idxw : irange(0, list.size()))
        {
          auto & w = list[idxw];
          if ((w.firstInt.first <= k && k <= w.firstInt.second) ||
              (w.secondInt.first <= k && k <= w.secondInt.second))
          {
            found = true;
          }
          else
          {
            nokPos[idxk].insert(idxw);
          }
        }
        
      }
    }

    map<int, int> goodPos;
    set<int>      usedPos;
    for (auto i : irange(0, list.size()))
      goodPos[i] = -1;

    for (auto i : irange(list.size() -1, -1))
    {
      for (auto vIdx : irange(0, nokPos.size()))
      {
        if (nokPos[vIdx].size() == i)
        {
          set<int> candidatesList;
          for (auto i : irange(0, list.size()))
            if (!usedPos.contains(i))
              candidatesList.insert(i);

          for (auto p : nokPos[vIdx])
            candidatesList.erase(p);


          if (candidatesList.size() == 1)
          {
            // OK
            auto newgoodPos = *candidatesList.begin();
            goodPos[vIdx]   = newgoodPos;
            usedPos.insert(newgoodPos);
          }
          else
          {
            // bad;
            int qi = 0;
            qi++;
          }
        }
      }
    }

    /** /
    INT64 res = 1;
    for (auto idx : irange(0, 6))
    {
      res *= yourTicket[goodPos[idx]];
    }
    /**/

    INT64 res2 = 1;
    for (auto idx : goodPos)
    {
      if (idx.second < 6)
        res2 *= yourTicket[idx.first];
    }

    return to_string(res2);
  }
};
//}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[.]")
{
  REQUIRE(Solve(1 + R"(
class: 1-3 or 5-7
row: 6-11 or 33-44
seat: 13-40 or 45-50

your ticket:
7,1,14

nearby tickets:
7,3,47
40,4,50
55,2,20
38,6,12)")
            .Do() == "71");
}

TEST_CASE(TODAY "Sample 2", HIDE_IF_OLD_TEST "[.]")
{
  REQUIRE(Solve(1 + R"(
class: 0-1 or 4-19
row: 0-5 or 8-19
seat: 0-13 or 16-19

your ticket:
11,12,13

nearby tickets:
3,9,18
15,1,5
5,14,9)")
            .Do2() == "1716");
}


TEST_CASE(TODAY "Part One", HIDE_IF_OLD_TEST "[.]")
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
