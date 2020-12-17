// day13 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"
#include <execution>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/integer/mod_inverse.hpp>

namespace mp = boost::multiprecision;

#define THISDAY "day13"
#define FIRST_STAR  "4938"
#define SECOND_STAR "230903629977901"

#ifdef THISDAY
  #define TODAY THISDAY "/"
  #define HIDE_IF_OLD_TEST "[.]"
#else
  #define TODAY
  #define HIDE_IF_OLD_TEST
#endif  // THISDAY

// disable conversion warning
#pragma warning(disable : 4267)
#pragma warning(disable : 4996)


//namespace
//{
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
  vector<string> buses;
  vector<INT64>  delta;

  vector<INT64> runing;
  int         start;
  Solve(const string & inStr)
  {
    input = GetLines(inStr);
    start = stoi(input[0]);

    static const regex colsSepRx(",");
    forEachRxToken(input[1], colsSepRx, [&](string chunk) {
      buses.push_back(chunk);
    });

    int idx = 0;
    for (auto b : buses)
    {
      if (b != "x")
      {
        runing.push_back(stoi(b));
        delta.push_back(idx);
      }

      idx++;
    }

    
    /** /
    forEachLine(inStr, [&](string line) {
      static const regex matchExp(R"~((\d+)-(\d+) (.): (\w+))~");
      auto               res = match_rx(line, matchExp);
      list.push_back({ stoi(res[1]), stoi(res[2]), res[3].str()[0], res[4] });
    });
    /**/

    /** /
    forEachLine(inStr, [&](string line) {
      static const regex colsSepRx(":");
      forEachRxToken(line, colsSepRx, [&](string chunk) {
        int i = 0;
        i++;
      });
    });
    /**/
  };

  string Do()
  {
    vector<int> remaining;
    for (auto b : runing)
    {
      remaining.push_back(b - start % b);
    }

    auto min = min_element(remaining.begin(), remaining.end());

    return to_string(runing[min - remaining.begin()] * (*min));
  }

  vector<INT64> histRef;
  map<int, vector<INT64>> stepsList;
  map < int, vector<INT64>> multiplierList;
  map<int, vector<pair<INT64, INT64>>> partials;
  string Do4()
  {
    for (auto i : irange(1, runing.size()))
    {
      delta[i] = (runing[i] * 20 - delta[i]) % runing[i];
    }

    INT64         crBus = runing[0];
    
    for (auto i : irange(1, runing.size()))
    {
      auto nexDiff = delta[i];

      if (1)
      {
        cout << "\nCrBus: " << crBus;
        cout << "\nNextNr: " << runing[i];
        cout << "\nNextTargetMod: " << nexDiff;

        if (nexDiff < 0)
        {
          MessageBoxA(0, 0, 0, 0);
        }

        cout << endl << crBus << " * 0...  % " << runing[i] << " = " << endl;

        for (auto x1 = 0; x1 < lcm(crBus, runing[i]); x1 += crBus)
          cout << x1 % runing[i] << " ";
      }
      

      for (INT64 k = 0, step = 0; /*k < lcmV*/; k += crBus, step++)
      {
        int multiplier = 0;
        int p          = i;
        for (; p > 0; p--)
        {
          auto v = k % runing[p];
          if (v != delta[p])
          {
            break;
          }
        }

        int p2 = i;
        if (false && (p != 0 && p < i))
        {
          for (INT64 k2 = k; /*k < lcmV*/; k2 = k2 + runing[i] * crBus)
          {
            multiplier++;
            p2 = i - 1;
            for (; p2 > 0; p2--)
            {
              auto v = k2 % runing[p2];
              if (v != delta[p2])
              {
                break;
              }
            }

            if (p2 == 0)
            {
              // evrika
              k = k2;
              break;
            }
          }
        }

        if (p == 0 || p2 == 0)
        {
          stepsList[i].push_back(step);
          multiplierList[i].push_back(multiplier == 0 ? partials[i].size()  : multiplier);

          crBus = k;
          histRef.push_back(crBus);
          break;
        }
        else if (p < i && p2 == i)
        {
          // partial match
          partials[i].push_back({ k, step });
        }
      }
    }

    INT64 lc = runing[0];
    for (auto i : irange(1, runing.size()))
    {
      lc = std::lcm(lc, runing[i]);
    }

    return to_string(crBus % lc);
  }

  vector<int>   coef;
  vector<int>   coef2;
  vector<int>   inv;
  vector<INT64> HIST;

  string Do5()
  {
    Do4();

    for (auto i : runing)
    {
      coef.push_back(0);
      coef2.push_back(0);
    }

    if (0)
    for (auto i : irange(1, runing.size()))
    {
      delta[i] = (runing[i] * 20 - delta[i]) % runing[i];
    }

    INT64 crNr = runing[0];
    INT64 lc   = runing[0];
    for (auto i : irange(1, runing.size()))
    {
      lc = std::lcm(lc, runing[i]);

      mp::int128_t moduloInvMap;
      auto         base = runing[i];
      
      auto         targetDif = delta[i];

      auto invers = boost::integer::mod_inverse<mp::int128_t>(crNr, base);
      auto x      = invers.convert_to<INT64>();
      

      //cout << endl << invers;
      coef[i] = (targetDif * x) % base;
      if (delta[i-1] != 0)
      {
        auto prevDelta = delta[i - 1];

        auto a1       = coef[i] * prevDelta;
        auto invers2 = boost::integer::mod_inverse<mp::int128_t>(a1, base);
        auto x2       = invers2.convert_to<INT64>();
        //cout << endl << "       " << invers2;
        //coef2[i] = (x2 - 1);

        if ((a1 % runing[i-1])!= 0)
        {
          int bSec    = runing[i - 1];
          int m1    = a1 % bSec;  // 55 % 13 = 3
          int posS1   = m1 * inv.back() % bSec;  // 3 * 10 % 13 = 4

          int a2    = (coef[i] + base ) * crNr; // 5 * 102 + 19 * 102 = 2448;
          int m2    = a2 % bSec;  // 2446 % 13 = 4
          int posS2 = m2 * inv.back() % bSec;  // 4 * 10 % 13 = 4

          int diffPos = (bSec + posS1 - posS2) % bSec;

          int c2  = (coef[i - 1] * posS1) / diffPos; // big nr!!!!!!! modulo?
          coef2[i] = c2;
        }
        else
        {
          auto vx  = lcm(coef[i - 1], base % delta[i - 1]);
          coef2[i] = vx;
        }

        crNr = ((coef[i] + coef2[i] * base) * crNr) % lc;
      }
      else
      {
        crNr = coef[i] * crNr;
      }

      HIST.push_back(crNr);
      inv.push_back(x);

      //cout << endl << crNr;
    }
    return to_string(crNr % lc);
  }

  string Do6() {
    long long t = 0, step = 1;

    long long off = 0;
    for (auto fieldIdx = 0; fieldIdx < runing.size(); fieldIdx++)
    // if ((freq = atoi(field)))
    {
      off       = delta[fieldIdx];
      auto freq = runing[fieldIdx];
      {
        while ((t + off) % freq)
          t += step;
        step *= freq;
      }
    }
    return to_string(t);
  }
};
//}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 3", HIDE_IF_OLD_TEST "[x.]")
{

    REQUIRE(Solve(1 + R"(
939
17,x,13,19)")
            .Do6() == "3417");

    REQUIRE(Solve(1 + R"(
939
67,7,59,61)")
            .Do6() == "754018");

    REQUIRE(Solve(1 + R"(
939
67,7,59)")
            .Do5() == "6901");




}
/** /

TEST_CASE(TODAY "Sample2", HIDE_IF_OLD_TEST "[.]")
{
  REQUIRE(Solve(1 + R"(
939
17,x,13,19)")
            .Do4() == "3417");
}


TEST_CASE(TODAY "Sample", HIDE_IF_OLD_TEST "[.]")
{
  REQUIRE(Solve(1 + R"(
939
17,x,13,19)")
            .Do5() == "3417");
}


TEST_CASE(TODAY "Sample 2a", HIDE_IF_OLD_TEST "[.]")
{
  REQUIRE(Solve(1 + R"(
939
17,x,13,19)")
            .Do5() == "3417");
  REQUIRE(Solve(1 + R"(
939
67,7,59,61)")
            .Do5() == "754018");

  REQUIRE(Solve(1 + R"(
939
7,13,x,x,59,x,31,19)")
            .Do5() == "1068781");
}

/**/

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
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do6() == SECOND_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do6());
#endif
}
