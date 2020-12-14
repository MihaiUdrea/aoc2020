// day14 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

//#define THISDAY "day14"

//#define FIRST_STAR  ""
//#define SECOND_STAR ""

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
  /**/
  struct data
  {
    INT64 first;
    INT64 second;
    char   ch;
    string raw;
  };

  vector<data> list;
  /**/

  vector<string> input;
  map<INT64, INT64> mem;
  Solve(const string & inStr){

    // input = GetLines(inStr);

    /**/
    forEachLine(inStr, [&](string line) {
      static const regex matchExp(R"~((mask = (\w+))|(mem\[(\d+)\] = (\w+)))~");
      auto               res = match_rx(line, matchExp);

      data d;
      
      if (!res[0].matched)
      {
        return;
      }

      if (res[1].matched)
      {
        d.ch = 'M';
        // mask
        d.raw = res[2];
        string s1     = d.raw;
        for (auto & ch : s1)
          if (ch == 'X')
            ch = '0';

        string s0 = d.raw;
        for (auto & ch : s0)
          if (ch == 'X')
            ch = '1';

        d.first = stoll(s0, 0, 2);
        d.second = stoll(s1, 0, 2);
      }
      else
      {
        // mem
        d.ch = 'W';

        d.first = stoll(res[4]);
        d.second = stoll(res[5]);
        d.raw    = res[3];
      }
      list.push_back(d);
    });
    
  };

  string Do2() { 

    struct MaskEx : data
    {
      set<int> ones;
      //set<int> zeroes;
      vector<int> xes;

      MaskEx(string & s)
      {
        int pow = s.length();
        for (auto ch : s)
        {
          pow--;

          if (ch == '1')
            ones.insert(pow);
          else if (ch == 'X')
            xes.push_back(pow);
        }
      }
    };

    string s;
    MaskEx mask(s);

    for (auto inst : list)
    {
      if (inst.ch == 'M')
      {
        // mask
        mask = MaskEx(inst.raw);
        mask.first = inst.first;
        mask.second = inst.second;
      }
      else
      {
        MaskEx myMask = mask;

        // mem
        auto v1 = inst.first;
        for (auto onePow : mask.ones)
        {
          auto v2 = inst.first | (1ll << onePow);
          inst.first = v2;
        }
        for (auto xPow : mask.xes)
        {
          auto v2    = inst.first & (~(1ll << xPow));
          inst.first = v2;
        }

        vector<INT64> addrList;
        for (auto step : irange(0, 2 << (mask.xes.size() - 1)))
        {
          INT64 v2 = inst.first;
          for (auto xPowIdx : irange(0, mask.xes.size()))
          {
            //int xPowIdx = step;

            auto & xPow = mask.xes[xPowIdx];

            bool oneOrzero = (step >> xPowIdx) % 2;

            if (oneOrzero)
            {
              INT64 v3 = v2 | (1ll << xPow);
              v2      = v3;
            }
            else
            {
              INT64 v3 = v2 & (~(1ll << xPow));
              v2 = v3;
            }
          }
          addrList.push_back(v2);
        }

        for (auto m : addrList)
        {
          mem[m] = inst.second;
        }
      }
    }

    INT64 sum = 0;
    for (auto m : mem)
    {
      sum += m.second;
    }

    return to_string(sum); }

  string Do() {
    data mask;

    for (auto inst : list)
    {
      if (inst.ch == 'M')
      {
        // mask
        mask = inst;
      }
      else
      {
        // mem
        auto v1 = inst.second & mask.first;
        auto v2 = v1 | mask.second;

        mem[inst.first] = v2;
      }
    }

    INT64 sum = 0;
    for (auto m : mem)
    {
      sum += m.second;
    }

    return to_string(sum);
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[.]")
{
  REQUIRE(Solve(1 + R"(
mask = 000000000000000000000000000000X1001X
mem[42] = 100
mask = 00000000000000000000000000000000X0XX
mem[26] = 1)")
            .Do2() == "208");
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
