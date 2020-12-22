// day22 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"
#include  <list>

//#define THISDAY "day22"

//#define FIRST_STAR  "35562"
//#define SECOND_STAR "34424"

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
  map<int, std::list<int>> mycards;

  Solve(const string & inStr){
   for (auto [idx, chunk] : inStr | views::tokenize(regex("\n\n"), -1) | views::transform(to_string_view()) | views::enumerate)
      for (auto line : chunk | views::tokenize(regex("\n"), -1) | views::tail)
        mycards[idx].push_back(stoi(line));
  };

  using list = std::list<int>;

  pair<int,int> Play(map<int, std::list<int>> cards)
  {
    while (none_of(cards | views::values, &list::empty))
    {
      auto rgn = cards | views::values;

      auto  minMax   = minmax_element(cards, less{}, [](auto & l) {
        return l.second.front();
      });

      auto & winer    = minMax.max->second;
      auto & loser    = minMax.min->second;

      winer.push_back(winer.front());
      winer.push_back(loser.front());

      winer.pop_front();
      loser.pop_front();

      /** /
      if (cards[0].front() > cards[1].front())
      {
        cards[0].push_back(cards[0].front());
        cards[0].push_back(cards[1].front());

        cards[0].pop_front();
        cards[1].pop_front();
      }
      else
      {
        cards[1].push_back(cards[1].front());
        cards[1].push_back(cards[0].front());

        cards[0].pop_front();
        cards[1].pop_front();
      }
      /**/
    }

    int idx = 0;
    if (cards[0].empty())
      idx = 1;


    return { idx, inner_product(cards[idx] | views::reverse, views::iota(1), 0) }; 
  }

  string Do() { return to_string(Play(mycards).second); }
  string Do2() { return to_string(Play2(mycards).second); }
  

  pair<int, int> Play2(map<int, std::list<int>> cards, int level = 0)
  {
    set<pair<std::list<int>, std::list<int>>> history;

    
    int round = 0;
    while (!cards[0].empty() && !cards[1].empty())
    {
      round++;

      if (!history.insert({ cards[0], cards[1] }).second)
        return { 0, 0 };

      //cout << "Round :" << round << "minigame:" << level << std::endl;

       bool zeroWins = false;
       if (cards[0].front() < cards[0].size() && cards[1].front() < cards[1].size())
       {
         map<int, std::list<int>> cardsMinigame;
         cardsMinigame[0] = cards[0] | views::tail | views::take(cards[0].front()) | to<std::list>;
         cardsMinigame[1] = cards[1] | views::tail | views::take(cards[1].front()) | to<std::list>;
         zeroWins         = Play2(std::move(cardsMinigame), level + 1).first == 0;

         //cout << "DECIDED BY MINIGAME\n";
       }
       else
       {
         zeroWins = cards[0].front() > cards[1].front();
       }
       
      if (zeroWins)
      {
        //cout << "Player 1 wins\n";
        cards[0].push_back(cards[0].front());
        cards[0].push_back(cards[1].front());

        cards[0].pop_front();
        cards[1].pop_front();
      }
      else
      {
        //cout << "Player 2 wins\n";
        cards[1].push_back(cards[1].front());
        cards[1].push_back(cards[0].front());

        cards[0].pop_front();
        cards[1].pop_front();
      }

      if (0)
      {
      cout << "Cards state:\nPlayer 1: ";
      for (auto i : cards[0])
        cout << i << " ";
      cout << "\nPlayer 2: ";
      for (auto i : cards[1])
        cout << i << " ";
      cout << std::endl;
      }      
    }

    int idx = 0;
    if (cards[0].empty())
      idx = 1;

    int nr = 0;
    for (auto [pos, card] : cards[idx] | views::enumerate)
      nr += (cards[idx].size() - pos) * card;

    
    return { idx, nr };   
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[.]")
{
  REQUIRE(Solve(1 + R"(
Player 1:
9
2
6
3
1

Player 2:
5
8
4
7
10)")
            .Do() == "306");
}

TEST_CASE(TODAY "Nicu 2", HIDE_IF_OLD_TEST "[.]")
{
  REQUIRE(Solve(1 + R"(
Player 1:
38
1
28
32
43
21
42
29
18
13
39
41
49
31
19
26
27
40
35
14
3
36
12
16
45

Player 2:
34
15
47
20
23
2
11
9
8
7
25
50
48
24
46
44
10
6
22
5
33
30
4
17
37)")
            .Do2() == "34173");
}

TEST_CASE(TODAY "Part One", HIDE_IF_OLD_TEST "[x.]")
{
#ifdef FIRST_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do() == FIRST_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do());
#endif  // FIRST_STAR
}

TEST_CASE(TODAY "Part Two", HIDE_IF_OLD_TEST "[.]")
{
#ifdef SECOND_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do2() == SECOND_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do2());
#endif
}
