// day23 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "day23"

#define FIRST_STAR  "36542897"
#define SECOND_STAR "562136730660"

#ifdef THISDAY
  #define TODAY THISDAY "/"
  #define HIDE_IF_OLD_TEST "[.]"
#else
  #define TODAY
  #define HIDE_IF_OLD_TEST
#endif  // THISDAY

// disable conversion warning
#pragma warning(disable : 4267)

using namespace ranges;

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
  std::list<int> cups;
  
  Solve(const string & inStr){

    for (auto ch : inStr | views::all)
      cups.push_back(ch-'0');

  };

  string Do() { 
  
    auto crPosIt = cups.begin();
    auto sz      = cups.size();
    for (auto i : views::iota(0, 100))
    {
      int  crPos = distance(cups.begin(), crPosIt);
      auto crCup = *crPosIt;

      auto rgnTake3 = cups | views::cycle | views::drop(crPos + 1) | views::take(4) | to<vector>;
      int  nextCr   = rgnTake3.back();
      rgnTake3.erase(prev(rgnTake3.end()));

      auto destination = crCup;

      do
      {
        destination--;
        if (destination == 0)
          destination = 9;
      } while (contains(rgnTake3, destination));

      // remove
      for (auto cup : rgnTake3)
        cups.remove(cup);

      auto whereDest = find(cups, destination);
      copy(rgnTake3, inserter(cups, next(whereDest)));

      crPosIt = find(cups, nextCr);
    
      auto s = cups | views::transform([](auto ch)->char {return '0' +ch;}) | to<string>;

      //cout << "Move " << i + 2 << endl << s << endl;
    }
     
    auto cupOneIt = find(cups, 1);

    auto s = cups 
      | views::cycle 
      | views::drop(distance(cups.begin(), cupOneIt) + 1) 
      | views::take(8)
      | views::transform([](auto ch)->char {return '0' +ch;}) | to<string>;

    return s; 
  }

  string Do2(int count = 100)
  { 
    //;  // 1000000
    // fill
    for (auto i : views::iota(10, count + 1))
      cups.push_back(i);

    auto crPosIt = cups.begin();
    auto sz      = cups.size();

    vector<std::tuple<int, int, int>> next2Atfter1;
    for (auto i : views::iota(0, 10 * count))
    {
      int  crPos = distance(cups.begin(), crPosIt);
      auto crCup = *crPosIt;

      auto rgnTake3 = cups | views::cycle | views::drop(crPos + 1) | views::take(4) | to<vector>;
      int  nextCr   = rgnTake3.back();
      rgnTake3.erase(prev(rgnTake3.end()));

      auto destination = crCup;

      do
      {
        destination--;
        if (destination == 0)
          destination = sz;
      } while (contains(rgnTake3, destination));

      // remove
      for (auto cup : rgnTake3)
        cups.remove(cup);

      auto whereDest = find(cups, destination);
      copy(rgnTake3, inserter(cups, next(whereDest)));

      crPosIt = find(cups, nextCr);

      if (0)
      {
        auto s = cups | views::transform([](auto ch) -> char {
                   return '0' + ch;
                 }) |
                 to<string>;

        cout << "Move " << i + 2 << endl << s << endl;
      }
      if (true || (false && i % 100 == 0))
      {
        auto cupOneIt = find(cups, 1);

        //cout << "i = " << i << endl;
        for (auto cup : cups | views::cycle | views::drop(distance(cups.begin(), cupOneIt)) |
                          views::take(count))
        {
          if (nextCr == cup)
            cout << "(" <<cup << ") ";          
          else
          cout << cup << " ";
        }
        cout << endl;
      }

      if (0)
      {
        auto cupOneIt = find(cups, 1);
        auto next2    = cups | views::cycle | views::drop(distance(cups.begin(), cupOneIt) + 1) |
                     views::take(2) | to<vector>;
        if (!next2Atfter1.empty() && get<0>(next2Atfter1.back()) == next2[0] &&
            get<1>(next2Atfter1.back()) == next2[1])
          get<2>(next2Atfter1.back())++;
        else
        {
          next2Atfter1.emplace_back(next2[0], next2[1], 1);

          if (next2Atfter1.size() > 2)
            cout << " End of seq: " << get<2>(*prev(prev(next2Atfter1.end())));

          cout << "\nNext 2: " << get<0>(next2Atfter1.back()) << ", " << get<1>(next2Atfter1.back())
               << " count: " << get<2>(next2Atfter1.back());
        }
      }

    }

    auto cupOneIt = find(cups, 1);

    auto s = accumulate (cups | views::cycle | views::drop(distance(cups.begin(), cupOneIt) + 1) |
             views::take(2), 1ll, multiplies{});
    
    return to_string(s); }


  void Print(const vector<int> & next, int cup, int crPos) 
  {
    if (cup == 1)
      return;

    if (crPos == cup)
      cout << "(" << cup << ") ";
    else
      cout << cup << " ";

    Print(next, next[cup], crPos);
  }

  string Do3(int count = 1'000'000)
  {
    int    size = 9;
    auto   vec  = cups | views::all | to<vector>;
    vector<int> next(count + 1);
    for (auto i : irange(0, vec.size() - 1))
      next[vec[i]] = vec[i + 1];
    
    next[vec.back()] = 10;

    for (auto i : irange(10, count))
      next[i] = i+1;
    next[count] = vec[0];

    int crPos = vec.front();
    INT64 i     = 0;
    for (; i < 10 * count - 1; i++)
    {
      auto first = next[crPos];
      auto second = next[first];
      auto last  = next[second];

      auto destination = crPos;
      do
      {
        destination--;
        if (destination == 0)
          destination = count;
      } while (destination == first || destination == second ||
               destination == last);
      auto saveLastLink = next[last];

      next[last]      = next[destination];
      next[destination] = first;
      next[crPos]       = saveLastLink;

      crPos = saveLastLink;
      // next[crPos] =

      if (0)
      {
        cout << endl << i << endl << endl;
        Print(next, next[1], crPos);
      }
    }

    return to_string((INT64)next[1] * (INT64)next[next[1]]);
  }
  };
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[.]")
{
  REQUIRE(Solve(1 + R"(
389125467)")
            .Do2(100) == "42");
}

TEST_CASE(TODAY "Sample 2", HIDE_IF_OLD_TEST "[.]")
{
  REQUIRE(Solve(1 + R"(
389125467)")
            .Do3() == "149245887792");
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
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do3());
#endif
}
